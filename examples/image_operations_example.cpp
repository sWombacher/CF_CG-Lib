#ifdef _WIN32
// enable exception handling for windows
// this requires 'int main(int, char**)' function definition
// therefore 'int main()' is dissabled
#define CFCG_EXCEPTION_HANDLING
#endif

#include "windowRasterized.h"

int main(int argc, char** argv) {
    // window2D provides a lot of image operations
    // additional operations can be utilized by using opencv directly
    using Image = cf::WindowRasterized;

    std::string image_path1, image_path2;
    if (argc < 3) {
        std::cout << "Please provide two image files, if you want to use custom images.\n";
        image_path1 = CHAOS_FILE_PATH;
        image_path1 += "x1.png";

        image_path2 = CHAOS_FILE_PATH;
        image_path2 += "x2.png";
    } else {
        image_path1 = argv[1];
        image_path2 = argv[2];
    }

    // read images
    Image img1(image_path1.c_str());
    Image img2(image_path2.c_str());
    img1.show();
    img2.show();

    // blend image2 above image1
    const int minWidth = std::min(img1.getWidth(), img2.getWidth());
    const int minHeight = std::min(img1.getHeight(), img2.getHeight());

    Image toDisplay(minWidth, minHeight, "Result");
    const float blending_step = 0.1f;

	std::cout << "Press any key on a window to blend image1 to image2.\n" << std::endl;
	toDisplay.waitKey();

    for (float blend = 0.f; blend <= 1.0001f; blend += blending_step) {  // das Epsilon 0.0001 muss sein ;(
        for (int y = 0; y < minHeight; ++y) {
            for (int x = 0; x < minWidth; ++x) {
                // just a simple way to blend two colors
                cf::Color tmp = (blend * img1.getColor(x, y)) + ((1.f - blend) * img2.getColor(x, y));
                toDisplay.setColor(x, y, tmp);
            }
        }
        // show result and wait for key input on a window
		toDisplay.show();
		std::cout << "Press key once more." << std::endl;
        toDisplay.waitKey();
	}
    cf::Console::clearConsole();
	std::cout << "Press enter to finish the process";
	cf::Console::waitKey();
    return 0;
}
