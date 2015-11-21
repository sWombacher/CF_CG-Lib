
#include "window2D.h"

int main(int argc, char** argv){
    // window2D provides a lot of image operations
    // additional operations can be used from opencv directly
    typedef cf::Window2D Image;

    if (argc < 3){
        std::cout << "Please provide two image files";
        return -1;
    }
    Image img1(argv[1]);
    Image img2(argv[2]);

    // blend image2 above image1
    int minWidth = img1.getImageWidth();
    if (minWidth > img2.getImageWidth())
        minWidth = img2.getImageWidth();

    int minHeight = img1.getImageHeight();
    if (minHeight > img2.getImageHeight())
        minHeight = img2.getImageHeight();

    Image toDisplay(minWidth, minHeight);
    const float blending_step = 0.1f;

    for (float blend = 0.f; blend <= 1.f; blend += blending_step){
        for (int y = 0; y < minHeight; ++y){
        for (int x = 0; x < minWidth ; ++x){
            cf::Color tmp = (blend * img1.getColor(x, y)) + ((1.f - blend) * img2.getColor(x, y));
            toDisplay.setColor(x, y, tmp);
        }
        }
        toDisplay.show();
        toDisplay.waitKey();
    }
    return 0;
}
