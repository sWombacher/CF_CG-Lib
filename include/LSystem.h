#ifndef LSYSTEM_H_H
#define LSYSTEM_H_H

#include <string>
#include <vector>
#include <memory>

#include <glm/glm.hpp>

#include "utils.h"

namespace cf{

/**
 * @brief The LindenmayerSystem class
 * lazy people (like myself) may use the IFS tyepdef
 */
struct LindenmayerSystem{

    /**
     * @brief read a *.lin file from path
     * @param filePath Path to a *.lin file
     */
    void read(const std::string& filePath);

    const std::string& getName () const;
    char getAxiom() const;

    const std::string* getProduction(char symbol) const;

    std::size_t getNumProductions() const;

    bool clearWindowEachTime() const;

    const Interval& getRangeX() const;
    const Interval& getRangeY() const;

    float getScale() const;
    float getStartAngle() const;
    float getAdjustmentAngel() const;

    const std::vector<std::pair<const char, const std::string> >& getAllProductions() const;

private:
    std::string m_Name;

    char m_Axiom;
    std::vector<std::pair<const char, const std::string> > m_Productions;


    float m_Scale;
    float m_StartAngle;
    float m_AdjustmentAngle;

    bool m_ClearWindowEachTime;


    Interval m_RangeX;
    Interval m_RangeY;
};

typedef LindenmayerSystem LSystem; // short version for lazy people like myself :)
}


#endif
