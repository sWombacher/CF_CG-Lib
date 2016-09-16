#include "utils.h"
#include "InfInt.h"
#include <iomanip> // set precision

int main(){
    // read ant string and print it
    std::string antStr = cf::readAntString(CHAOS_FILE_PATH "Ant_10.ant");
    std::cout << "Ant string:\n" << antStr << std::endl << std::endl << std::endl;


    // calculate with very very large ints :)
    InfInt veryBigNumber = "999999999999999999999999999999999";
    veryBigNumber *= veryBigNumber;
    veryBigNumber /= 10;
    veryBigNumber += InfInt(111111111111111111) + 1;
    std::cout << "Very big number calculation result:\n" << veryBigNumber << std::endl << std::endl << std::endl;


    // read pal file & print a color
    std::vector<cf::Color> pal = cf::readPaletteFromFile(CHAOS_FILE_PATH "Chaos_ant.pal");
    std::cout << "Pallete at index 42:\n" << pal[42] << std::endl << std::endl;
    std::cout << "Pallete at index 42 inverted:\n" << pal[42].invert() << std::endl << std::endl << std::endl;


    // convert between degrees/radiants
    // Note: OpenGL uses degree, glm radians (see 'window_3D_example.cpp')
    float deg = cf::degree2radian(180.f);
    float rad = cf::radian2degree(glm::pi<float>() / 2.f);
    std::cout << "180 degree in radiant:  " << deg << std::endl
              << " Pi half   in  degree:  " << rad << std::endl
              << std::endl << std::endl;


    // print floating point values with differnt precisions
    // based on: http://www.cplusplus.com/reference/iomanip/setprecision/
    double pi_approx = 3.14159;
    std::cout << std::setprecision(5) << pi_approx << std::endl
              << std::setprecision(9) << pi_approx << std::endl
              << std::fixed << std::setprecision(5) << pi_approx << std::endl
              << std::setprecision(9) << pi_approx << std::endl
              << std::endl
              << std::endl;


    // Direction example
    cf::Direction::AbsoluteDirection west = cf::Direction::AbsoluteDirection::WEST;
    std::cout << "If you current direction is 'WEST'\nAnd you take a right turn your new direction will be:\n'"
              << cf::Direction::toString(cf::Direction::getNextiDirection(west, cf::Direction::RelativeDirection::RIGHT)) << "'\n"
              << std::endl
              << std::endl;


    // program end
    std::cout << "Press enter to end the process" << std::endl;
    cf::Console::waitKey();
    return 0;
}
