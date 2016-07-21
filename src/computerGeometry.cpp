#include "computerGeometry.h"
#include <fstream>
#include <sstream>

std::ostream& operator<<(std::ostream& os, const cf::Vec3& rhs){ return os << rhs.m_Data; }

namespace cf{
std::vector<PointVector> readDATFile(const std::string &filePath) {
    std::fstream file(filePath, std::fstream::in);
    if (!file)
        throw std::runtime_error("File not found in function: \"readPaletteFromFile\"");

    std::string str;
    if (!file.good()){
        str = filePath;
        str += ", file not found";
        throw std::runtime_error(str);
    }

    std::vector<PointVector> points;
    while (file.good()){
        std::getline(file, str);
        cf::_removeWindowsSpecificCarriageReturn(str);

        glm::vec3 tmp;
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
        if (valueCounter == 3){
            PointVector point;
            point.m_Data = tmp;
            points.push_back(point);
        }
    }
    return points;
}



Vec3::Vec3(bool pointVector, float x, float y, float w) : m_PointVector(pointVector), m_Data(x, y, w){}

Vec3  Vec3::operator+ (const Vec3& rhs) const{
    Vec3 toReturn(this->m_PointVector | rhs.m_PointVector);
    toReturn.m_Data = this->m_Data + rhs.m_Data;
    return toReturn;
}
Vec3& Vec3::operator+=(const Vec3& rhs){
    this->m_Data += rhs.m_Data;
    return *this;
}

Vec3  Vec3::operator- (const Vec3& rhs) const{
    Vec3 toReturn(this->m_PointVector | rhs.m_PointVector);
    toReturn.m_Data = this->m_Data - rhs.m_Data;
    return toReturn;
}
Vec3& Vec3::operator-=(const Vec3& rhs){
    this->m_Data -= rhs.m_Data;
    return *this;
}

Vec3  Vec3::operator* (float rhs) const{
    Vec3 toReturn(this->m_PointVector);
    toReturn.m_Data = this->m_Data * rhs;
    return toReturn;
}
Vec3& Vec3::operator*=(float rhs){
    this->m_Data *= rhs;
    return *this;
}

cf::Vec3 operator*(float lhs, const cf::Vec3& vec){
    Vec3 toReturn(vec.m_PointVector);
    toReturn.m_Data = lhs * vec.m_Data;
    return toReturn;
}

Vec3  Vec3::operator% (const Vec3& rhs) const{
    Vec3 toReturn(this->m_PointVector | rhs.m_PointVector);
    toReturn.m_Data = glm::cross(this->m_Data, rhs.m_Data);
    return toReturn;
}
Vec3& Vec3::operator%=(const Vec3& rhs){
    this->m_Data = glm::cross(this->m_Data, rhs.m_Data);
    return *this;
}

void Vec3::normalize(){
    if (!this->m_PointVector)
        throw std::runtime_error("DirectionVector cannot be normalized!");
    this->m_Data.x /= this->m_Data.z;
    this->m_Data.y /= this->m_Data.z;
    this->m_Data.z = 1.f; // this->m_Data.z /= this->m_Data.z;
}
bool Vec3::isPointVector() const{ return this->m_PointVector; }
float Vec3::scalarProduct(const Vec3& rhs) const{ return glm::dot(this->m_Data, rhs.m_Data); }

float Vec3::getX() const{ return this->m_Data.x; }
float Vec3::getY() const{ return this->m_Data.y; }
float Vec3::getW() const{ return this->m_Data.z; }

void Vec3::setX(float value){ this->m_Data.x = value; }
void Vec3::setY(float value){ this->m_Data.y = value; }
void Vec3::setW(float value){
    if (!this->m_PointVector && value != 0.f)
        throw std::runtime_error("W value of a DirectionVector may not be set!");
    this->m_Data.z = value;
}

float& Vec3::operator[](int idx){ return this->m_Data[idx]; }

Vec3::operator cf::Point () const {
    cf::Vec3 cpy = *this;
    cpy.normalize();
    return { cpy.getX(), cpy.getY() };
}

Vec3::operator glm::vec3& () {
    return this->m_Data;
}

}
