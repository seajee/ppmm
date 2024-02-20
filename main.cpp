#include <iostream>

#include "ppmm.h"

int main(void)
{
    using namespace ppmm;

    Image img("cat.ppm");
    if (!img.IsOpen()) {
        std::cerr << "ERROR: Could not open file" << std::endl;
        return 1;
    }

    Filter filter(img);
    filter.Grayscale();

    if (!img.WriteToFile("cat_grayscale.ppm")) {
        std::cerr << "ERROR: Could not write file" << std::endl;
        return 1;
    }

    filter.Inverted();

    if (!img.WriteToFile("cat_inverted.ppm")) {
        std::cerr << "ERROR: Could not write file" << std::endl;
        return 1;
    }

    return 0;
}
