
#include "IFS.h"

int main(int argc, char** argv) {
    // receive file name/path
    std::string filePath;
    if (argc < 2){
        std::cout << "Please provide a .ifs file, if you want a different ifs file\n\n\n";
        filePath = CHAOS_FILE_PATH; // defined macro directing to <pathToLib>/ChaosAndFractal_Lib/chaos_files
        filePath += "Baum_3.ifs";
    }
    else
        filePath = argv[1];

    // create IFS object
    // and read *.ifs file
    cf::IteratedFunctionSystem ifs; // alternative:    cf::IFS ifs;
    ifs.read(filePath);

    // output all parsed data of IFS file
    std::string align = " :  ";
    std::cout << "Name"               << align << ifs.getName()               << '\n'
              << "Num transformation" << align << ifs.getNumTransformations() << '\n'
              << "Interval X min"     << align << ifs.getRangeX().min         << '\n'
              << "Interval X max"     << align << ifs.getRangeX().max         << '\n'
              << "Interval Y min"     << align << ifs.getRangeY().min         << '\n'
              << "Interval Y max"     << align << ifs.getRangeY().max         << '\n'
              << "\n\n" << std::endl;

    for (std::size_t i = 0; i < ifs.getNumTransformations(); ++i)
         std::cout << "Transformation " << i << ": \n" << ifs.getTransformation(i) << std::endl;


    // calculate a simple matrix multiplication
    std::cout << "\nTransformation example:" << std::endl;

    const glm::mat3x3& m = ifs.getTransformation(0);
    glm::vec3 v(1, 0, 1);
    std::cout << "Vector before transformation: " << v << std::endl;

    // print simple transformation
    v = m * v;
    std::cout << "Vector after  transformation: " << v << std::endl << std::endl;

    std::cout << "Press enter to finish the process";
    cf::Console::waitKey();
    return 0;
}
