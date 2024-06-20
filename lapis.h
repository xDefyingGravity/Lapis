#ifndef __LAPIS__H__
#define __LAPIS__H__


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <png.h>

#define WIDTH  640
#define HEIGHT 480
#define min(a, b) ((a) < (b) ? (a) : (b))

extern int* COMBINE_STRENGTH;


typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} Pixel;

typedef struct {
    Pixel **pixels;
} PixelGroup;

typedef struct {
    int width, height;
    Pixel **pixels;
} Image;

typedef struct {
    int width, height;
    PixelGroup **pixels;
} PixelGroups;

void initImage(Image *image);
void freeImage(Image *image);
Image readPNG(const char* filename);
void writePNG(const char* filename, Image img);
void initReconstructedImage(Image *image, int width, int height);
PixelGroups groupPixels(Image img);
void freePixelGroups(PixelGroups groups);
Pixel blendPixels(PixelGroup* group);
int convert(int* depth, char* filename, char* output);


#endif