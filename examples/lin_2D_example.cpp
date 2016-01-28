
#include "LSystem.h"


int main(int argc, char** argv) {

    if (argc < 2){
        std::cout << "Please provide a .lin file" << std::endl;
		getchar();
        return -1;
    }

    cf::LindenmayerSystem ls;   // alternative:     cf::LSystem ls;
    ls.read(argv[1]);

    std::string align = " :  ";
    std::cout << "Name"                    << align << ls.getName()                              << '\n'
              << "Axiom"                   << align << ls.getAxiom()                             << '\n'
              << "Number of productions"   << align << ls.getNumProductions()                    << '\n'
              << "Clear window each time?" << align << (ls.clearWindowEachTime() ? "yes" : "no") << '\n'
              << "Start angle"             << align << ls.getStartAngle()                        << "\n"
              << "Adjustment angle"        << align << ls.getAdjustmentAngel()                   << '\n'
              << "Scale"                   << align << ls.getScale()                             << std::endl;


    // most of the files provide F as a symbol
    char symbol = 'F';
    std::cout << "\nProductions to symbol: " << symbol << std::endl;

    const std::string* prod = ls.getProduction(symbol);

    if (prod)
        std::cout << *prod << std::endl;
    else
        std::cout << "File does not have production to: " << symbol << std::endl;

    // print all productions
    std::cout << std::endl << std::endl << "All productions:" << std::endl << std::endl;
    for (auto& e: ls.getAllProductions())
        std::cout << "Symbol: " << e.first << "\nProduction: " << e.second << std::endl << std::endl;

    getchar();
    return 0;
}
