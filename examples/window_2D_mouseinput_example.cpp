



#include "window.h"


void mouse(int button, int state, int x, int y){
    std::cout << "x: " << x << "\ty: " << y <<std::endl;
}



int main(int argc, char** argv) {

    cf::Window2D w(800,600);
    w.init(&argc, argv, mouse);

    for (int y = 0; y < w.getWindowHeight(); ++y){
        for (int x = 0; x < w.getWindowWidth() / 2;  ++x)
            w.setPixelColor(x, y, cf::Color(255, 255, 255));

        for (int x = w.getWindowWidth() / 2; x < w.getWindowWidth();  ++x)
            w.setPixelColor(x, y, cf::Color(255, 0, 0));
    }
    w.draw();
    getchar();

    const int step_count = 10;
    while(w.getPixelColor(0, 0) == cf::Color(255, 255, 255)){

        for (int i = 0; i < step_count; ++i){
            for (int y = 0; y < w.getWindowHeight(); ++y){
            for (int x = 0; x < w.getWindowWidth() - 1;  ++x){
                cf::Color col = w.getPixelColor(x + 1, y);
                w.setPixelColor(x, y, col);
            }
            }
        }

        w.draw();
        getchar();
    }
    return 0;
}
