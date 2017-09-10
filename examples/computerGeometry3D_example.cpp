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
    cf::fMultiVector vec_f = vec;

    const auto v = 12.50_E + 123456.0_value;
    vec += v;
    vec += vec_f - 12.0_e0;
    std::cout << v << '\n' << vec_f << '\n' << vec << std::endl;

	const auto res = 1.0_e1 * (1.0_e1 ^ 1.0_e2);//  *1.0_e2;
	std::cout << "Result: " << res << std::endl; 
	getchar(); 
    return 0;

    cf::WindowCoordinateSystem3D system(&argc, argv);
    system.beginDrawing([&]() {
        // system.drawPlane({1, 1, 1, 0}, cf::Color::RED, 127);
        // system.drawPlane({1, 1, 1}, {0, 1.5f, 0}, cf::Color::RED, 127);
        //system.drawLine({0, 0, 0}, {1, 1, 0});
        // system.drawPoint({0,0,0});
        //system.drawCircle({0, 0, 0}, {1, 1, 0}, 1.f, cf::Color::ORANGE);
        // system.drawSphere({1, 1, 1}, 0.5f);

        vec = 1.0_e0 + 1.0_einf;
        vec = 1.0_e2;
        system.drawMultiVector(STYPE::IPNS, vec, cf::Color::GREEN, 127);
    });
    return 0;
}
