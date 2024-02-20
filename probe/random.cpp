#include <iostream>
#include <cstdlib>

int rand_range(int min, int max)
{
    return rand() % (max - min) + min;
}

void generate_random_image(int width, int height)
{
    std::cout << "P3" << std::endl;
    std::cout << width << " " << height << std::endl;
    std::cout << "255" << std::endl;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            std::cout << rand_range(0, 256) << " ";
            std::cout << rand_range(0, 256) << " ";
            std::cout << rand_range(0, 256) << " ";
        }
        std::cout << std::endl;
    }
}

int main(void)
{
    srand(time(NULL));

    generate_random_image(3, 3);

    return 0;
}
