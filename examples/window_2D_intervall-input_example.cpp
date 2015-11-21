
#include "window2D.h"
#include <iomanip> // set precision


int main(int argc, char** argv) {

    cf::Window2D w(800,600);

    for (int y = 0; y < w.getImageHeight(); ++y){
        for (int x = 0; x < w.getImageWidth() / 2;  ++x)
            w.setColor(x, y, cf::Color::ORANGE);

        for (int x = w.getImageWidth() / 2; x < w.getImageWidth();  ++x)
            w.setColor(x, y, cf::Color::WHITE);
    }
    float x, y;
    w.show();

    for (int i = 0; i < 3; ++i){
        // waitMouseInput uses floating point values, this is required for exchanging intervalls
        w.waitMouseInput(x, y);
        std::cout << "x: " << x << "\ty: " << y <<std::endl;
    }

    std::cout << std::fixed << std::setprecision(3); // set cout to use 3 post dot digits
    w.setNewIntervall(cf::Intervall(0.f, 1.f), cf::Intervall(10.f, 100.f));
    for (int i = 0; i < 6; ++i){
        if (i == 3)
            w.setInvertYAxis(false); // invert y-values, top to bottom

        // waitMouseInput uses floating point values, this is required for exchanging intervalls
        w.waitMouseInput(x, y);
        std::cout << "x: " << x << "\ty: " << y <<std::endl;
    }


    for (int i = 0; i < 3; ++i){
        unsigned char key = w.waitKey(0);
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
