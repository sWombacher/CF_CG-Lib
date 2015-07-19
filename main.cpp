

#include "window2D.h"


int main(int argc, char** argv) {

    cf::Window2D window(200, 100, "Hello World!");
    window.setInvertYAxis(true);

    window.setColor(10, 10, cf::Color(0, 0, 255));
    window.drawCircle(cf::Point(100, 100), 10, 5, cf::Color(255, 0, 0));

    window.setWindowScale(1.f);
    window.show();

    float x, y;

    while(true){
        window.waitMouseInput(x, y);
        std::cout << "x: " << x << "\ty: " << y << std::endl;
    }

    window.waitKey();
    return 0;
}
