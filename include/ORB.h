#ifndef ORBIT_H_H
#define ORBIT_H_H

#include "utils.h"

namespace cf{

/**
 * @brief The Orbit class
 * lazy people (like myself) may use the ORB tyepdef
 */
struct Orbit{
    /**
     * @brief read a *.orb file from path
     * @param filePath Path to a *.orb file
     */
    void read(const std::string& filePath);

    const Interval& getRangeX() const;
    const Interval& getRangeY() const;

    const std::string& getName() const;

    const std::vector<glm::vec3>& getAllStartingPoints() const;
    const std::vector<float>&     getAllFactors       () const;

    std::size_t getNumFactors       () const;
    std::size_t getNumStartingPoints() const;

private:
    std::string m_Name;
    Interval m_IntervalX;
    Interval m_IntervalY;

    std::vector<glm::vec3> m_StartPoints;
    std::vector<float> m_Factors;
};

typedef Orbit ORB;

}

#endif
