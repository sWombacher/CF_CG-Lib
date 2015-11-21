

#include "utils.h"
#include <fstream>
#include <sstream>
#include <iomanip>




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
       << '\\' << mat[0][2] << ' ' << mat[1][2] << ' ' << mat[2][2] << ' ' << mat[3][2] << "/\n";
    return of;
}






namespace cf{

std::vector<Color> readPaletteFromFile(const char* filename){
    std::fstream file(filename, std::fstream::in);
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

        std::vector<int> rgb;
        while(std::getline(sstr, str, ',')){
            removeWindowsSpecificCarriageReturn(str);
            rgb.push_back(std::stoi(str));
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
    std::string str;
    if (!file.good()){
        str = filename;
        str += ", file not found";
        throw std::runtime_error(str.c_str());
    }
    // ignore first line
    std::getline(file, str);
    std::getline(file, str);
    removeWindowsSpecificCarriageReturn(str);
    return str;
}


Direction::AbsoluteDirection Direction::getNextiDirection(AbsoluteDirection currentDirection, RelativeDirection relativeMovement){
    switch(relativeMovement){
    case RelativeDirection::LEFT:{

        int dir = currentDirection;
        --dir;
        if (dir < 0)
            dir += AbsoluteDirection::NUM_ABS_DIRS;

        return static_cast<AbsoluteDirection>(dir);
    }
        break;
    case RelativeDirection::RIGHT:{

        int dir = currentDirection;
        ++dir;
        if (dir >= AbsoluteDirection::NUM_ABS_DIRS)
            dir -= AbsoluteDirection::NUM_ABS_DIRS;

        return static_cast<AbsoluteDirection>(dir);
    }
        break;
    default:
        return currentDirection;
    }
}



float Intervall::translateInterverllPostion(const Intervall& originalInterall, const Intervall& newIntervall, float originalPosition){
    float factor = (newIntervall.max - newIntervall.min) / (originalInterall.max - originalInterall.min);
    originalPosition -= originalInterall.min;

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


const Color ColorType::MAGENTA = Color(128,   0, 255);
const Color ColorType::YELLOW  = Color(255, 255,   0);
const Color ColorType::ORANGE  = Color(255, 128,   0);
const Color ColorType::WHITE   = Color(255, 255, 255);
const Color ColorType::BLACK   = Color(  0,   0,   0);
const Color ColorType::GREEN   = Color(  0, 255,   0);
const Color ColorType::BLUE    = Color(  0,   0, 255);
const Color ColorType::CYAN    = Color(  0, 255, 255);
const Color ColorType::PINK    = Color(255,   0, 255);
const Color ColorType::RED     = Color(255,   0,   0);




void removeWindowsSpecificCarriageReturn(std::string& str){
    if (!str.size())
        return;

    // NOTE:
    // erase function didn't work here...
    // for whatever reason...

    // count \r
    int count = 0;
    for (auto e : str){
        if (e == '\r')
            ++count;
    }

    if (count){
        std::string toReturn;
        toReturn.resize(str.size() - count);
        for (int iter = 0, iter2 = 0; iter < str.size(); ++iter){
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
