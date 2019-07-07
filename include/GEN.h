#pragma once

#include <utils.h>

namespace cf {

struct GEN {
    /**
     * @brief read a *.ifs file from path
     * @param fiilePath Path to a *.ifs file
     */
    void read(const std::string& filePath);

    const cf::Interval& getRangeX() const;
    const cf::Interval& getRangeY() const;
    const std::string& getName() const;

    const std::vector<glm::vec3>& getInitiatorPoints() const;
    size_t getInitiatorPointCount() const;
    glm::vec3 getInitiatorPoint(size_t idx) const;

    const std::vector<glm::vec3>& getGeneratorPoints() const;
    size_t getGeneratorPointCount() const;
    glm::vec3 getGeneratorPoint(size_t idx) const;

    bool clearWindowEachTime() const;

private:
    std::string m_Name;

    bool m_ClearWindowEachTime;

    std::vector<glm::vec3> m_InitiatorPoints;
    std::vector<glm::vec3> m_GeneratorPoints;

    Interval m_RangeX;
    Interval m_RangeY;
};


}
