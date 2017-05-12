#ifdef _WIN32
// enable exception handling for windows
// this requires 'int main(int, char**)' function definition
// therefore 'int main()' is dissabled
#define CFCG_EXCEPTION_HANDLING
#endif


#include "computerGeometry3D.hpp"
#include "windowCoordinateSystem3D.h"

int main(int argc, char** argv){
    cf::WindowCoordinateSystem3D system(&argc, argv);
    system.drawPlane({1,1,1,0});
    system.drawLine({0,0,0,0}, {1,1,0,0});
    system.drawPoint({10,10,10,0});
    system.waitFinish();
    return 0;
}


