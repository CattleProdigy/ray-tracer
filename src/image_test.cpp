#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <png++/png.hpp>
#include <Eigen/Core>

using namespace std;

int main() {

        
    png::image< png::rgb_pixel > test_image(256, 256);
    Eigen::Vector3f v3;

    for (size_t y = 0; y < test_image.get_height(); ++y) {

        for (size_t x = 0; x < test_image.get_width(); ++x) {

            v3 = Eigen::Vector3f(x, y, x+y);
            test_image[y][x] = png::rgb_pixel(x, y, x + y);

        }

    }

    test_image.write("test_image.png");

    return 0;
} 
