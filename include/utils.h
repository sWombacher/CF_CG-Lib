#ifndef UTILS_H_H
#define UTILS_H_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
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

/**
 * @brief _removeWindowsSpecificCarriageReturn Removes 'carriage return' characters in strings ('carriage return' may be read from unix system by providing windows files)
 * @param str string containing 'carriage return', which will be removed
 */
void _removeWindowsSpecificCarriageReturn(std::string& str);


struct Color;

/**
 * @brief readPaletteFromFile
 * @param filePath Read *.pal file from path
 * @return
 */
std::vector<Color> readPaletteFromFile(const std::string& filePath);

/**
 * @brief readAntString
 * @param filePath Read *.ant file from path
 * @return
 */
std::string readAntString(const std::string& filePath);

/**
 * @brief readDATFile Reads a *.dat file
 * @param filePath Read *.dat file from path
 * @return
 */
template<typename _VectorType = glm::vec3>
std::vector<_VectorType> readDATFile(const std::string& filePath){
    std::fstream file(filePath, std::fstream::in);
    if (!file)
        throw std::runtime_error("File not found in function: \"readPaletteFromFile\"");

    std::string str;
    if (!file.good())
        throw std::runtime_error(filePath + ", file not found");

    std::vector<_VectorType> points;
    while (file.good()){
        std::getline(file, str);
        cf::_removeWindowsSpecificCarriageReturn(str);

        // remove non numbers, and non . ' '
        for (size_t i = 0; i < str.size(); ++i){
            char c = str[i];
            if (c != ' ' && c != '.' && (c < '0' || c > '9')){
                std::cout << "Warning: Unknown symbol detected, ASCII code: '" << (int)str[i] << "'" << std::endl;
                str.erase(i, 1);
                --i;
            }
        }

        _VectorType tmp;
        std::stringstream sstr(str);
        int valueCounter = 0;
        while (sstr.good()){
            std::getline(sstr, str, ' ');
            if (str.size()){
                float value = std::stof(str);
                tmp[valueCounter] = value;
                ++valueCounter;
                if (valueCounter >= 3)
                    break;
            }
        }
        if (valueCounter == 3)
            points.push_back(tmp);
    }
    return points;
}
/**
 * @brief radian2degree Converts a radian value to a degree value
 * @param radianValue Radian value to be converted
 * @return Converted degree value
 */
float radian2degree(float radianValue);

/**
 * @brief degree2radian Converts a degree value to a radian value
 * @param degreeValue Degree value to be converted
 * @return Converted radian value
 */
float degree2radian(float degreeValue);


/**
 * @brief The Direction struct for getting absolute directions from a current direction and a relative direction
 */
struct Direction{
    enum class AbsoluteDirection { NORTH, EAST, SOUTH, WEST, NUM_ABS_DIRS };
    enum class RelativeDirection { LEFT, FORWARD, RIGHT, NUM_REL_DIRS };

    /**
     * @brief getNextiDirection receive absolute direction by providing a relative directon
     * @param currentDirection current absolute direction
     * @param relativeMovement relative movement
     * @return
     */
    static AbsoluteDirection getNextiDirection(AbsoluteDirection currentDirection, RelativeDirection relativeMovement);
    static std::string toString(AbsoluteDirection absDir);
    static std::string toString(RelativeDirection relDir);
};


/**
 * @brief The Interval struct provides functionallity to translate values from one interval into another
 */
struct Interval{
    Interval(float _min = 0, float _max = 0):min(_min), max(_max){}
    float min;
    float max;

    static float translateIntervalPostion(const Interval& originalInterval, const Interval& newInterval, float originalPosition);
    friend std::ostream& operator<<(std::ostream& os, const Interval& interval);
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
 * @brief The Console struct offers utility functions for 'console'
 */
struct Console {
    /**
     * @brief readString Read a line into a std::string (includes spaces)
     * @return Read line
     */
	static std::string readString();

    /**
     * @brief readFloat Reads a floatingpoint value
     * @return Read value
     */
	static float readFloat();

    /**
     * @brief readInt Reads a integer value
     * @return Read value
     */
	static int readInt();

    /**
     * @brief waitKey Wait until key input (on windows also sets the console window active)
     */
	static void waitKey();

    /**
     * @brief clearConsole Clears the console
     */
	static void clearConsole();

private:
	static void _console2foreground();
};

}

#endif
