#include "lapis.h"

int main(int argc, char** argv) {
    int depth = 6;
    const char *input = "test/img/test.png";
    const char *output = "test/output.png";
    convert(&depth, input, output);

    return 0;
}