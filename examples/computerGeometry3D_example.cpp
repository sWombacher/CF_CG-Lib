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
        auto drawNext = [&](const char* str){
            std::cout << "Press enter to continue to: " << str << std::endl;
            system.waitKey();
            system.clearWindow();
        };

// IPNS
        const auto s0 = -1.0_e1 - 0.22_einf + 1.0_e0;
        const auto s1 = 1.0_e2 - 0.22_einf + 1.0_e0;
        const auto s2 = -0.72_einf + 1.0_e0;

        drawNext("Points");

        drawNext("Spheres");
        system.drawMultiVector(STYPE::IPNS, s0, cf::Color::RED  , 255);
        system.drawMultiVector(STYPE::IPNS, s1, cf::Color::GREEN, 255);
        system.drawMultiVector(STYPE::IPNS, s2, cf::Color::BLUE , 255);

        drawNext("Circle");
//        system.drawMultiVector(STYPE::IPNS, s0 % s1, cf::Color::BLUE, 255);
        const auto circle = s0 % s1;
        std::cout << "Circle:\n" << circle << std::endl;
        system.drawMultiVector(STYPE::OPNS, circle, cf::Color::BLUE, 255);

        drawNext("Point pair");
        system.drawMultiVector(STYPE::IPNS, s0 % s1 % s2, cf::Color::BLACK, 255);

// OPNS

    });
    return 0;
}
