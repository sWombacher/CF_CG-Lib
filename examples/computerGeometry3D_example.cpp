#ifdef _WIN32
// enable exception handling for windows
// this requires 'int main(int, char**)' function definition
// therefore 'int main()' is dissabled
#define CFCG_EXCEPTION_HANDLING
#endif

#include "windowCoordinateSystem3D.h"

int main(int argc, char** argv) {
    using namespace cf::literals;
    using Blade = cf::dMultiVector::Blade;
    using STYPE = cf::WindowCoordinateSystem3D::SPACE_TYPE;
    cf::dMultiVector vec(Blade(Blade::TYPE::E0, 1.0), Blade(Blade::TYPE::E1, 2.0), Blade(Blade::TYPE::E2, 0.0));

    cf::WindowCoordinateSystem3D system(&argc, argv);
    system.beginDrawing([&]() {
        vec = 1.0_e0 + 1.0_einf;
        system.drawMultiVector(STYPE::IPNS, vec, cf::Color::GREEN, 127);
    });
    return 0;
}
