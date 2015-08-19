

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

void removeWindowsSpecificCarriageReturn(std::string& str){
    if (!str.size())
        return;

    std::size_t iter;
    while((iter = str.find('\r')) != std::string::npos)
        str.erase(iter);

    /*
    std::size_t cr = str.rfind('\r');
    if (cr != std::string::npos)
        str = str.substr(0, cr);
    */
}


}
