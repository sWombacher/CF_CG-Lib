#include "ORB.h"

int main(int argc, char** argv) {
    if (argc < 2){
        std::cout << "Please provide a .orb file" << std::endl;
        return -1;
    }
    cf::Orbit orb; // alternative:    cf::ORB orb;
    orb.read(argv[1]);

    const std::string align = "\r\t\t\t: ";
    std::cout << "Name"               << align << orb.getName()              << '\n'
              << "Num Factors"        << align << orb.getNumFactors()        << '\n'
              << "Num Startingpoints" << align << orb.getNumStartingPoints() << '\n'
              << "Intervall X min"    << align << orb.getRangeX().min        << '\n'
              << "Intervall X max"    << align << orb.getRangeX().max        << '\n'
              << "Intervall Y min"    << align << orb.getRangeY().min        << '\n'
              << "Intervall Y max"    << align << orb.getRangeY().max        << '\n'
              << "\n\n" << std::endl;

    std::cout << "Startinpoints:\n";
    for (const auto& e : orb.getAllStartingPoints())
        std::cout << e << std::endl;

    std::cout << "\n\nFactors:\n";
    for (const auto& e : orb.getAllFactors())
        std::cout << e << std::endl;

    getchar();
    return 0;
}
