

#include "utils.h"

namespace cf{

class Orbit{
public:
    Orbit() = default;
    void read(const char* filename);

    const Intervall& getRangeX() const;
    const Intervall& getRangeY() const;

    const std::string& getName() const;

    const std::vector<glm::vec3>& getAllStartingPoints() const;
    const std::vector<float>&     getAllFactors       () const;

    std::size_t getNumFactors       () const;
    std::size_t getNumStartingPoints() const;

private:
    std::string m_Name;
    Intervall m_IntervallX;
    Intervall m_IntervallY;

    std::vector<glm::vec3> m_StartPoints;
    std::vector<float> m_Factors;
};

typedef Orbit ORB;

}

