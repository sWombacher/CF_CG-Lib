

#include "window2D.h"


int main(int argc, char** argv) {

    cf::Window_2D window(200, 100, "Hello World!");
    window.setColor(10, 10, cf::Color(0, 0, 255));
    window.drawCircle(cv::Point(100, 100), 10, 5, cf::Color(255, 0, 0));

    window.setScale(5.f);
    window.show();
    //window.waitKey();

    window.waitKey();
    return 0;
}
