#include "windowVercorized.hpp"
#include <iomanip> // set precision


int main(void) {
	std::cout << "Use your mouse to set points on the window\n\n";
    cf::Window2D window(800, 600);

    // init image colors to orange
    // set pixel one by one
    // this method is slow and is only to show the API
    // prefer to use "window.colorWholeImage(cf::Color::ORANGE);"
    for (int y = 0; y < window.getHeight(); ++y){
    for (int x = 0; x < window.getWidth() ; ++x){
        window.setColor(x, y, cf::Color::ORANGE);
    }
    }
    //window.clear(cf::Color::ORANGE);
    window.show();
    float x, y;

    std::cout << std::fixed << std::setprecision(3); // set cout to use 3 post dot digits

    // draw 3 user provided points on the screen
    std::cout << "Mouse position - standard intervall" << std::endl;
    for (int i = 0; i < 3; ++i){
        // waitMouseInput uses floating point values, this is required for exchanging intervalls
        window.waitMouseInput(x, y);
        window.drawCircle(cf::Point(x, y), 2, 2, cf::Color::BLACK);
        std::cout << "x: " << x << "\ty: " << y <<std::endl;
        window.show();
    }

    // draw a line between 2 user provided points
    std::cout << "\nMouse position - changed intervall" << std::endl;
    window.setNewIntervall(cf::Intervall(0.f, 1.f), cf::Intervall(10.f, 100.f));
    cf::Point firstPoint(0, 0);
    for (int i = 0; i < 2; ++i){
        // waitMouseInput uses floating point values, this is required for exchanging intervalls
        window.waitMouseInput(x, y);
        std::cout << "x: " << x << "\ty: " << y <<std::endl;
        if (i == 0)
            firstPoint = cf::Point(x, y);
        else {
            cf::Point secondPoint(x, y);
            window.drawLine(firstPoint, secondPoint, 2, cf::Color(255, 255, 255));

            // show uer points
            cf::Color col = cf::Color::MAGENTA;
            window.drawCircle(firstPoint, 2, 2, col);
            window.drawCircle(secondPoint, 2, 2, col.invert());
        }
        window.show();
    }


    std::cout << "\nMouse position - changed intervall & inverted y-values" << std::endl;
    window.setInvertYAxis(true); // invert y-values (bottom line = intervall minimum)
    for (int i = 0; i < 2; ++i){
        // waitMouseInput uses floating point values, this is required for exchanging intervalls
        window.waitMouseInput(x, y);
        std::cout << "x: " << x << "\ty: " << y <<std::endl;
        if (i == 0)
            firstPoint = cf::Point(x, y);
        else
            window.drawRectangle(firstPoint, cf::Point(x, y), 2, cf::Color::GREEN);
        window.show();
    }


	std::cout << "\n\n\n\nUse the keyboard on active window to display keys\n";
    std::cout << "\nKey input test" << std::endl;
    for (int i = 0; i < 3; ++i){
        unsigned char key = window.waitKey(0);
        if (key == 27 /*esc-key*/){
            std::cout << "Esc-Key pressed, closing program" << std::endl;
            break;
        }
        else if (key == 'a')
            std::cout << "a Key pressed" << std::endl;
        else
            std::cout << "Key: " << key << std::endl;
    }
    return 0;
}
