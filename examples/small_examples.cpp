#ifdef _WIN32
// enable exception handling for windows
// this requires 'int main(int, char**)' function definition
// therefore 'int main()' is dissabled
#define CFCG_EXCEPTION_HANDLING
#endif

#include "BigIntegerLibrary.hh"

#include "utils.h"
#include <iomanip> // set precision

int main() {
    // read ant string and print it
    const std::string antStr = cf::readAntString(CHAOS_FILE_PATH "Ant_10.ant");
    std::cout << "Ant string:\t" << antStr << '\n' << std::endl;

    // iterate through some colors
    // option 1:
    std::cout << "Iterate throgh colors, option 1:\n";
    const auto colors = {cf::Color::GREEN, cf::Color::RandomColor()};
    for (const auto& e : colors)
        std::cout << e << "\t\t";
    std::cout << "\n\n";

    // option 2:
    std::cout << "Iterate throgh colors, option 2:\n";

    // use as many colors as you like ;)
    auto iter = cf::Color::CreateEndlessColorIterator(cf::Color::BLACK, cf::Color::RandomColor());
    for (int i = 0; i < 3; ++i) {
        std::cout << *iter << '\n';
        ++iter;
    }
    std::cout << std::endl;

    // read pal file and print a color
    std::vector<cf::Color> pal = cf::readPaletteFromFile(CHAOS_FILE_PATH "Chaos_ant.pal");
    std::cout << "Palette at index 4:\t\t" << pal[4] << std::endl;
    std::cout << "Palette at index 4 inverted:\t" << pal[4].invert() << std::endl << std::endl;

    // read dat file and print a vector
    std::vector<glm::vec3> dat = cf::readDATFile(CHAOS_FILE_PATH "Life1.dat");
    std::cout << "Dat file at index 0:\t" << dat[0] << std::endl << std::endl;

    // calculation with very very large ints :)
    BigInteger veryBigNumber = stringToBigInteger("999999999999999999999999999999999");
    veryBigNumber *= veryBigNumber;
    veryBigNumber /= 10;
    veryBigNumber += BigInteger(111111111111111111) + 1;
    std::cout << "Very big number calculation result:\n" << bigIntegerToString(veryBigNumber) << std::endl << std::endl;

    // convert between degrees/radians
    // Note: OpenGL uses degree, glm radians (see 'window_3D_example.cpp')
    const float deg = cf::degree2radian(180.f);
    const float rad = cf::radian2degree(glm::pi<float>() / 2.f);
    std::cout << "180 degree in radian:  " << deg << std::endl << " Pi half   in degree:  " << rad << std::endl << std::endl;

    // print floating point values with different precisions
    // based on: http://www.cplusplus.com/reference/iomanip/setprecision/
    const double pi_approx = 3.14159;
    std::cout << "Different pi approximation outputs:\n"
              << std::setprecision(5) << pi_approx << "\t\t" << std::setprecision(9) << pi_approx << "\t\t" << std::fixed
              << std::setprecision(5) << pi_approx << "\t\t" << std::setprecision(9) << pi_approx << std::endl
              << std::endl;

    // Direction example
    cf::Direction::AbsoluteDirection west = cf::Direction::AbsoluteDirection::WEST;
    std::cout << "If your current direction is 'WEST'\nand you take a right turn your new direction will be:\t'"
              << cf::Direction::toString(cf::Direction::getNextiDirection(west, cf::Direction::RelativeDirection::RIGHT))
              << "'\n"
              << std::endl;

    // change console color
    std::cout << termcolor::blue << "blue " << termcolor::bold << "bold " << termcolor::on_cyan << "onGrey "
              << termcolor::reverse << "reverse " << termcolor::reset << std::endl;

    // program end
    std::cout << "Press enter to end the process" << std::flush;
    cf::Console::waitKey();
    return 0;
}
