

#include "window2D.h"


int main(int argc, char** argv) {

    cf::Window2D window(200, 100, "Hello World!");
    window.setInvertYAxis(true);

    window.setColor(10, 10, cf::Color(0, 0, 255));
    window.drawCircle(cf::Point(100, 100), 20, -1, cf::Color(255, 0, 0));

    window.drawRectangle(cf::Point(0, 0), cf::Point(20, 20), -1, cf::Color(255, 0, 0));

    window.setWindowScale(1.f);
    window.show();

    //window.setNewIntervall(cf::Intervall(1.f, 2.f), cf::Intervall(-0.1f, 1000.f));
    window.setWindowScale(5.f);

    float x, y;
    while(false){
        window.waitMouseInput(x, y);
        std::cout << "x: " << x << "\ty: " << y << std::endl;
    }


    std::string str = "bamlee\rpeter";
    cf::removeWindowsSpecificCarriageReturn(str);
    str += "peter";
    std::cout << str << std::endl;

    window.waitKey();
    return 0;
}
