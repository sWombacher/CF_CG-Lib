#ifndef UTILS_H_H
#define UTILS_H_H

#include <string>
#include <vector>
#include <iostream>

#include <inttypes.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/rotate_vector.hpp>

// enable standard output for vec2, vec3 & vec4
std::ostream& operator<<(std::ostream& of, const glm::vec2& vec);
std::ostream& operator<<(std::ostream& of, const glm::vec3& vec);
std::ostream& operator<<(std::ostream& of, const glm::vec4& vec);

// enable standard output for mat3x3 & mat4x4
std::ostream& operator<<(std::ostream& of, const glm::mat3x3& mat);
std::ostream& operator<<(std::ostream& of, const glm::mat4x4& mat);


namespace cf{
struct Color;

/**
 * @brief readPaletteFromFile
 * @param filePath read *.pal file from path
 * @return
 */
std::vector<Color> readPaletteFromFile(const std::string& filePath);

/**
 * @brief readAntString
 * @param filePath read *.ant file from path
 * @return
 */
std::string readAntString(const char* filePath);


/**
 * @brief radiant2degree converts from radiant value to degree values
 * @param radiantValue
 * @return degree value
 */
float radiant2degree(float radiantValue);

/**
 * @brief degree2radiant converts from degree value to radiant values
 * @param degreeValue
 * @return radiant value
 */
float degree2radiant(float degreeValue);


/**
 * @brief The Direction struct for getting absolute directions from a current direction and a relative direction
 */
struct Direction{
    enum AbsoluteDirection { NORTH, EAST, SOUTH, WEST, NUM_ABS_DIRS };
    enum RelativeDirection { LEFT, FORWARD, RIGHT, NUM_REL_DIRS };

    /**
     * @brief getNextiDirection receive absolute direction by providing a relative directon
     * @param currentDirection current absolute direction
     * @param relativeMovement relative movement
     * @return
     */
    static AbsoluteDirection getNextiDirection(AbsoluteDirection currentDirection, RelativeDirection relativeMovement);
};


/**
 * @brief The Intervall struct provides functionallity to translate values from one intervall into another
 */
struct Intervall{
    Intervall(float _min = 0, float _max = 0):min(_min), max(_max){}
    float min;
    float max;

    static float translateIntervallPostion(const Intervall& originalIntervall, const Intervall& newIntervall, float originalPosition);
    friend std::ostream& operator<<(std::ostream& os, const Intervall& intervall);
};

/**
 * @brief The Color struct offers a class for rgb access
 */
struct Color{
    Color(uint8_t red = 0, uint8_t green = 0, uint8_t blue = 0) : b(blue), g(green), r(red){}

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
    Color invert() const;

    static const Color MAGENTA;
    static const Color YELLOW;
    static const Color ORANGE;
    static const Color WHITE;
    static const Color BLACK;
    static const Color GREEN;
    static const Color GREY;
    static const Color BLUE;
    static const Color CYAN;
    static const Color PINK;
    static const Color RED;
};

/**
 * @brief _removeWindowsSpecificCarriageReturn removes '\r' characters in strings ('\r' may be read from unix system with windows files)
 * @param str string containing '\r', which will be removed
 */
void _removeWindowsSpecificCarriageReturn(std::string& str);

}

#endif
