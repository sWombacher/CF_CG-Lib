#ifdef _WIN32
// enable exception handling for windows
// this requires 'int main(int, char**)' function definition
// therefore 'int main()' is dissabled
#define CFCG_EXCEPTION_HANDLING
#endif

#include "computerGeometry3D.hpp"
#include "windowCoordinateSystem3D.h"

int main(int argc, char** argv) {
    cf::WindowCoordinateSystem3D system(&argc, argv);
    system.beginDrawing([&](){
        system.drawPlane({1, 1, 1, 0}, cf::Color::RED, 127);
        system.waitKey();
        system.drawLine({0, 0, 0, 0}, {1, 1, 0, 0});
        system.drawSphere({1, 1, 1, 0});
    });
    return 0;
}
