#include "windowRasterized.hpp"

int main(int argc, char** argv){
    // window2D provides a lot of image operations
    // additional operations can be utilized by using opencv directly
    typedef cf::WindowRasterized Image;

    if (argc < 3){
        std::cout << "Please provide two image files";
		cf::Console::waitKey();
        return -1;
    }

    // read images
    Image img1(argv[1]);
    Image img2(argv[2]);
	img1.show();
	img2.show();

    std::cout << "Press keys on a window \"Result\", to blend image1 to image2";

    // blend image2 above image1
    int minWidth = img1.getWidth();
    if (minWidth > img2.getWidth())
        minWidth = img2.getWidth();

    int minHeight = img1.getHeight();
    if (minHeight > img2.getHeight())
        minHeight = img2.getHeight();

    Image toDisplay(minWidth, minHeight, "Result");
    const float blending_step = 0.1f;

    for (float blend = 0.f; blend <= 1.f; blend += blending_step){
        for (int y = 0; y < minHeight; ++y){
        for (int x = 0; x < minWidth ; ++x){
            // just a simple way to blend two colors
            cf::Color tmp = (blend * img1.getColor(x, y)) + ((1.f - blend) * img2.getColor(x, y));
            toDisplay.setColor(x, y, tmp);
        }
        }
        // show result and wait for key input on a window
        toDisplay.show();
        toDisplay.waitKey();
    }
    return 0;
}
