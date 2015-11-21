
#include "IFS.h"


int main(int argc, char** argv) {

    if (argc < 2){
        std::cout << "Please provide a .ifs file" << std::endl;
        return -1;
    }

    cf::IteratedFunctionSystem ifs; // alternative:    cf::IFS ifs;
    ifs.read(argv[1]);


    std::cout << "Name: "               << ifs.getName() << '\n'
              << "Num transformation: " << ifs.getNumTransformations() << '\n'
              << "Intervall X min: "    << ifs.getRangeX().min << '\n'
              << "Intervall X max: "    << ifs.getRangeX().max << '\n'
              << "Intervall Y min: "    << ifs.getRangeY().min << '\n'
              << "Intervall Y max: "    << ifs.getRangeY().max << '\n'
              << "\n\n" << std::endl;

    for (int i = 0; i < ifs.getNumTransformations(); ++i)
         std::cout << "Transformation " << i << ": \n" << ifs.getTransformation(i) << std::endl;

    std::cout << "\nTransformation example:" << std::endl;


    const glm::mat3x3& m = ifs.getTransformation(0);

    glm::vec3 v(1, 0, 1);
    std::cout << "Vector before transformation: " << v << std::endl;

    v = m * v;
    std::cout << "Vector after  transformation: " << v << std::endl << std::endl;


    getchar();
    return 0;
}
