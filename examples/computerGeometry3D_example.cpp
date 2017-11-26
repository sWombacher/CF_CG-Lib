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

    cf::WindowCoordinateSystem3D system(&argc, argv);
    system.beginDrawing([&]() {
        /*
        system.drawMultiVector(STYPE::IPNS, 1.0_e0 + 1.0_einf, cf::Color::GREEN, 127);

        system.drawMultiVector(STYPE::IPNS, 1.0_e1, cf::Color::RED, 127);
        system.drawMultiVector(STYPE::IPNS, 1.0_e2, cf::Color::BLUE, 127);
        system.drawMultiVector(STYPE::IPNS, 1.0_e3, cf::Color::GREEN, 127);
        */

        const auto s0 = -1.0_e1 - 0.22_einf + 1.0_e0;
        const auto s1 = -0.72_einf + 1.0_e0;
        const auto s2 = 1.0_e2 - 0.22_einf + 1.0_e0;
//        system.drawMultiVector(STYPE::IPNS, s0, cf::Color::RED, 127);
//        system.drawMultiVector(STYPE::IPNS, s1, cf::Color::GREEN, 127);
//        system.drawMultiVector(STYPE::IPNS, s2, cf::Color::BLUE, 127);

        system.drawMultiVector(STYPE::IPNS, s0 % s1 % s2, cf::Color::BLACK, 255);

    });
    return 0;
}
