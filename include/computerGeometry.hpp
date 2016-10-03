#ifndef COMPUTER_GEOMETRY_H_H
#define COMPUTER_GEOMETRY_H_H

#include "windowCoordinateSystem.hpp"
#include "utils.h"
#include <sstream>
#include <fstream>
#include <string>


namespace cf {
    template<bool POINTVECTOR> struct Vec3;

    /**
     * @brief PointVector Specialiaztion of general Vec3
     */
	typedef Vec3<true > PointVector;

    /**
     * @brief DirectionVector Specialiaztion of general Vec3, where component 'w' may not be written to
     */
	typedef Vec3<false> DirectionVector;
}

/**
 * @brief operator<< Simple shift operator for output
 * @param os Outputstream, e.g. std::cout
 * @param rhs cf::PointVector or cf::DirectionVector
 * @return
 */
template<bool b>
std::ostream& operator<<(std::ostream& os, const cf::Vec3<b>& rhs);



namespace cf{

/**
 * @brief The Vec3 struct General class for vector operations
 *
 * it porvieds:
 *  - conversions from/to cf::Point and glm::vec3
 *  - Cross product ('operator%') and dot product ('operator*') with other vectors
 *  - Support for DirectionVectors and PointVectors (see typedef 'PointVector' and 'DirectionVector')
 */
template<bool POINTVECTOR>
struct Vec3{
    Vec3(float x = 0.f, float y = 0.f) : m_Data(x, y, POINTVECTOR ? 1.f : 0.f) {}
    Vec3(float x, float y, float w) : m_Data(x, y, w) {
        if (!POINTVECTOR && w)
            throw std::runtime_error("Error: DirectionVectors 'w' component may not be set");
    }
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

    /**
     * @brief operator* Multiplys each component of the vector with a factor
     * @param rhs Factor for the multiplication
     * @return Multiplied vector
     */
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

    /**
     * @brief operator% Performs the cross product between two vectors
     * @param rhs Second operand for cross product
     * @return
     */
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

    /**
     * @brief normalize Normalizes the PointVector (division by the 'w' component),
     * compile error on DirectionVecotrs
     */
    void normalize(){
        static_assert(POINTVECTOR, "Error: DirectionVector cannot be normalized!");
        if (this->m_Data.z == 0.f){
            std::cout << "WARNING: PointVector cannot be normalized (w is 0)" << std::endl;
            return;
        }
        this->m_Data.x /= this->m_Data.z;
        this->m_Data.y /= this->m_Data.z;
        this->m_Data.z = 1.f; // this->m_Data.z /= this->m_Data.z;
    }

    /**
     * @brief isPointVector Checks wether a Vector is a PointVector or DirectionVector
     * @return
     */
    bool isPointVector() const{ return POINTVECTOR; }

    /**
     * @brief operator* Performs the dot product between two vectors
     * @param rhs Second operand for dot product
     * @return
     */
    template<bool RHS>
    float operator*(const Vec3<RHS>& rhs) const{ return glm::dot(this->m_Data, rhs.m_Data); }

    /**
     * @brief getX Read access to component 'x'
     * @return
     */
    float getX() const{ return this->m_Data.x; }
    /**
     * @brief getY Read access to component 'y'
     * @return
     */
    float getY() const{ return this->m_Data.y; }
    /**
     * @brief getW Read access to component 'w'
     * @return
     */
    float getW() const{ return this->m_Data.z; }

    /**
     * @brief setX Write to component 'x'
     * @param value
     */
    void setX(float value){ this->m_Data.x = value; }
    /**
     * @brief setY Write to component 'y'
     * @param value
     */
    void setY(float value){ this->m_Data.y = value; }
    /**
     * @brief setW Write to component 'w', compile error on DirectionVectors
     * @param value
     */
    void setW(float value){
        static_assert(POINTVECTOR, "Error: Write acces to DirectionVector's w component is not allowed");
        this->m_Data.z = value;
    }

    /**
     * @brief operator[]  Access to each component of the Vector, Note: read access is granted to all components (including index 2)
     * @param idx Acess index
     * @return
     */
    float  operator[](int idx) const { return this->m_Data[idx]; }

    /**
     * @brief operator[] Access to each component of the Vector, Note: no write access for index 2 on DirectionVectors
     * @param idx Acess index, idx = 0 -> x, idx = 1 -> y, idx = 2 -> w
     * @return
     */
    float& operator[](int idx) {
        if (idx == 2 && !POINTVECTOR)
            throw std::runtime_error("Error: Write acces to DirectionVector's w component is not allowed");
        return this->m_Data[idx];
    }

    operator       glm::vec3 () const { return this->m_Data; }
    operator const glm::vec3&() const { return this->m_Data; }

    /**
     * @brief operator cf::Point Conversion operator to cf::Point, compile error on DirectionVectors
     */
    operator cf::Point () const {
        static_assert(POINTVECTOR, "Error: No convertion right from cf::DirectionVector to cf::Point, try changing type to cf::PointVector");
        return cf::Point(this->m_Data.x / this->m_Data.z, this->m_Data.y / this->m_Data.z);
    }

    cf::PointVector& operator=(const cf::Point& p){
        this->m_Data[0] = p.x;
        this->m_Data[1] = p.y;
        this->m_Data[2] = 1.f;
        return *this;
    }

	cf::Vec3<POINTVECTOR>& operator=(const glm::vec3& rhs) {
		this->m_Data = rhs;
		return *this;
	}

    /**
     * @brief operator cf::DirectionVector Conversion operator from PointVector to DirectionVector, exception if 'w' is not 0
     */
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

}

template<bool b>
std::ostream& operator<<(std::ostream& os, const cf::Vec3<b>& rhs){ return os << rhs.m_Data; }

#endif // COMPUTER_GEOMETRY_H_H
