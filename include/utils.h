
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


/// TODO:
/// orb-files, dat-files

// enable standard output for vec2, vec3 & vec4
std::ostream& operator<<(std::ostream& of, const glm::vec2& vec);
std::ostream& operator<<(std::ostream& of, const glm::vec3& vec);
std::ostream& operator<<(std::ostream& of, const glm::vec4& vec);

// enable standard output for mat3x3 & mat4x4
std::ostream& operator<<(std::ostream& of, const glm::mat3x3& mat);
std::ostream& operator<<(std::ostream& of, const glm::mat4x4& mat);


namespace cf{

struct Color;
std::vector<Color> readPaletteFromFile(const char* filename);
std::string readAntString(const char* filename);


float radiant2degree(float radiantValue);
float degree2radiant(float degreeValue);


namespace Direction{
    enum AbsoluteDirection { NORTH, EAST, SOUTH, WEST, NUM_ABS_DIRS };
    enum RelativeDirection { LEFT, FORWARD, RIGHT };
    static AbsoluteDirection getNextiDirection(AbsoluteDirection currentDirection, RelativeDirection relativeMovement);
};



struct Intervall{
    Intervall(float _min = 0, float _max = 0):min(_min), max(_max){};
    float min;
    float max;

    static float translateInterverllPostion(const Intervall& originalInterall, const Intervall& newIntervall, float originalPosition);
};



struct Color{
    Color(int red = 0, int green = 0, int blue = 0) : r(red), g(green), b(blue){};

    // NOTE:
    // do not change the order of b, g, r in this struct!
    // this order is required for some window2D functions
    uint8_t b;
    uint8_t g;
    uint8_t r;

    Color  operator* (float value);
    Color  operator/ (float value);

    Color& operator*=(float value);
    Color& operator/=(float value);

    Color  operator+ (const Color& c);
    Color  operator- (const Color& c);

    Color& operator+=(const Color& c);
    Color& operator-=(const Color& c);

    friend cf::Color operator*(float value, const cf::Color& c);
    friend cf::Color operator/(float value, const cf::Color& c);

    bool operator==(const Color& c);
    bool operator!=(const Color& c);

    friend std::ostream& operator<<(std::ostream& os, const Color& c);

    static const Color MAGENTA;
    static const Color YELLOW;
    static const Color ORANGE;
    static const Color WHITE;
    static const Color BLACK;
    static const Color GREEN;
    static const Color BLUE;
    static const Color CYAN;
    static const Color PINK;
    static const Color RED;
};

void removeWindowsSpecificCarriageReturn(std::string& str);
}




#endif
