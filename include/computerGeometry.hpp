#ifndef COMPUTER_GEOMETRY_H_H
#define COMPUTER_GEOMETRY_H_H

#include "windowCoordinateSystem.hpp"
#include <sstream>
#include <fstream>
#include <string>


namespace cf {
	template<bool POINTVECTOR> struct Vec3;
	typedef Vec3<true > PointVector;
	typedef Vec3<false> DirectionVector;
}

template<bool b>
std::ostream& operator<<(std::ostream& os, const cf::Vec3<b>& rhs);



namespace cf{

std::vector<PointVector> readDATFile(const std::string& filePath);

template<bool POINTVECTOR>
struct Vec3{
    Vec3(float x = 0.f, float y = 0.f) : m_Data(x, y, POINTVECTOR ? 1.f : 0.f) {}
    Vec3(float x, float y, float w) : m_Data(x, y, w) {}
    Vec3(const cf::Point& p) : m_Data(p.x, p.y, 1.f){}

    template<bool RHS>
    Vec3<RHS | POINTVECTOR> operator+ (const Vec3<RHS>& rhs) const{
        Vec3<RHS | POINTVECTOR> tmp;
        tmp.m_Data = this->m_Data + rhs.m_Data;
        return tmp;
    }
    template<bool RHS>
    Vec3<POINTVECTOR>& operator+=(const Vec3<RHS>& rhs){
        static_assert(POINTVECTOR || !RHS, "Error: Inplace addition of Drection and Point is a Point, (Direction += Point  is not allowed)");
        this->m_Data += rhs.m_Data;
        return *this;
    }

    template<bool RHS>
    Vec3<RHS | POINTVECTOR> operator- (const Vec3<RHS>& rhs) const{
        Vec3<RHS | POINTVECTOR> tmp;
        tmp.m_Data = this->m_Data - rhs.m_Data;
        return tmp;
    }
    template<bool RHS>
    Vec3<POINTVECTOR>& operator-=(const Vec3<RHS>& rhs){
        static_assert(POINTVECTOR || !RHS, "Error: Inplace substruction of Drection and Point is a Point, (Direction -= Point  is not allowed)");
        this->m_Data -= rhs.m_Data;
        return *this;
    }

    cf::Vec3<POINTVECTOR> operator* (float rhs) const{
        cf::Vec3<POINTVECTOR> tmp = *this;
        tmp.m_Data *= rhs;
        return tmp;
    }
    cf::Vec3<POINTVECTOR>& operator*=(float rhs){
        this->m_Data *= rhs;
        return *this;
    }

    friend cf::Vec3<POINTVECTOR> operator*(float lhs, const cf::Vec3<POINTVECTOR>& vec){
        cf::Vec3<POINTVECTOR> tmp;
        tmp.m_Data = vec.m_Data * lhs;
        return tmp;
    }

    template<bool RHS>
    Vec3<RHS | POINTVECTOR>  operator% (const Vec3<RHS>& rhs) const{
        Vec3<RHS | POINTVECTOR> tmp;
        tmp.m_Data = glm::cross(this->m_Data, rhs.m_Data);
        return tmp;
    }
    template<bool RHS>
    Vec3<POINTVECTOR>& operator%=(const Vec3<RHS>& rhs){
        static_assert(POINTVECTOR || !RHS, "Error: Inplace crossproduct of Drection and Point is a Point, (Direction %= Point  is not allowed)");
        this->m_Data = glm::cross(this->m_Data, rhs.m_Data);
        return *this;
    }

    void normalize(){
        static_assert(POINTVECTOR, "Error: DirectionVector cannot be normalized!");
        this->m_Data.x /= this->m_Data.z;
        this->m_Data.y /= this->m_Data.z;
        this->m_Data.z = 1.f; // this->m_Data.z /= this->m_Data.z;
    }

    bool isPointVector() const{ return POINTVECTOR; }

    template<bool RHS>
    float operator*(const Vec3<RHS>& rhs) const{ return glm::dot(this->m_Data, rhs.m_Data); }

    float getX() const{ return this->m_Data.x; }
    float getY() const{ return this->m_Data.y; }
    float getW() const{ return this->m_Data.z; }

    void setX(float value){ this->m_Data.x = value; }
    void setY(float value){ this->m_Data.y = value; }
    void setW(float value){
        static_assert(POINTVECTOR, "Error: Write acces to DirectionVector's w component is not allowed");
        this->m_Data.z = value;
    }

    float  operator[](int idx) const { return this->m_Data[idx]; }
    float& operator[](int idx) {
        if (idx == 2 && !POINTVECTOR)
            throw std::runtime_error("Error: Write acces to DirectionVector's w component is not allowed");
        return this->m_Data[idx];
    }

    operator       glm::vec3 () const { return this->m_Data; }
    operator const glm::vec3&() const { return this->m_Data; }

    operator cf::Point () const {
        static_assert(POINTVECTOR, "Error: No convertion right from cf::DirectionVector to cf::Point, try changing type to cf::PointVector");
        return cf::Point(this->m_Data.x / this->m_Data.z, this->m_Data.y / this->m_Data.z);
    }

    cf::PointVector& operator=(const cf::Point p){
        this->m_Data[0] = p.x;
        this->m_Data[1] = p.y;
        this->m_Data[2] = 1.f;
        return *this;
    }

	cf::Vec3<POINTVECTOR>& operator=(const glm::vec3& rhs) {
		this->m_Data = rhs;
		return *this;
	}

    operator cf::Vec3<false> () const {
        if (this->m_Data.z)
            throw std::runtime_error("Error: Convertion from PointVector not possible (weight != 0)");
        return cf::Vec3<false>(this->m_Data.x, this->m_Data.y);
    }

private:
    friend struct Vec3<!POINTVECTOR>;

    template<bool b>
    friend std::ostream& (::operator<<)(std::ostream&, const Vec3<b>&);

    glm::vec3 m_Data;
};

std::vector<cf::PointVector> readDATFile(const std::string& filePath){
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

        // remove non numbers, and non . ' '
        for (size_t i = 0; i < str.size(); ++i){
            char c = str[i];
            if (c != ' ' && c != '.' && (c < '0' || c > '9')){
                std::cout << "Warning: Unknown symbol detected, ASCII code: '" << (int)str[i] << "'" << std::endl;
                str.erase(i, 1);
                --i;
            }
        }

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
            point = tmp;
            points.push_back(point);
        }
    }
    return points;
}

}

template<bool b>
std::ostream& operator<<(std::ostream& os, const cf::Vec3<b>& rhs){ return os << rhs.m_Data; }

#endif // COMPUTER_GEOMETRY_H_H
