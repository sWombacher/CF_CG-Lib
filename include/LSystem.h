

#ifndef LSYSTEM_H_H
#define LSYSTEM_H_H

#include <string>
#include <vector>
#include <memory>

#include <glm/glm.hpp>


#include "utils.h"


namespace cf{


class LindenmayerSystem{

public:
    void read(const char* filename);

    const std::string& getName () const;
    const std::string& getAxiom() const;

    const std::string* getProduction(char symbol) const;

    std::size_t getNumProductions() const;

    bool clearWindowEachTime() const;

    const Intervall& getRangeX() const;
    const Intervall& getRangeY() const;

    float getScale() const;
    float getStartAngle() const;
    float getAdjustmentAngel() const;

    const std::vector<std::pair<const char, const std::string> >& getAllProductions() const;

private:
    std::string m_Name;

    std::string m_Axiom;
    std::vector<std::pair<const char, const std::string> > m_Productions;


    float m_Scale;
    float m_StartAngle;
    float m_AdjustmentAngle;

    bool m_ClearWindowEachTime;


    Intervall m_RangeX;
    Intervall m_RangeY;
};



typedef LindenmayerSystem LSystem; // short version for lazy people like myself :)
}


#endif
