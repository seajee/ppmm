#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

void filter_grayscale(unsigned char *pixels, int width, int height, int comp)
{
    for (int i = 0; i < width*height*comp; i += comp) {
        float rl = (float)pixels[i + 0] / 255.0;
        float gl = (float)pixels[i + 1] / 255.0;
        float bl = (float)pixels[i + 2] / 255.0;

        float y = 0.2126*rl + 0.7152*gl + 0.0722*bl;

        pixels[i + 0] = (unsigned char)(y*255);
        pixels[i + 1] = (unsigned char)(y*255);
        pixels[i + 2] = (unsigned char)(y*255);
    }
}

void filter_inverted(unsigned char *pixels, int width, int height, int comp)
{
    for (int i = 0; i < width*height*comp; i += comp) {
        unsigned char r = pixels[i + 0];
        unsigned char g = pixels[i + 1];
        unsigned char b = pixels[i + 2];

        pixels[i + 0] = 255 - r;
        pixels[i + 1] = 255 - g;
        pixels[i + 2] = 255 - b;
    }
}

int main(void)
{
    const char *file_path = "cat.ppm";

    std::ifstream input_file(file_path);
    if (!input_file.is_open()) {
        std::cerr << "ERROR: Could not open file " << file_path << std::endl;
        return 1;
    }

    char format_cstr[3] = { 0 };
    input_file.read(format_cstr, 2);
    std::string format(format_cstr);

    if (format != "P3") {
        std::cerr << "ERROR: Not a PPM image" << std::endl;
        return 1;
    }

    int width;
    int height;
    const int comp = 3;
    int depth;

    input_file >> width;
    input_file >> height;
    input_file >> depth;

    int pixel_count = width * height;

    unsigned char pixels[pixel_count * comp];

    for (int i = 0; i < pixel_count * comp; i += comp) {
        int r, g, b;

        input_file >> r;
        input_file >> g;
        input_file >> b;

        pixels[i + 0] = r;
        pixels[i + 1] = g;
        pixels[i + 2] = b;
    }

    input_file.close();

    filter_grayscale(pixels, width, height, comp);
    filter_inverted(pixels, width, height, comp);

    std::cout
        << "P3" << std::endl
        << width << " " << height << std::endl
        << "255" << std::endl;

    for (int i = 0; i < width*height*comp; ++i) {
        std::cout << (int)pixels[i];
        
        if (i % (width*comp) == width*comp - 1) {
            std::cout << std::endl;
        } else {
            std::cout << " ";
        }
    }

    return 0;
}
