

#include "LSystem.h"

#include <fstream>
#include <sstream>





namespace cf{

void LindenmayerSystem::read(const char* filename){
    std::fstream input(filename, std::fstream::in);
    std::string str;

    // read filename
    std::getline(input, str);
    this->m_Name = str.substr(str.rfind(' '));

    // read axiom
    std::getline(input, this->m_Axiom);

    // read num productions
    std::getline(input, str);
    int productionCounter = std::stoi(str);

    for (int i = 0; i < productionCounter; ++i){
        std::getline(input, str);
        char symbol = str[0];
        str = str.substr(str.find('>') + 1);
        //str = str.substr(0, str.size() -1);

        /// TODO line endings

        this->m_Productions.push_back(std::make_pair(symbol, str));
    }

    std::getline(input, str);
    std::stringstream sstream;
    sstream << str;

    std::getline(sstream, str, ' ');
    this->m_StartAngle = std::stof(str);


    auto readNext = [&str](auto& stream){
        do{
            std::getline(stream, str, ' ');
        } while (!str.size());
    };

    readNext(sstream);
    this->m_AdjustmentAngle = std::stof(str);

    readNext(sstream);
    this->m_Scale = std::stof(str);

    std::getline(input, str);
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
    if (str[str.size() - 2] == '0')
        this->m_ClearWindowEachTime = false;
    else if (str[str.size() - 2] == '1')
        this->m_ClearWindowEachTime = true;
    else
        throw std::runtime_error("no flag (not) to clear window");
}

const std::string& LindenmayerSystem::getName () const{
    return this->m_Name;
}
const std::string& LindenmayerSystem::getAxiom() const{
    return this->m_Axiom;
}

const std::string* LindenmayerSystem::getProduction(char symbol) const{
    for (int i = 0; i < this->m_Productions.size(); ++i){
        if (this->m_Productions[i].first == symbol)
            return &this->m_Productions[i].second;
    }
    return NULL;
}

std::size_t LindenmayerSystem::getNumProductions() const{
    return this->m_Productions.size();
}

bool LindenmayerSystem::clearWindowEachTime() const{
    return this->m_ClearWindowEachTime;
}

const Intervall& LindenmayerSystem::getRangeX() const{
    return this->m_RangeX;
}
const Intervall& LindenmayerSystem::getRangeY() const{
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



}

