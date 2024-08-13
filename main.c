#include "lapis.h"
int main(int argc, char** argv) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <depth> <input> <output>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int depth = atoi(argv[1]);
    convert(&depth, argv[2], argv[3]);

    return 0;
}