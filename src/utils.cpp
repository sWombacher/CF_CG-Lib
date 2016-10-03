

#include "utils.h"
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

std::vector<Color> readPaletteFromFile(const std::string& filePath){
    std::fstream file(filePath, std::fstream::in);
    if (!file)
        throw std::runtime_error("File not found in function: \"readPaletteFromFile\"");

    std::string str;
    if (!file.good()){
        str = filePath;
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
            str = filePath;
            str += ", wrong number of elements";
            throw std::runtime_error(str.c_str());
        }

        LUT.push_back(Color(rgb[0], rgb[1], rgb[2]));
    }
    return LUT;
}

std::string readAntString(const std::string& filePath){
    std::fstream file(filePath, std::fstream::in);
    if (!file)
        throw std::runtime_error("File not found in function: \"readAntString\"");

    std::string str;
    if (!file.good()){
        str = filePath;
        str += ", file not found";
        throw std::runtime_error(str.c_str());
    }

    // ignore first line
    std::getline(file, str);
    std::getline(file, str);
    _removeWindowsSpecificCarriageReturn(str);
    return str;
}

Direction::AbsoluteDirection Direction::getNextiDirection(AbsoluteDirection currentDirection, RelativeDirection relativeMovement){
    switch(relativeMovement){
    case RelativeDirection::LEFT:{
        int dir = int(currentDirection);
        --dir;
        if (dir < 0)
            dir += int(AbsoluteDirection::NUM_ABS_DIRS);

        return static_cast<AbsoluteDirection>(dir);
    }
    case RelativeDirection::RIGHT:{
        int dir = int(currentDirection);
        ++dir;
        if (dir >= int(AbsoluteDirection::NUM_ABS_DIRS))
            dir -= int(AbsoluteDirection::NUM_ABS_DIRS);

        return static_cast<AbsoluteDirection>(dir);
    }
    case RelativeDirection::FORWARD:
        return currentDirection;
    default:
        break;
    }
    return AbsoluteDirection::NUM_ABS_DIRS; // this should not occur hopfully :)
}
std::string Direction::toString(AbsoluteDirection absDir){
    switch(absDir){
    case AbsoluteDirection::WEST : return "WEST";
    case AbsoluteDirection::EAST : return "EAST";
    case AbsoluteDirection::NORTH: return "NORTH";
    case AbsoluteDirection::SOUTH: return "SOUTH";
    default:
        return "UNKOWN DIRECTION";
    }
}
std::string Direction::toString(RelativeDirection relDir){
    switch(relDir){
    case RelativeDirection::LEFT   : return "LEFT";
    case RelativeDirection::FORWARD: return "FORWARD";
    case RelativeDirection::RIGHT  : return "RIGHT";
    default:
        return "UNKOWN DIRECTION";
    }
}


float Interval::translateIntervalPostion(const Interval& originalInterval, const Interval& newInterval, float originalPosition){
    float factor = (newInterval.max - newInterval.min) / (originalInterval.max - originalInterval.min);
    originalPosition -= originalInterval.min;

    return originalPosition * factor + newInterval.min;
}


bool Color::operator==(const Color& c){
    if (this->r == c.r && this->g == c.g && this->b == c.b)
        return true;
    return false;
}

bool Color::operator!=(const Color& c){
    return !(*this == c);
}

Color Color::invert() const{
    Color toReturn = cf::Color::WHITE;
    toReturn -= *this;
    return toReturn;
}


std::ostream& operator<<(std::ostream &os, const Color& c){
    os << "Red: " << int(c.r) << "   Green: " << int(c.g) << "   Blue: " << int(c.b);
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
const Color Color::GREY    = Color(127, 127, 127);
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

float radian2degree(float radianValue){
    return radianValue / glm::pi<float>() * 180.f;
}

float degree2radian(float degreeValue){
    return degreeValue / 180.f * glm::pi<float>();
}


std::string Console::readString() {
	Console::_console2foreground();
	std::string tmp;
    std::cin.clear();
	std::getline(std::cin, tmp);
	return tmp;
}
float Console::readFloat() {
	float value;
    auto getString = []() -> std::string {
        std::string str = Console::readString();
        std::replace(str.begin(), str.end(), ',', '.');
        return str;
    };

    std::stringstream sstr(getString());
	sstr >> value;
    while (sstr.fail()) {
		std::cout << "\nError: provided value is not of type floatingpoint, please enter a valid value" << std::endl;
		sstr.clear();
        sstr << getString();
		sstr >> value;
	}
	return value;
}
int Console::readInt() {
	int value;
    auto getString = []() -> std::string {
        std::string str = Console::readString();
        if (std::find(str.begin(), str.end(), ',') != str.end())
            std::cout << "Warning: provided value is a floating point value, required type: int" << std::endl;
        if (std::find(str.begin(), str.end(), '.') != str.end())
            std::cout << "Warning: provided value is a floating point value, required type: int" << std::endl;
        return str;
    };

    std::stringstream sstr(getString());
	sstr >> value;
    while (sstr.fail()) {
		std::cout << "\nError: provided value is not of type integer, please enter a valid value" << std::endl;
		sstr.clear();
        sstr << getString();
		sstr >> value;
	}
	return value;
}

void Console::waitKey() {
	Console::_console2foreground();
	getchar();
}

void Console::clearConsole() {
	Console::_console2foreground();

#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}

#ifdef _WIN32
#include <Windows.h>
#endif

void Console::_console2foreground() {
#ifdef _WIN32
	static HWND consoleHandle = GetConsoleWindow();
	SetForegroundWindow(consoleHandle);
#else
	/// currently only supported on windows
#endif
}

}



