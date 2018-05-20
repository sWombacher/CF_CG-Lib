#ifndef LSYSTEM_H_H
#define LSYSTEM_H_H

#include <map>
#include <memory>
#include <string>

#include <glm/glm.hpp>

#include "utils.h"

namespace cf {

/**
 * @brief The LindenmayerSystem class
 * lazy people (like myself) may use the IFS tyepdef
 */
struct LindenmayerSystem {

    /**
     * @brief read a *.lin file from path
     * @param filePath Path to a *.lin file
     */
    void read(const std::string& filePath);

    const std::string& getName() const;
    const std::string& getAxiom() const;

    const std::string* getProduction(char symbol) const;

    std::size_t getNumProductions() const;

    bool clearWindowEachTime() const;

    const Interval& getRangeX() const;
    const Interval& getRangeY() const;

    float getScale() const;
    float getStartAngle() const;
    float getAdjustmentAngle() const;

    const std::map<char, const std::string>& getAllProductions() const;

  private:
    std::string m_Name;
    std::string m_Axiom;
    std::map<char, const std::string> m_Productions;

    float m_Scale;
    float m_StartAngle;
    float m_AdjustmentAngle;

    bool m_ClearWindowEachTime;

    Interval m_RangeX;
    Interval m_RangeY;
};

typedef LindenmayerSystem LSystem; // short version for lazy people like myself :)

/**
 * @brief The LSystem_Controller struct\n
 * This class enables easy iterating above a given iteration depth\n
 *
 * usage: \n
 \verbatim
 LSystem_Controller myController(<depth>, <lsystem>);
 for (char c : myController)
      std::cout << c;
 \endverbatim
 */
struct LSystem_Controller {
    LSystem_Controller(size_t depth, const LSystem& LSystem);

    struct iterator {
        const char& operator*();
        iterator& operator++();
        bool operator!=(const iterator& rhs);

      private:
        using ProductionMap = std::decay<decltype(LSystem().getAllProductions())>::type;

        friend struct LSystem_Controller;
        iterator(char axiom, const ProductionMap& lsystem, size_t depth, bool endIterator);

        std::vector<std::pair<int, char>> m_Positions;
        const ProductionMap& m_Productions;
        char m_CurrentProduction = '\0';
        size_t m_CurrentDepth = 0;
        bool m_EndReached;
    };

    iterator begin() const;
    iterator end() const;

  private:
    char m_Axiom;
    const size_t m_Depth;
    iterator::ProductionMap m_Productions;
};
}

#endif
