#include "IFS.h"

#include <stdexcept>
#include <fstream>
#include <sstream>



namespace cf{

void IteratedFunctionSystem::read(const char* fiilename){
    std::fstream input(fiilename, std::fstream::in);
    std::string str;

    std::getline(input, str); // name
    {
        removeWindowsSpecificCarriageReturn(str);
        std::size_t nameStartposition = str.rfind('/');
        if (nameStartposition != std::string::npos)
            this->m_Name = str.substr(nameStartposition + 1);
    }

    std::getline(input, str); // num transformations
    removeWindowsSpecificCarriageReturn(str);

    int num_transformations = std::stoi(str);
    for (int i = 0; i < num_transformations && std::getline(input, str); ++i) {
        removeWindowsSpecificCarriageReturn(str);
        std::stringstream sstr;
        sstr << str;
        std::vector<double> transformation;

        while (std::getline(sstr, str, ' ')) {
            removeWindowsSpecificCarriageReturn(str);
            if (str.size()) // two spaces in a row
                transformation.push_back(std::stod(str));
        }

        if (transformation.size() != 6)
            throw "Error in file format (transformation), size: " + std::to_string(transformation.size());

        glm::mat3x3 mat;
        mat[0][0] = transformation[0];
        mat[1][0] = transformation[1];
        mat[2][0] = transformation[4];

        mat[0][1] = transformation[2];
        mat[1][1] = transformation[3];
        mat[2][1] = transformation[5];

        mat[0][2] = 0;
        mat[1][2] = 0;
        mat[2][2] = 1;

        this->m_Transformations.push_back(mat);
    }

    // read bounds
    std::getline(input, str);
    removeWindowsSpecificCarriageReturn(str);
    std::stringstream sstr;
    sstr << str;

    std::vector<float> values;
    while (std::getline(sstr, str, ' ')) {
        removeWindowsSpecificCarriageReturn(str);
        if (str.size()) // two spaces in a row
            values.push_back(std::stof(str));
    }

    if (values.size() != 4)
        throw "Error in file format (bonds): wrong number of elements";

    this->m_RangeX.min = values[0];
    this->m_RangeX.max = values[1];

    this->m_RangeY.min = values[2];
    this->m_RangeY.max = values[3];

    if (this->m_RangeX.min >= this->m_RangeX.max || this->m_RangeY.min >= this->m_RangeY.max)
        throw "Error in file format (bonds), bound values have to be in ascending order";

    input.close();
}

std::size_t IteratedFunctionSystem::getNumTransformations() const{
    return this->m_Transformations.size();
}

const glm::mat3x3& IteratedFunctionSystem::getTransformation(std::size_t pos) const{
    if (pos >= this->m_Transformations.size())
        throw std::out_of_range("out of bound exception, in function \"IteratedFunctionSystem::getTransformation\"");

    return this->m_Transformations[pos];
}

const Intervall& IteratedFunctionSystem::getRangeX() const{
    return this->m_RangeX;
}
const Intervall& IteratedFunctionSystem::getRangeY() const{
    return this->m_RangeY;
}

const std::string& IteratedFunctionSystem::getName() const{
    return this->m_Name;
}

const std::vector<glm::mat3x3> &IteratedFunctionSystem::getAllTransformation() const{
    return this->m_Transformations;
}
}
