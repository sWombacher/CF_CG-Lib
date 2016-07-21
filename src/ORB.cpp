
#include "ORB.h"
#include <fstream>
#include <sstream>

namespace cf{

void Orbit::read(const std::string &filename){
    std::fstream input(filename, std::fstream::in);
    if (!input)
        throw std::runtime_error("File not found in function: \"Orbit::read\"");

    std::string str;
    std::getline(input, str);
    _removeWindowsSpecificCarriageReturn(str);
    this->m_Name = str.substr(str.rfind(' ') + 1);

    // remove white spaces
    // including windows specific carriage return
    auto removeWhiteSpace = [](const std::string& str) -> std::string{
        std::string toReturn;
        for (const auto& e : str){
            if (e != ' ' && e != '\t' && e != '\n' && e != '\r'){
                toReturn.reserve(1);
                toReturn.push_back(e);
            }
        }
        return toReturn;
    };

    std::getline(input, str);
    str = removeWhiteSpace(str);
    int factorCount = std::stoi(str);
    for (int i = 0; i < factorCount; ++i){
        std::getline(input, str);
        str = removeWhiteSpace(str);
        this->m_Factors.push_back(std::stof(str));
    }

    std::stringstream sstr;
    std::getline(input, str);
    str = removeWhiteSpace(str);
    int startPointCount = std::stoi(str);
    for (int i = 0; i < startPointCount; ++i){
        std::getline(input, str);
        _removeWindowsSpecificCarriageReturn(str);
        sstr.clear();
        sstr << str;

        std::vector<float> values;
        for (std::string s; std::getline(sstr, s, ' ');){
            if (!s.size())
                continue;

            values.push_back(std::stof(s));
        }
        if (values.size() != 3)
            throw std::runtime_error("Wrong format in Orb-File (starting point format)");

        this->m_StartPoints.push_back(glm::vec3(values[0], values[1], values[2]));
    }


    std::getline(input, str);
    _removeWindowsSpecificCarriageReturn(str);
    sstr.clear();
    sstr << str;
    std::vector<float> values;
    for (std::string s; std::getline(sstr, s, ' ');){
        if (!s.size())
            continue;

        values.push_back(std::stof(s));
    }
    if (values.size() != 4)
        throw std::runtime_error("Wrong format in Orb-File (intervall format)");

    this->m_IntervallX.min = values[0];
    this->m_IntervallX.max = values[1];

    this->m_IntervallY.min = values[2];
    this->m_IntervallY.max = values[3];
}

const Intervall& Orbit::getRangeX() const{
    return this->m_IntervallX;
}
const Intervall& Orbit::getRangeY() const{
    return this->m_IntervallY;
}

const std::string &Orbit::getName() const{
    return this->m_Name;
}

const std::vector<glm::vec3> &Orbit::getAllStartingPoints() const{
    return this->m_StartPoints;
}
const std::vector<float> &Orbit::getAllFactors() const{
    return this->m_Factors;
}

std::size_t Orbit::getNumFactors() const{
    return this->m_Factors.size();
}
std::size_t cf::Orbit::getNumStartingPoints() const{
    return this->m_StartPoints.size();
}


}
