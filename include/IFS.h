
#ifndef H_ITERATED_FUNCTION_SYSTEM_H_H
#define H_ITERATED_FUNCTION_SYSTEM_H_H


#include "utils.h"


namespace cf{


class IteratedFunctionSystem{

public:
    IteratedFunctionSystem() = default;
    void read(const char* fiilename);

    std::size_t getNumTransformations() const;

    const glm::mat3x3& getTransformation(std::size_t pos) const;

    const Intervall& getRangeX() const;
    const Intervall& getRangeY() const;
    const std::string& getName() const;
    const std::vector<glm::mat3x3>& getAllTransformation() const;


private:
    std::string m_Name;
    std::vector<glm::mat3x3> m_Transformations;

    Intervall m_RangeX;
    Intervall m_RangeY;
};



typedef IteratedFunctionSystem IFS; // short version for lazy people like myself :)

}


#endif
