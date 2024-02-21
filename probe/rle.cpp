#include <iostream>
#include <cassert>

#include "../ppmm.h"

using namespace ppmm;

void blank_image(int width, int height, int format)
{
    std::cout
        << "P3" << std::endl
        << width << " " << height << std::endl
        << "255" << std::endl;

    for (int i = 0; i < width*height*format; ++i) {
        std::cout << 255;
        
        if (i % (width*format) == width*format - 1) {
            std::cout << std::endl;
        } else {
            std::cout << " ";
        }
    }
}

int main(void)
{
    blank_image(250, 250, 3);
    return 0;

    //Image img("blank.ppm");
    Image img("random.ppm");
    assert(img.IsOpen());

    //int width = img.GetWidth();
    int size = img.GetSize();
    //int format = img.GetFormat();

    unsigned char cur = img[0];
    int e = 1;

    for (int i = 1; i < size; ++i) {
        if (img[i] != cur) {
            std::cout << e << " " << (int)cur << std::endl;
            cur = img[i];
            e = 1;
        } else {
            ++e;
        }

        //std::cout << (int)img[i];
        //if (i % (width*format) == width*format - 1) {
        //    std::cout << std::endl;
        //} else {
        //    std::cout << " ";
        //}
    }

    std::cout << e << " " << (int)cur << std::endl;

    return 0;
}
