#include "windowRasterized.hpp"

int main(int argc, char** argv){
    // window2D provides a lot of image operations
    // additional operations can be utilized by using opencv directly
    typedef cf::WindowRasterized Image;

	std::string image_path1, image_path2;
    if (argc < 3){
        std::cout << "Please provide two image files, if you want to use custom images.\n";
		image_path1 = CHAOS_FILE_PATH;
		image_path1 += "x1.png";
		
		image_path2 = CHAOS_FILE_PATH;
		image_path2 += "x2.png";
    }
	else {
		image_path1 = argv[1];
		image_path2 = argv[2];
	}

    // read images
    Image img1(image_path1.c_str());
    Image img2(image_path2.c_str());
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
	cf::Console::clearConsole();
	std::cout << "Press any key to finish the process." << std::endl;
	toDisplay.show();
	toDisplay.waitKey();
    return 0;
}
