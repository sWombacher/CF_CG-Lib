#include "LSystem.h"

#include <fstream>
#include <sstream>

namespace cf {

void LindenmayerSystem::read(const std::string& filePath) {
    std::fstream input(filePath, std::fstream::in);
    if (!input) {
        const auto error = R"(File not found in function: "LindenmayerSystem::read")";
        std::cout << error << std::endl;
        throw std::runtime_error(error);
    }
    std::string str;

    // read filename
    std::getline(input, str);
    _removeWindowsSpecificCarriageReturn(str);
    this->m_Name = str.substr(str.rfind(' ') + 1);

    // read axiom
    std::getline(input, this->m_Axiom);
    _removeWindowsSpecificCarriageReturn(this->m_Axiom);

    // read num productions
    std::getline(input, str);
    _removeWindowsSpecificCarriageReturn(str);
    int productionCounter = std::stoi(str);

    for (int i = 0; i < productionCounter; ++i) {
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

    auto readNext = [&str](std::stringstream& stream) {
        do {
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

const std::string& LindenmayerSystem::getName() const { return this->m_Name; }
const std::string& LindenmayerSystem::getAxiom() const { return this->m_Axiom; }

const std::string* LindenmayerSystem::getProduction(char symbol) const {
    auto found = this->m_Productions.find(symbol);
    if (found != this->m_Productions.end())
        return &found->second;
    return nullptr;
}

std::size_t LindenmayerSystem::getNumProductions() const { return this->m_Productions.size(); }

bool LindenmayerSystem::clearWindowEachTime() const { return this->m_ClearWindowEachTime; }

const Interval& LindenmayerSystem::getRangeX() const { return this->m_RangeX; }
const Interval& LindenmayerSystem::getRangeY() const { return this->m_RangeY; }

float LindenmayerSystem::getScale() const { return this->m_Scale; }
float LindenmayerSystem::getStartAngle() const { return this->m_StartAngle; }
float LindenmayerSystem::getAdjustmentAngle() const { return this->m_AdjustmentAngle; }

const std::map<char, const std::string>& LindenmayerSystem::getAllProductions() const { return this->m_Productions; }

std::ostream& operator<<(std::ostream& os, const Interval& interval) {
    os << '[' << interval.min << ", " << interval.max << ']';
    return os;
}

LSystem_Controller::LSystem_Controller(size_t depth, const LSystem& LSystem)
    : m_Depth(depth), m_Productions(LSystem.getAllProductions()) {
    const auto& axiom = LSystem.getAxiom();
    if (axiom.empty())
        throw std::runtime_error("Error: No Axiom");
    else if (axiom.size() == 1)
        this->m_Axiom = axiom.front();
    else {
        // add axiom to productions and add a simple symbol for it
        char symbol = 'A';
        while (this->m_Productions.find(symbol) != this->m_Productions.end())
            ++symbol;

        // symbol may be > 'Z', but could colide with other symbols of a production
        if (symbol > 'Z')
            throw std::runtime_error("Error: Unable to find suitable symbol");

        this->m_Axiom = symbol;
        this->m_Productions.emplace(symbol, LSystem.getAxiom());
    }
}

LSystem_Controller::iterator LSystem_Controller::begin() const {
    return iterator(this->m_Axiom, this->m_Productions, this->m_Depth, false);
}

LSystem_Controller::iterator LSystem_Controller::end() const {
    return iterator(this->m_Axiom, this->m_Productions, this->m_Depth, true);
}

std::string LSystem_Controller::getCompleteString() const {
    std::string result;
    for (const auto& e : *this)
        result += e;

    return result;
}

const char& LSystem_Controller::iterator::operator*() {
    return this->m_Productions->at(this->m_CurrentProduction).at(size_t(this->m_Positions.at(this->m_CurrentDepth).first));
}

LSystem_Controller::iterator& LSystem_Controller::iterator::operator++() {
    if (this->m_EndReached)
        throw std::runtime_error("Bound exception");

    while (true) {
        const std::string& cur_production = this->m_Productions->at(this->m_CurrentProduction);
        auto& cur_pos = this->m_Positions[this->m_CurrentDepth];
        ++cur_pos.first;

        if (size_t(cur_pos.first) >= cur_production.size()) {
            if (this->m_CurrentDepth == 0) {
                this->m_EndReached = true;
                return *this;
            }

            // go up
            --this->m_CurrentDepth;
            this->m_CurrentProduction = this->m_Positions[this->m_CurrentDepth].second;
        } else {
            // check for terminal symbol
            char curSymbol = cur_production[size_t(cur_pos.first)];
            if (this->m_Productions->find(curSymbol) == this->m_Productions->end())
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

bool LSystem_Controller::iterator::operator!=(const LSystem_Controller::iterator& rhs) {
    if (this->m_EndReached && rhs.m_EndReached)
        return false;
    return this->m_Positions != rhs.m_Positions;
}

LSystem_Controller::iterator::iterator(char axiom, const ProductionMap& lsystem, size_t depth, bool endIterator)
    : m_Positions(depth, {endIterator ? (1 << 31) : -1, '\0'}), m_Productions(&lsystem), m_CurrentProduction(axiom),
      m_EndReached(endIterator) {
    if (!endIterator)
        this->operator++();
}
} // namespace cf
