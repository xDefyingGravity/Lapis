#include "lapis.h"

int strength = 1;
int* COMBINE_STRENGTH = &strength;

int nearestMultiple(int num, int multiple) {
    int remainder = num % multiple;
    if (remainder == 0) {
        return num;
    } else {
        return num + (multiple - remainder);
    }
}

void resizeImage(Image *img, int depth) {
    int newWidth = nearestMultiple(img->width, depth);
    int newHeight = nearestMultiple(img->height, depth);

    if (newWidth != img->width || newHeight != img->height) {
        Image resizedImg;
        initReconstructedImage(&resizedImg, newWidth, newHeight);
        for (int y = 0; y < newHeight; y++) {
            for (int x = 0; x < newWidth; x++) {
                if (y < img->height && x < img->width) {
                    resizedImg.pixels[y][x] = img->pixels[y][x];
                } else {
                    resizedImg.pixels[y][x] = resizedImg.pixels[y][x - 1];
                }
            }
        }

        freeImage(img);
        *img = resizedImg;
    }
}


void initImage(Image *image) {
    image->width = WIDTH;
    image->height = HEIGHT;
    image->pixels = (Pixel **)malloc(HEIGHT * sizeof(Pixel *));
    if (image->pixels == NULL) {
        perror("Error allocating memory for image pixels");
        exit(EXIT_FAILURE);
    }
    for (int y = 0; y < HEIGHT; y++) {
        image->pixels[y] = (Pixel *)malloc(WIDTH * sizeof(Pixel)); 
        if (image->pixels[y] == NULL) {
            perror("Error allocating memory for image row");
            exit(EXIT_FAILURE);
        }
        for (int x = 0; x < WIDTH; x++) {
            image->pixels[y][x] = (Pixel){0, 0, 0};
        }
    }
}


void freeImage(Image *image) {
    for (int y = 0; y < HEIGHT; y++) {
        free(image->pixels[y]);
    }
    free(image->pixels);
}

Image readPNG(const char* filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        perror("Error reading input file");
        exit(EXIT_FAILURE);
    }

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) {
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        png_destroy_read_struct(&png, NULL, NULL);
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    if (setjmp(png_jmpbuf(png))) {
        png_destroy_read_struct(&png, &info, NULL);
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    png_init_io(png, fp);
    png_read_info(png, info);

    int width = png_get_image_width(png, info);
    int height = png_get_image_height(png, info);
    png_byte color_type = png_get_color_type(png, info);
    png_byte bit_depth = png_get_bit_depth(png, info);

    if (bit_depth == 16) png_set_strip_16(png);
    if (color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png);
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) png_set_expand_gray_1_2_4_to_8(png);
    if (png_get_valid(png, info, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png);
    if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
    if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png);

    png_read_update_info(png, info);

    Image img;
    img.width = width;
    img.height = height;
    img.pixels = (Pixel **)malloc(height * sizeof(Pixel *));
    for (int y = 0; y < height; y++) {
        img.pixels[y] = (Pixel *)malloc(width * sizeof(Pixel));
    }

    png_bytep row_pointers[height];
    for (int y = 0; y < height; y++) {
        row_pointers[y] = (png_bytep)malloc(png_get_rowbytes(png, info));
    }

    png_read_image(png, row_pointers);

    for (int y = 0; y < height; y++) {
        png_bytep row = row_pointers[y];
        for (int x = 0; x < width; x++) {
            png_bytep px = &(row[x * 4]);
            img.pixels[y][x].r = px[0];
            img.pixels[y][x].g = px[1];
            img.pixels[y][x].b = px[2];
        }
    }

    for (int y = 0; y < height; y++) {
        free(row_pointers[y]);
    }

    png_destroy_read_struct(&png, &info, NULL);
    fclose(fp);

    return img;
}
void writePNG(const char* filename, Image img) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) {
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        png_destroy_write_struct(&png, NULL);
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    if (setjmp(png_jmpbuf(png))) {
        png_destroy_write_struct(&png, &info);
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    png_init_io(png, fp);

    png_set_IHDR(png, info, img.width, img.height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    png_write_info(png, info);

    for (int y = 0; y < img.height; y++) {
        png_write_row(png, (png_const_bytep)img.pixels[y]);

    }

    png_write_end(png, NULL);

    png_destroy_write_struct(&png, &info);

    fclose(fp);
}
void initReconstructedImage(Image *image, int width, int height) {
    image->width = width;
    image->height = height;
    image->pixels = (Pixel **)malloc(height * sizeof(Pixel *) + height * width * sizeof(Pixel));
    if (image->pixels == NULL) {
        perror("Error allocating memory for reconstructed image pixels");
        exit(EXIT_FAILURE);
    }
    // Set up pixel pointers
    for (int y = 0; y < height; y++) {
        image->pixels[y] = (Pixel *)(image->pixels + height) + y * width;
    }
}



PixelGroups groupPixels(Image img) {
    int groupWidth = img.width / *COMBINE_STRENGTH;
    int groupHeight = img.height / *COMBINE_STRENGTH;
    groupHeight += (img.height % *COMBINE_STRENGTH != 0) ? 1 : 0;

    PixelGroups groups;
    groups.width = groupWidth;
    groups.height = groupHeight;
    groups.pixels = (PixelGroup **)malloc(groupHeight * sizeof(PixelGroup *));
    if (groups.pixels == NULL) {
        perror("Error allocating memory for PixelGroups");
        exit(EXIT_FAILURE);
    }

    for (int y = 0; y < groupHeight; y++) {
        groups.pixels[y] = (PixelGroup *)malloc(groupWidth * sizeof(PixelGroup));
        if (groups.pixels[y] == NULL) {
            perror("Error allocating memory for PixelGroup row");
            exit(EXIT_FAILURE);
        }
        for (int x = 0; x < groupWidth; x++) {
            groups.pixels[y][x].pixels = (Pixel **)malloc(*COMBINE_STRENGTH * sizeof(Pixel *));
            if (groups.pixels[y][x].pixels == NULL) {
                perror("Error allocating memory for PixelGroup pixels");
                exit(EXIT_FAILURE);
            }
            for (int i = 0; i < *COMBINE_STRENGTH; i++) {
                groups.pixels[y][x].pixels[i] = (Pixel *)malloc(*COMBINE_STRENGTH * sizeof(Pixel));
                if (groups.pixels[y][x].pixels[i] == NULL) {
                    perror("Error allocating memory for PixelGroup pixel row");
                    exit(EXIT_FAILURE);
                }
            }

            for (int dy = 0; dy < *COMBINE_STRENGTH; dy++) {
                for (int dx = 0; dx < *COMBINE_STRENGTH; dx++) {
                    if ((y * *COMBINE_STRENGTH + dy) < img.height && (x * *COMBINE_STRENGTH + dx) < img.width) {
                        groups.pixels[y][x].pixels[dy][dx] = img.pixels[y * *COMBINE_STRENGTH + dy][x * *COMBINE_STRENGTH + dx];
                    } else {
                        groups.pixels[y][x].pixels[dy][dx] = (Pixel){0, 0, 0};
                    }
                }
            }
        }
    }
    return groups;
}
void freePixelGroups(PixelGroups groups) {
    free(groups.pixels);
}

Pixel blendPixels(PixelGroup* group) {
    if (group == NULL || group->pixels == NULL) {
        return (Pixel){0, 0, 0};
    }

    int totalR = 0, totalG = 0, totalB = 0;
    int numPixels = *COMBINE_STRENGTH * *COMBINE_STRENGTH; 
    for (int dy = 0; dy < *COMBINE_STRENGTH; dy++) {
        for (int dx = 0; dx < *COMBINE_STRENGTH; dx++) {
            totalR += group->pixels[dy][dx].r;
            totalG += group->pixels[dy][dx].g;
            totalB += group->pixels[dy][dx].b;
        }
    }

    Pixel avgPixel = (Pixel){0, 0, 0};

    avgPixel.r = totalR / numPixels;
    avgPixel.g = totalG / numPixels;
    avgPixel.b = totalB / numPixels;

    return avgPixel;
}



int convert(int* depth, char* filename, char* output) {

    if (depth != NULL) {
        *COMBINE_STRENGTH = *depth;
    } else {
        *COMBINE_STRENGTH = 1;
    }

    Image img = readPNG(filename);
    resizeImage(&img, *depth);
    PixelGroups groups = groupPixels(img);

    Image reconstructedImg;
    initReconstructedImage(&reconstructedImg, img.width, img.height);
    Pixel avgPixel = (Pixel){0, 0, 0};
    for (int y = 0; y < groups.height; y++) {
        for (int x = 0; x < groups.width; x++) {
            avgPixel = blendPixels(&groups.pixels[y][x]);
            for (int dy = 0; dy < *COMBINE_STRENGTH; dy++) {
                for (int dx = 0; dx < *COMBINE_STRENGTH; dx++) {
                    int ry = y * *COMBINE_STRENGTH + dy;
                    int rx = x * *COMBINE_STRENGTH + dx;
                    if (ry < reconstructedImg.height && rx < reconstructedImg.width) {
                        
                        reconstructedImg.pixels[ry][rx] = avgPixel;
                    }
                }
            }
        }
    }

    writePNG(output, reconstructedImg);
    //freeImage(&img);
    freePixelGroups(groups);


    return 0;
}
//lapis.c