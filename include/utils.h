
#ifndef _UTILS_H_H_
#define _UTILS_H_H_

#include <string>
#include <vector>
#include <iostream>

#include <inttypes.h>


#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/rotate_vector.hpp>




// enable output for vec2, vec3 & vec4
std::ostream& operator<<(std::ostream& of, const glm::vec2& vec);
std::ostream& operator<<(std::ostream& of, const glm::vec3& vec);
std::ostream& operator<<(std::ostream& of, const glm::vec4& vec);

// enable output for mat3x3 & mat4x4
std::ostream& operator<<(std::ostream& of, const glm::mat3x3& mat);
std::ostream& operator<<(std::ostream& of, const glm::mat4x4& mat);


namespace cf{

struct Color;
std::vector<Color> readPaletteFromFile(const char* filename);
std::string readAntString(const char* filename);

struct Convert{
    template<typename T>
    static T radiant2degree(T radiantValue){
        return radiantValue / glm::pi<T>() * T(180.0);
    }
    template<typename T>
    static T degree2radiant(T degreeValue){
        return degreeValue / T(180.0) * glm::pi<T>();
    }
};

struct Direction{
    enum AbsoluteDirection {
        NORTH, EAST, SOUTH, WEST, NUM_ABS_DIRS
    };
    enum RelativeDirection{
        LEFT, FORWARD, RIGHT
    };
    static AbsoluteDirection getNextiDirection(AbsoluteDirection currentDirection, RelativeDirection relativeMovement);
};



struct Intervall{
    Intervall(float _min, float _max):min(_min), max(_max){};
    float min;
    float max;

    static float translateInterverllPostion(const Intervall& originalInterall, const Intervall& newIntervall, float originalPosition);
};


struct Color{
    Color(int red = 0, int green = 0, int blue = 0) : r(red), g(green), b(blue){};

    // NOTE:
    // do not change the order of b, g, r in this struct!
    uint8_t b;
    uint8_t g;
    uint8_t r;

    bool operator==(const Color& c);
    bool operator!=(const Color& c);
};

void removeWindowsSpecificCarriageReturn(std::string& str);

}

#endif
