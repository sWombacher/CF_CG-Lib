#ifdef _WIN32
// enable exception handling for windows
// this requires 'int main(int, char**)' function definition
// therefore 'int main()' is dissabled
#define CFCG_EXCEPTION_HANDLING
#endif

#include "GEN.h"

int main(int argc, char** argv) {
    // receive file name/path
    std::string filePath;
    if (argc < 2) {
        std::cout << "Please provide a .gen file, if you want a different ifs file\n\n\n";
        filePath = CHAOS_FILE_PATH; // defined macro directing to <pathToLib>/ChaosAndFractal_Lib/chaos_files
//        filePath += "KOCH_INS.GEN";
        filePath += "BUSCH_04.GEN";
    } else
        filePath = argv[1];

    cf::GEN gen;
    gen.read(filePath);

    std::cout << "FileName: " << gen.getName() << '\n'
              << "RangeX: " << gen.getRangeX() << '\n'
              << "RangeY: " << gen.getRangeY() << '\n'
              << "ClearWindowEachTime: " << (gen.clearWindowEachTime() ? "yes" : "no") << '\n';

    std::cout << "\nInitiatorPoints:\n";
    for (const auto& e : gen.getInitiatorPoints())
        std::cout << e << '\n';

    std::cout << "\nGeneratorPoints:\n";
    for (const auto& e : gen.getGeneratorPoints())
        std::cout << e << '\n';

    std::cout << std::endl;
    return 0;
}
