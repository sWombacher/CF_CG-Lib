#include "ORB.h"

int main(int argc, char** argv) {
    // read provided *.orb file
    // if no file is provided as a commandline argument
    // use a default .orb file
    std::string filePath;
    if (argc < 2){
        std::cout << "Please provide a .orb file, if you want a differnt orb file\n\n\n";
        filePath = CHAOS_FILE_PATH;
        filePath += "Brezel_1.orb";
    }
    else
        filePath = argv[1];

    cf::Orbit orb; // alternative:    cf::ORB orb;
    orb.read(filePath);

    // print file data
    const std::string align = " :  ";
    std::cout << "Name"               << align << orb.getName()              << '\n'
              << "Num Factors"        << align << orb.getNumFactors()        << '\n'
              << "Num Startingpoints" << align << orb.getNumStartingPoints() << '\n'
              << "Interval X min"     << align << orb.getRangeX().min        << '\n'
              << "Interval X max"     << align << orb.getRangeX().max        << '\n'
              << "Interval Y min"     << align << orb.getRangeY().min        << '\n'
              << "Interval Y max"     << align << orb.getRangeY().max        << '\n'
              << "\n\n\n" << std::flush;

    std::cout << "Startingpoints:\n";
    for (const auto& e : orb.getAllStartingPoints())
        std::cout << e << std::endl;

    std::cout << "\n\nFactors:\n";
    for (const auto& e : orb.getAllFactors()){
        std::cout << e << std::endl;
    }

    std::cout << "Press enter to finish the process";
	cf::Console::waitKey();
    return 0;
}
