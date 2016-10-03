

#include "LSystem.h"

#include <fstream>
#include <sstream>





namespace cf{

void LindenmayerSystem::read(const std::string& filePath){
    std::fstream input(filePath, std::fstream::in);
    if (!input)
        throw std::runtime_error("File not found in function: \"LindenmayerSystem::read\"");

    std::string str;

    // read filename
    std::getline(input, str);
    _removeWindowsSpecificCarriageReturn(str);
    this->m_Name = str.substr(str.rfind(' ') + 1);

    // read axiom
    std::getline(input, str);
    _removeWindowsSpecificCarriageReturn(str);
    this->m_Axiom = str[0];

    // read num productions
    std::getline(input, str);
    _removeWindowsSpecificCarriageReturn(str);
    int productionCounter = std::stoi(str);

    for (int i = 0; i < productionCounter; ++i){
        std::getline(input, str);
        _removeWindowsSpecificCarriageReturn(str);
        char symbol = str[0];
        str = str.substr(str.find('>') + 1);
        //str = str.substr(0, str.size() -1);

        this->m_Productions.push_back(std::make_pair(symbol, str));
    }

    std::getline(input, str);
    _removeWindowsSpecificCarriageReturn(str);
    std::stringstream sstream;
    sstream << str;

    std::getline(sstream, str, ' ');
    this->m_StartAngle = std::stof(str);


    auto readNext = [&str](std::stringstream& stream){
        do{
            std::getline(stream, str, ' ');
            _removeWindowsSpecificCarriageReturn(str);
        } while (!str.size());
    };

    readNext(sstream);
    this->m_AdjustmentAngle = std::stof(str);

    readNext(sstream);
    this->m_Scale = std::stof(str);

    std::getline(input, str);
    _removeWindowsSpecificCarriageReturn(str);

    sstream.clear();
    sstream << str;

    readNext(sstream);
    this->m_RangeX.min = std::stof(str);

    readNext(sstream);
    this->m_RangeX.max = std::stof(str);

    readNext(sstream);
    this->m_RangeY.min = std::stof(str);

    readNext(sstream);
    this->m_RangeY.max = std::stof(str);

    std::getline(input, str);
    _removeWindowsSpecificCarriageReturn(str);

    if (str[str.size() - 1] == '0')
        this->m_ClearWindowEachTime = false;
    else if (str[str.size() - 1] == '1')
        this->m_ClearWindowEachTime = true;
    else
        throw std::runtime_error("no flag (not) to clear window");
}

const std::string& LindenmayerSystem::getName () const{
    return this->m_Name;
}
char LindenmayerSystem::getAxiom() const{
    return this->m_Axiom;
}

const std::string* LindenmayerSystem::getProduction(char symbol) const{
    for (std::size_t i = 0; i < this->m_Productions.size(); ++i){
        if (this->m_Productions[i].first == symbol)
            return &this->m_Productions[i].second;
    }
    return nullptr;
}

std::size_t LindenmayerSystem::getNumProductions() const{
    return this->m_Productions.size();
}

bool LindenmayerSystem::clearWindowEachTime() const{
    return this->m_ClearWindowEachTime;
}

const Interval& LindenmayerSystem::getRangeX() const{
    return this->m_RangeX;
}
const Interval& LindenmayerSystem::getRangeY() const{
    return this->m_RangeY;
}


float LindenmayerSystem::getScale() const{
    return this->m_Scale;
}
float LindenmayerSystem::getStartAngle() const{
    return this->m_StartAngle;
}
float LindenmayerSystem::getAdjustmentAngel() const{
    return this->m_AdjustmentAngle;
}

const std::vector<std::pair<const char, const std::string> > &LindenmayerSystem::getAllProductions() const{
    return this->m_Productions;
}

std::ostream& operator<<(std::ostream& os, const Interval& interval){
    os << '[' << interval.min << ", " << interval.max << ']';
    return os;
}

}

