

#include "window2D.h"


int main(int argc, char** argv) {

    cf::Window2D w;
    for (int y = 0; y < w.getImageHeight(); ++y){
        for (int x = 0; x < w.getImageWidth() / 2;  ++x)
            w.setColor(x, y, cf::Color(255, 255, 255));

        for (int x = w.getImageWidth() / 2; x < w.getImageWidth();  ++x)
            w.setColor(x, y, cf::Color::BLUE);
    }
    w.show();

    const int step_count = 100;
    while(w.getColor(0, 0) == cf::Color::WHITE){
        for (int i = 0; i < step_count; ++i){
            for (int y = 0; y < w.getImageHeight()   ; ++y){
            for (int x = 0; x < w.getImageWidth() - 1; ++x){
                cf::Color& col = w.getColor(x + 1, y);
                w.setColor(x, y, col);

                // alternative (and faster) version
                // NOTE: by using this version you have to deal with y-axis inverting and invervall changing yourself
                //
                //cv::Mat& img = w.getImage();
                //img.at<cf::Color>(y, x) = img.at<cf::Color>(y, x + 1);
            }
            }
        }
        w.show();
        w.waitKey();
    }
    return 0;
}
