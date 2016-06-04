

#include "utils.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <opencv2/opencv.hpp>

typedef unsigned int uint; // may not be defined (for example on windows)


std::ostream& operator<<(std::ostream& of, const glm::vec2& vec){
    of << '(' << vec[0] << ',' << vec[1] << ')';
    return of;
}
std::ostream& operator<<(std::ostream& of, const glm::vec3& vec){
    of << '(' << vec[0] << ',' << vec[1] << ',' << vec[2] << ')';
    return of;
}
std::ostream& operator<<(std::ostream& of, const glm::vec4& vec){
    of << '(' << vec[0] << ',' << vec[1] << ',' << vec[2] << ',' << vec[3] << ')';
    return of;
}


std::ostream& operator<<(std::ostream& of, const glm::mat3x3& mat){
    of << std::fixed << std::setprecision(4)
       << '/'  << mat[0][0] << ' ' << mat[1][0] << ' ' << mat[2][0] << "\\\n"
       << '|'  << mat[0][1] << ' ' << mat[1][1] << ' ' << mat[2][1] << "|\n"
       << '\\' << mat[0][2] << ' ' << mat[1][2] << ' ' << mat[2][2] << "/\n";
    return of;
}

std::ostream& operator<<(std::ostream& of, const glm::mat4x4& mat){
    of << std::fixed << std::setprecision(5)
       << '/'  << mat[0][0] << ' ' << mat[1][0] << ' ' << mat[2][0] << ' ' << mat[3][0] << "\\\n"
       << '|'  << mat[0][1] << ' ' << mat[1][1] << ' ' << mat[2][1] << ' ' << mat[3][1] << "|\n"
       << '|'  << mat[0][2] << ' ' << mat[1][2] << ' ' << mat[2][2] << ' ' << mat[3][2] << "|\n"
       << '\\' << mat[0][3] << ' ' << mat[1][3] << ' ' << mat[2][3] << ' ' << mat[3][3] << "/\n";
    return of;
}




namespace cf{

std::vector<Color> readPaletteFromFile(const std::string& filename){
    std::fstream file(filename, std::fstream::in);
    if (!file)
        throw "File not found in function: \"readPaletteFromFile\"";

    std::string str;
    if (!file.good()){
        str = filename;
        str += ", file not found";
        throw std::runtime_error(str.c_str());
    }

    std::vector<Color> LUT;

    while(std::getline(file, str)){
        std::stringstream sstr;
        sstr << str;

        std::vector<uint8_t> rgb;
        while(std::getline(sstr, str, ',')){
            _removeWindowsSpecificCarriageReturn(str);
            rgb.push_back(uint8_t(std::stoi(str)));
        }

        if (rgb.size() != 3){
            str = filename;
            str += ", wrong number of elements";
            throw std::runtime_error(str.c_str());
        }

        LUT.push_back(Color(rgb[0], rgb[1], rgb[2]));
    }
    return LUT;
}

std::string readAntString(const char* filename){
    std::fstream file(filename, std::fstream::in);
    if (!file)
        throw "File not found in function: \"readAntString\"";

    std::string str;
    if (!file.good()){
        str = filename;
        str += ", file not found";
        throw std::runtime_error(str.c_str());
    }

    // ignore first line
    std::getline(file, str);
    std::getline(file, str);
    _removeWindowsSpecificCarriageReturn(str);
    return str;
}


namespace Direction{
AbsoluteDirection getNextiDirection(AbsoluteDirection currentDirection, RelativeDirection relativeMovement){
    switch(relativeMovement){
    case RelativeDirection::LEFT:{

        int dir = currentDirection;
        --dir;
        if (dir < 0)
            dir += AbsoluteDirection::NUM_ABS_DIRS;

        return static_cast<AbsoluteDirection>(dir);
    }
    case RelativeDirection::RIGHT:{

        int dir = currentDirection;
        ++dir;
        if (dir >= AbsoluteDirection::NUM_ABS_DIRS)
            dir -= AbsoluteDirection::NUM_ABS_DIRS;

        return static_cast<AbsoluteDirection>(dir);
    }
    case RelativeDirection::FORWARD:
        return currentDirection;
    }
    return AbsoluteDirection::NUM_ABS_DIRS; // this should not occur hopfully :)
}}


float Intervall::translateIntervallPostion(const Intervall& originalIntervall, const Intervall& newIntervall, float originalPosition){
    float factor = (newIntervall.max - newIntervall.min) / (originalIntervall.max - originalIntervall.min);
    originalPosition -= originalIntervall.min;

    return originalPosition * factor + newIntervall.min;
}


bool Color::operator==(const Color& c){
    if (this->r == c.r && this->g == c.g && this->b == c.b)
        return true;
    return false;
}

bool Color::operator!=(const Color& c){
    return !(*this == c);
}


std::ostream& operator<<(std::ostream &os, const Color& c){
    os << "Red: " << c.r << "\tGreen: " << c.g << "\tBlue: " << c.b;
    return os;
}

Color  Color::operator* (float value){
    Color tmp = *this;
    tmp *= value;
    return tmp;
}
Color  Color::operator/ (float value){
    Color tmp = *this;
    tmp /= value;
    return tmp;
}

Color  Color::operator+ (const Color& c){
    Color toReturn = *this;
    toReturn += c;
    return toReturn;
}
Color  Color::operator- (const Color& c){
    Color toReturn = *this;
    toReturn -= c;
    return toReturn;
}

Color& Color::operator*=(float value){
    cv::Vec3b* e1 = reinterpret_cast<cv::Vec3b*>(this);
    (*e1) *= value;
    return *this;
}
Color& Color::operator/=(float value){
    cv::Vec3b* e1 = reinterpret_cast<cv::Vec3b*>(this);
    (*e1) /= value;
    return *this;
}

Color& Color::operator+=(const Color& c){
    int red   = int(this->r) + int(c.r);
    int green = int(this->g) + int(c.g);
    int blue  = int(this->b) + int(c.b);

    if (red > 255)
        red = 255;
    if (green > 255)
        green = 255;
    if (blue > 255)
        blue = 255;

    this->r = uint8_t(red);
    this->g = uint8_t(green);
    this->b = uint8_t(blue);

    return *this;
}
Color& Color::operator-=(const Color& c){
    int red   = int(this->r) - int(c.r);
    int green = int(this->g) - int(c.g);
    int blue  = int(this->b) - int(c.b);

    if (red < 0)
        red = 0;
    if (green < 0)
        green = 0;
    if (blue < 0)
        blue = 0;

    this->r = uint8_t(red);
    this->g = uint8_t(green);
    this->b = uint8_t(blue);

    return *this;
}

const Color Color::MAGENTA = Color(128,   0, 255);
const Color Color::YELLOW  = Color(255, 255,   0);
const Color Color::ORANGE  = Color(255, 128,   0);
const Color Color::WHITE   = Color(255, 255, 255);
const Color Color::BLACK   = Color(  0,   0,   0);
const Color Color::GREEN   = Color(  0, 255,   0);
const Color Color::BLUE    = Color(  0,   0, 255);
const Color Color::CYAN    = Color(  0, 255, 255);
const Color Color::PINK    = Color(255,   0, 255);
const Color Color::RED     = Color(255,   0,   0);



cf::Color operator* (float value, const cf::Color& c){
    cf::Color tmp = c;
    tmp *= value;
    return tmp;
}
cf::Color operator/ (float value, const cf::Color& c){
    cf::Color tmp = c;
    tmp /= value;
    return tmp;
}



void _removeWindowsSpecificCarriageReturn(std::string& str){
    if (!str.size())
        return;

    // NOTE:
    // erase function didn't work here...
    // for whatever reason...

    // count \r
    std::size_t count = 0;
    for (auto e : str){
        if (e == '\r')
            ++count;
    }

    if (count){
        std::string toReturn;
        toReturn.resize(str.size() - count);
        for (std::size_t iter = 0, iter2 = 0; iter < str.size(); ++iter){
            if (str[iter] != '\r'){
                toReturn[iter2] = str[iter];
                ++iter2;
            }
        }
        str = std::move(toReturn);
    }
}

float radiant2degree(float radiantValue){
    return radiantValue / glm::pi<float>() * 180.f;
}

float degree2radiant(float degreeValue){
    return degreeValue / 180.f * glm::pi<float>();
}


}

