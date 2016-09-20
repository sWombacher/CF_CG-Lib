#include "LSystem.h"

int main(int argc, char** argv) {
    // receive file name/path
    std::string filePath;
    if (argc < 2){
        std::cout << "Please provide a .lin file, if you want a different file\n\n\n";
        filePath = CHAOS_FILE_PATH; // defined macro directing to <pathToLib>/ChaosAndFractal_Lib/chaos_files
        filePath += "Busch_1.lin";
    }
    else
        filePath = argv[1];

    // create and parse *.lin file
    cf::LindenmayerSystem ls;   // alternative:     cf::LSystem ls;
    ls.read(filePath);

    // print all data found in *.lin file
    std::string align = " :  ";
    std::cout << "Name"                    << align << ls.getName()                              << '\n'
              << "Axiom"                   << align << ls.getAxiom()                             << '\n'
              << "Number of productions"   << align << ls.getNumProductions()                    << '\n'
              << "Clear window each time?" << align << (ls.clearWindowEachTime() ? "yes" : "no") << '\n'
              << "Start angle"             << align << ls.getStartAngle()                        << '\n'
              << "Adjustment angle"        << align << ls.getAdjustmentAngel()                   << '\n'
              << "Scale"                   << align << ls.getScale()                             << '\n'
              << "Interval X"              << align << ls.getRangeX()                            << '\n'
              << "Interval Y"              << align << ls.getRangeY()                            << '\n'
              << std::endl;

    // most of the files provide F as a symbol
    char symbol = 'F';
    std::cout << "\nProductions to symbol: " << symbol << std::endl;

    // display production
    // NOTE:
    //  not every character has to be a production
    //  in this case NULL will be returned
    const std::string* prod = ls.getProduction(symbol);
    if (prod)
        std::cout << *prod << std::endl;
    else
        std::cout << "File does not have production to: " << symbol << std::endl;

    // print all productions
    std::cout << std::endl << std::endl << "All productions:" << std::endl << std::endl;
    for (const auto& e: ls.getAllProductions()){
        std::cout << "Symbol: " << e.first << "\nProduction: " << e.second << std::endl << std::endl;
    }

    std::cout << "Press enter to finish the process";
	cf::Console::waitKey();
    return 0;
}
