#include <iostream>

#include "ppmm.h"

int main(int argc, char **argv)
{
    using namespace ppmm;

    // Check program usage
    if (argc < 3) {
        std::cerr
            << "ERROR: Incorrect usage" << std::endl
            << "Usage: " << argv[0] << " g/i/m/c <file> <output>" << std::endl
            << "  g: grayscale" << std::endl
            << "  i: inverted" << std::endl
            << "  m: multiply by factor" << std::endl
            << "  c: multiply by color" << std::endl;
        return 1;
    }

    const char parameter = *argv[1];
    const char *input_filepath = argv[2];
    const char *output_filepath = argv[3];

    // Open image file
    Image img(input_filepath);
    if (!img.IsOpen()) {
        std::cerr << "ERROR: Could not open file" << input_filepath << std::endl;
        return 1;
    }

    // Create a filter for the image
    Filter filter(img);

    // Apply the correct filter
    switch (parameter) {
    case 'g':
        filter.Grayscale();
        break;
    case 'i':
        filter.Inverted();
        break;
    case 'm':
        filter.Multiply(2.0);
        break;
    case 'c':
        filter.Multiply(0.2, 0.5, 0.9);
        break;
    default:
        std::cerr << "ERROR: parameter not recognized" << std::endl;
        return 1;
    }

    // Finally write the manipulated image to a file
    if (!img.WriteToFile(output_filepath)) {
        std::cerr << "ERROR: Could not write to " << output_filepath << std::endl;
        return 1;
    }

    return 0;
}
