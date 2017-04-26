#ifndef ITERATED_FUNCTION_SYSTEM_H_H
#define ITERATED_FUNCTION_SYSTEM_H_H

#include "utils.h"


namespace cf{

/**
 * @brief The IteratedFunctionSystem class
 * lazy people (like myself) may use the IFS tyepdef
 */
struct IteratedFunctionSystem{
    /**
     * @brief read a *.ifs file from path
     * @param fiilePath Path to a *.ifs file
     */
    void read(const std::string& fiilePath);

    std::size_t getNumTransformations() const;

    const glm::mat3x3& getTransformation(std::size_t pos) const;

    const Interval& getRangeX() const;
    const Interval& getRangeY() const;
    const std::string& getName() const;
    const std::vector<glm::mat3x3>& getAllTransformation() const;

private:
    std::string m_Name;
    std::vector<glm::mat3x3> m_Transformations;

    Interval m_RangeX;
    Interval m_RangeY;
};

typedef IteratedFunctionSystem IFS; // short version for lazy people like myself :)

}

#endif // ITERATED_FUNCTION_SYSTEM_H_H
