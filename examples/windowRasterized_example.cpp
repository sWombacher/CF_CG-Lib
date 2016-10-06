#include "windowRasterized.hpp"
#include <iostream>
#include <random>
#include <string>

int main(int argc, char** argv){
    std::string imagePath;
    std::string LUT_path;
    if (argc < 3) {
        std::cout << "Usage for custom images/LUT's:\n1. console parameter: full iamge path\n2. console parameter: full *.pal path\n\n\n";

        imagePath = CHAOS_FILE_PATH;
        imagePath += "x1.png";

        LUT_path = CHAOS_FILE_PATH;
        LUT_path += "Mandel.pal";
    }
    else {
        imagePath = argv[1];
        LUT_path = argv[2];
    }

// read image and LUT file
    cf::WindowRasterized image(imagePath.c_str());
    std::vector<cf::Color> LUT = cf::readPaletteFromFile(LUT_path);

    // a LUT should always contain 256 elements
    if (LUT.size() < 256)
        throw std::runtime_error("Error: please validate your LUT file");

    // scale image on function call "show", this has no effect on width/height/pixel of the actual image
    image.setWindowDisplayScale(5.f);
    image.show();

    // NOTE:
    //  it is possible that "show" function finishes to fast (most likely on first show call)
    //  in those cases the image might not be displayed correctly
    //  this issue can be solved by calling "waitKey"
    //image.waitKey(1000);

    std::cout << "Press enter to continue\n";
    image.waitKey();

// transform image into pseudo color image
    for (int y = 0; y < image.getHeight(); ++y){
    for (int x = 0; x < image.getHeight(); ++x){
        // a grey image has been loaded
        // -> r/g/b values are equal
        // use the r channel as a reference grey channel
        const cf::Color color = LUT[image.getColor(x, y).r];
        image.setColor(x, y, color);
    }
    }
    image.show();

    std::cout << "Press enter to continue\n";
    image.waitKey();

// generate a LUT of random numbers
    // initialize random generator with seed
    std::mt19937 gen(std::random_device().operator()()); // more "random" generator
    //std::mt19937 gen(42); // pseudo random geneartor

    // we want values in the range of [0, 255]
    std::uniform_int_distribution<int> colorDistribution(0, 255);

    // example for floating point generator:
    //std::uniform_real_distribution<float> distribution(-1.f, 10.f);
    //distribution(gen);

    for (unsigned int i = 0; i < LUT.size(); ++i){
        LUT[i].r = colorDistribution(gen);
        LUT[i].g = colorDistribution(gen);
        LUT[i].b = colorDistribution(gen);
    }

    // short version from above
    for (int y = 0; y < image.getHeight(); ++y){
    for (int x = 0; x < image.getHeight(); ++x)
        image.setColor(x, y, LUT[image.getColor(x, y).r]);
    }
    image.show();

    std::cout << "Press enter to continue\n";
    image.waitKey();

    // invert diagonal pixels
    for (int x = 0, y = 0;
         x < image.getWidth() && y < image.getHeight();
         ++x, ++y)
    {
        cf::Color c = image.getColor(x, y);
        c = c.invert();
        image.setColor(x, y, c);
    }
    image.show();

    std::cout << "Press enter to continue\n";
    image.waitKey();

    // color diagonal pixels black/white based on brightness
    for (int y = image.getHeight() - 1, x = 0;
         y >= 0 && x < image.getWidth();
         --y, ++x)
    {
        cf::Color c = image.getColor(x, y);

        // calculate simple average of RGB for brightness
        // a more preceise solution:
        // 0.21 R + 0.72 G + 0.07 B, source: http://www.johndcook.com/blog/2009/08/24/algorithms-convert-color-grayscale/
        int brightness = c.r + c.g + c.b;
        brightness /= 3;
        if (brightness < 128)
            image.setColor(x, y, cf::Color::WHITE);
        else
            image.setColor(x, y, cf::Color::BLACK);
    }
    image.show();

    std::cout << "Press enter to finish\n";
    image.waitKey();
    return 0;
}
