


#include "LSystem.h"


int main(int argc, char** argv) {

    if (argc < 2){
        std::cout << "Please provide a .ifs file" << std::endl;
        return -1;
    }

    cf::LindenmayerSystem ls;   // alternative:     cf::LSystem ls;

    ls.read(argv[1]);

    std::cout << "Name : " << ls.getName()  << '\n'
              << "Axiom: " << ls.getAxiom() << '\n'
              << "Number of productions: " << ls.getNumProductions() << '\n'
              << "Clear window each time?: " << (ls.clearWindowEachTime() ? "yes" : "no") << '\n'
              << "Start angle: " << ls.getStartAngle() << '\n'
              << "Adjustment angle: " << ls.getAdjustmentAngel() << '\n'
              << "Scale: " << ls.getScale() << std::endl;


    // most of the files provide F as a symbol
    char symbol = 'F';
    std::cout << "\nProductions to symbol: " << symbol << std::endl;

    const std::string* prod = ls.getProduction(symbol);

    if (prod)
        std::cout << *prod << std::endl;
    else
        std::cout << "File does not have production to: " << symbol << std::endl;

    getchar();
    return 0;
}
