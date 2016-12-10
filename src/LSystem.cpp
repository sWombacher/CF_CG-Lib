

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
    if (str.size() > 1){
        this->m_Axiom = 'A';
        this->m_Productions.emplace('A', str);
    }
    else
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
        this->m_Productions.emplace(symbol, str);
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
    auto found = this->m_Productions.find(symbol);
    if (found != this->m_Productions.end())
        return &found->second;
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
float LindenmayerSystem::getAdjustmentAngle() const{
    return this->m_AdjustmentAngle;
}

const std::map<char, const std::string>& LindenmayerSystem::getAllProductions() const{
    return this->m_Productions;
}

std::ostream& operator<<(std::ostream& os, const Interval& interval){
    os << '[' << interval.min << ", " << interval.max << ']';
    return os;
}

LSystem_Controller::LSystem_Controller(size_t depth, const LSystem &LSystem) : m_Depth(depth), m_LSystem(LSystem){}

LSystem_Controller::iterator LSystem_Controller::begin() { return iterator(this->m_LSystem, this->m_Depth, false); }

LSystem_Controller::iterator LSystem_Controller::end() { return iterator(this->m_LSystem, this->m_Depth, true ); }

const char &LSystem_Controller::iterator::operator*(){
    return this->m_LSystem.getProduction(this->m_CurrentProduction)->at(this->m_Positions.at(this->m_CurrentDepth).first);
}

LSystem_Controller::iterator &LSystem_Controller::iterator::operator++(){
    if (this->m_EndReached)
        throw std::runtime_error("Bound exception");

    while(true){
        const std::string& cur_production = *this->m_LSystem.getProduction(this->m_CurrentProduction);
        auto& cur_pos = this->m_Positions[this->m_CurrentDepth];
        ++cur_pos.first;

        if (size_t(cur_pos.first) >= cur_production.size()){
            if (this->m_CurrentDepth == 0){
                this->m_EndReached = true;
                return *this;
            }

            // go up
            --this->m_CurrentDepth;
            this->m_CurrentProduction = this->m_Positions[this->m_CurrentDepth].second;
        }
        else{
            // check for terminal symbol
            char curSymbol = cur_production[cur_pos.first];
            if (this->m_LSystem.getProduction(curSymbol) == nullptr)
                return *this;

            // go down
            if (this->m_CurrentDepth >= this->m_Positions.size() - 1)
                return *this;

            cur_pos.second = this->m_CurrentProduction;
            ++this->m_CurrentDepth;
            this->m_Positions[this->m_CurrentDepth].first = -1;
            this->m_CurrentProduction = curSymbol;
        }
    }
}

bool LSystem_Controller::iterator::operator!=(const LSystem_Controller::iterator &rhs){
    if (this->m_EndReached && rhs.m_EndReached)
        return false;
    return this->m_Positions != rhs.m_Positions;
}

LSystem_Controller::iterator::iterator(const LSystem &lsystem, size_t depth, bool endIterator): m_Positions(depth, {endIterator ? (1 << 31) : -1, '\0'}), m_LSystem(lsystem), m_EndReached(endIterator) {
    this->m_CurrentProduction = lsystem.getAxiom();
    if (!endIterator)
        this->operator ++();
}

}

