#include "GEN.h"

namespace cf {

void GEN::read(const std::string& filePath) {
    std::fstream file(filePath);
    if (!file)
        throw std::runtime_error("File \"" + filePath + "\" not found!");

    auto readNextLine = [&file, str = std::string()]() mutable {
        std::getline(file, str);
        _removeWindowsSpecificCarriageReturn(str);
        return str;
    };

    auto readValues = [&readNextLine](auto convertFunc) {
        std::vector<decltype(convertFunc(""))> values;
        std::stringstream sstr(readNextLine());
        for (std::string str; std::getline(sstr, str, ' ');) {
            if (str.size())
                values.push_back(convertFunc(str));
        }
        return values;
    };
    auto convertFuncFloat = [](const std::string& str) { return std::stof(str); };
    auto convertFuncInt = [](const std::string& str) { return std::stoi(str); };

    auto readPoints = [&readNextLine, &readValues, convertFuncFloat] {
        std::vector<glm::vec3> points;
        const auto pointCount = std::stoul(readNextLine());
        for (decltype(std::stoul("")) i = 0; i < pointCount; ++i) {
            const auto point = readValues(convertFuncFloat);
            if (point.size() != 3)
                throw std::runtime_error("Error: Incorrect file format");

            points.emplace_back(point[0], point[1], point[2]);
        }
        return points;
    };

    this->m_Name = readNextLine();
    this->m_Name = this->m_Name.substr(this->m_Name.find(':') + 1);

    this->m_InitiatorPoints = readPoints();
    this->m_GeneratorPoints = readPoints();

    const auto range = readValues(convertFuncFloat);
    if (range.size() != 4)
        throw std::runtime_error("Error: Incorrect file format");

    this->m_RangeX = Interval(range[0], range[1]);
    this->m_RangeY = Interval(range[2], range[3]);

    const auto lastLine = readValues(convertFuncInt);
    if (lastLine.size() != 2)
        throw std::runtime_error("Error: Incorrect file format");

    this->m_ClearWindowEachTime = lastLine.back() != 0;
}

const Interval& GEN::getRangeX() const { return this->m_RangeX; }

const Interval& GEN::getRangeY() const { return this->m_RangeY; }

const std::string& GEN::getName() const { return this->m_Name; }

const std::vector<glm::vec3>& GEN::getInitiatorPoints() const { return this->m_InitiatorPoints; }

const std::vector<glm::vec3>& GEN::getGeneratorPoints() const { return this->m_GeneratorPoints; }

bool GEN::clearWindowEachTime() const { return this->m_ClearWindowEachTime; }

} // namespace cf
