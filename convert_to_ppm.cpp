#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int main(int argc, char **argv)
{
    if (argc <= 1) {
        std::cerr
            << "ERROR: No input file provided" << std::endl
            << "Usage: " << argv[0] << " <filepath>" << std::endl;
        return 1;
    }

    const char *filename = argv[1];

    int width;
    int height;
    int format;

    unsigned char *data = stbi_load(filename, &width, &height, &format, 3);
    if (data == NULL) {
        std::cerr << "ERROR: Could not load file" << std::endl;
        return 1;
    }

    std::cout
        << "P3" << std::endl
        << width << " " << height << std::endl
        << "255" << std::endl;

    for (int i = 0; i < width*height*format; ++i) {
        std::cout << (int)data[i];
        
        if (i % (width*format) == width*format - 1) {
            std::cout << std::endl;
        } else {
            std::cout << " ";
        }
    }

    stbi_image_free(data);

    return 0;
}
