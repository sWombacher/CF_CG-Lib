#ifndef COMPUTER_GEOMETRY_H_H
#define COMPUTER_GEOMETRY_H_H

#include "windowCoordinateSystem.hpp"
#include "utils.h"
#include <sstream>
#include <fstream>
#include <string>


namespace cf {
    template<bool IS_POINTVECTOR, typename _ValueType> class Vec3;

    typedef Vec3<true , float> PointVector_f;
    typedef Vec3<false, float> DirectionVector_f;

    typedef Vec3<true , double> PointVector_d;
    typedef Vec3<false, double> DirectionVector_d;

    typedef Vec3<true , long double> PointVector_ld;
    typedef Vec3<false, long double> DirectionVector_ld;

    /**
     * @brief PointVector Specialiaztion of general Vec3
     */
    typedef PointVector_f PointVector;

    /**
     * @brief DirectionVector Specialiaztion of general Vec3, where component 'w' may not be written to
     */
    typedef DirectionVector_f DirectionVector;
}

/**
 * @brief operator<< Simple shift operator for output
 * @param os Outputstream, e.g. std::cout
 * @param rhs cf::PointVector or cf::DirectionVector
 * @return
 */
template<bool b, typename _VType>
std::ostream& operator<<(std::ostream& os, const cf::Vec3<b, _VType>& rhs);



namespace cf{

/**
 * @brief The Vec3 struct General class for vector operations
 *
 * it porvides:
 *  - conversions from/to cf::Point and glm::vec3
 *  - Cross product ('operator%') and dot product ('operator*') with other vectors
 *  - Support for DirectionVectors and PointVectors (see typedef 'PointVector' and 'DirectionVector')
 */
template<bool IS_POINTVECTOR, typename _ValueType>
class Vec3{
    typedef Vec3<IS_POINTVECTOR, _ValueType> MY_TYPE;

    // technically glm isn't required
    // union{ struct{ _ValueType x, y, w; }; _ValueType[3]; }
    // is enough
    // however, in this case you have to deal with compiler warnings =)
    // therefore using glm is easier
    typedef glm::tvec3<_ValueType, glm::precision::highp> glmVec3;
public:
    typedef _ValueType value_type;
    Vec3(const _ValueType& x = 0.0, const _ValueType& y = 0.0) : m_Data(x, y, IS_POINTVECTOR ? 1.0 : 0.0) {}
    Vec3(const _ValueType& x, const _ValueType& y, const _ValueType& w) : m_Data(x, y, w) {
        if (!IS_POINTVECTOR && !MY_TYPE::_EqualZero(w))
            throw std::runtime_error("Error: Direction vectors 'w' has to be set");
    }
    Vec3(const cf::Point& p) : m_Data(p.x, p.y, 1.0){
        static_assert(IS_POINTVECTOR, "Error: Direction vector cannot be initialized from a cf::Point");
    }

    template<bool PV_RHS, typename _VType>
    auto operator+ (const Vec3<PV_RHS, _VType>& rhs) const{
        Vec3<PV_RHS | IS_POINTVECTOR, decltype(_ValueType(0) + _VType(0))> tmp;
        for (size_t i = 0; i < 3; ++i)
            tmp[i] = this->m_Data[i] + rhs.m_Data[i];
        return tmp;
    }
    template<bool PV_RHS, typename _VType>
    MY_TYPE& operator+=(const Vec3<PV_RHS, _VType>& rhs){
        static_assert(IS_POINTVECTOR || !PV_RHS, "Error: Inplace addition of drection and point vector is a point vector, (Direction += Point  is not allowed)");
        for (size_t i = 0; i < 3; ++i)
            this->m_Data[i] += _ValueType(rhs.m_Data[i]);
        return *this;
    }

    template<bool PV_RHS, typename _VType>
    auto operator- (const Vec3<PV_RHS, _VType>& rhs) const{
        Vec3<PV_RHS | IS_POINTVECTOR, decltype(_ValueType(0) - _VType(0))> tmp;
        for (size_t i = 0; i < 3; ++i)
            tmp[i] = this->m_Data[i] - rhs.m_Data[i];
        return tmp;
    }
    template<bool PV_RHS, typename _VType>
    MY_TYPE& operator-=(const Vec3<PV_RHS, _VType>& rhs){
        static_assert(IS_POINTVECTOR || !PV_RHS, "Error: Inplace substruction of drection and point vector is a point vector, (Direction -= Point  is not allowed)");
        for (size_t i = 0; i < 3; ++i)
            this->m_Data[i] -= _ValueType(rhs.m_Data[i]);
        return *this;
    }

    /**
     * @brief operator* Multiplys each component of the vector with a factor
     * @param rhs Factor for the multiplication
     * @return Multiplied vector
     */
    MY_TYPE operator* (const _ValueType& rhs) const{
        MY_TYPE tmp = *this;
        tmp.m_Data *= rhs;
        return tmp;
    }
    MY_TYPE& operator*=(const _ValueType& rhs){
        this->m_Data *= rhs;
        return *this;
    }

    friend MY_TYPE operator*(const _ValueType& lhs, const MY_TYPE& vec){
        return vec * lhs;
    }

    /**
     * @brief operator% Performs the cross product between two vectors
     * @param rhs Second operand for cross product
     * @return
     */
    template<bool PV_RHS, typename _VType>
    auto operator% (const Vec3<PV_RHS, _VType>& rhs) const{
        Vec3<PV_RHS | IS_POINTVECTOR, decltype(_ValueType(0) * _ValueType(0) - _ValueType(0))> tmp = *this;
        tmp %= rhs;
        return tmp;
    }
    template<bool PV_RHS, typename _VType>
    MY_TYPE& operator%=(const Vec3<PV_RHS, _VType>& rhs){
        static_assert(IS_POINTVECTOR || !PV_RHS, "Error: Inplace crossproduct of drection and point vector is a point vector, (Direction %= Point  is not allowed)");
        const glmVec3 md = this->m_Data; // copy required!
        this->m_Data[0] = _ValueType(md[1] * rhs.m_Data[2] - md[2] * rhs.m_Data[1]);
        this->m_Data[1] = _ValueType(md[2] * rhs.m_Data[0] - md[0] * rhs.m_Data[2]);
        this->m_Data[2] = _ValueType(md[0] * rhs.m_Data[1] - md[1] * rhs.m_Data[0]);
        return *this;
    }

    /**
     * @brief normalize Normalizes the PointVector (division by the 'w' component), compile error on DirectionVecotrs
     * @return Return the normalized vector
     */
    MY_TYPE& normalize(){
        static_assert(IS_POINTVECTOR, "Error: direction vector cannot be normalized!");
        if (MY_TYPE::_EqualZero(this->m_Data.z))
            throw std::runtime_error("Error: point vector cannot be normalized (w is 0)");

        this->m_Data.x /= this->m_Data.z;
        this->m_Data.y /= this->m_Data.z;
        this->m_Data.z = _ValueType(1.0); // this->m_Data.z /= this->m_Data.z;
        return *this;
    }

    /**
     * @brief isPointVector Checks wether a Vector is a PointVector or DirectionVector
     * @return
     */
    bool isPointVector() const{ return IS_POINTVECTOR; }

    /**
     * @brief operator* Performs the dot product between two vectors
     * @param rhs Second operand for dot product
     * @return
     */
    template<bool PV_RHS, typename _VType>
    auto operator*(const Vec3<PV_RHS, _VType>& rhs) const{
        const auto& md = this->m_Data;
        const auto& rd = rhs.m_Data;
        return md[0] * rd[0] + md[1] * rd[1] + md[2] * rd[2];
    }

    /**
     * @brief getX Read access to component 'x'
     * @return
     */
    const _ValueType& getX() const{ return this->m_Data.x; }
    /**
     * @brief getY Read access to component 'y'
     * @return
     */
    const _ValueType& getY() const{ return this->m_Data.y; }
    /**
     * @brief getW Read access to component 'w'
     * @return
     */
    const _ValueType& getW() const{ return this->m_Data.z; }

    /**
     * @brief setX Write to component 'x'
     * @param value
     */
    void setX(const _ValueType& value){ this->m_Data.x = value; }
    /**
     * @brief setY Write to component 'y'
     * @param value
     */
    void setY(const _ValueType& value){ this->m_Data.y = value; }
    /**
     * @brief setW Write to component 'w', compile error on DirectionVectors
     * @param value
     */
    void setW(const _ValueType& value){
        static_assert(IS_POINTVECTOR, "Error: Write acces to direction vector's w component is not allowed");
        this->m_Data.z = value;
    }

    /**
     * @brief operator[]  Access to each component of the Vector, Note: read access is granted to all components (including index 2)
     * @param idx Acess index
     * @return
     */
    const _ValueType& operator[](int idx) const { return this->m_Data[idx]; }

    /**
     * @brief operator[] Access to each component of the Vector, Note: no write access for index 2 on DirectionVectors
     * @param idx Acess index, idx = 0 -> x, idx = 1 -> y, idx = 2 -> w
     * @return
     */
    _ValueType& operator[](int idx) {
        if (idx == 2 && !IS_POINTVECTOR)
            throw std::runtime_error("Error: Write acces to direction vector's w component is not allowed");
        return this->m_Data[idx];
    }

    operator glm::vec3 () const { return { float(this->m_Data[0]), float(this->m_Data[1]), float(this->m_Data[2]) }; }
    operator const glmVec3&() const { return this->m_Data; }

    /**
     * @brief operator cf::Point Conversion operator to cf::Point, compile error on DirectionVectors
     */
    operator cf::Point () const {
        static_assert(IS_POINTVECTOR, "Error: No convertion from direction vector to cf::Point, try changing type to point vector");
        if (MY_TYPE::_EqualZero(this->m_Data.z)){
            std::cerr << "Warning: Normalizing point vector with w = 0  -> point at infinity" << std::endl;
            return { std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity() };
        }
        return cf::Point(this->m_Data.x / this->m_Data.z, this->m_Data.y / this->m_Data.z);
    }

    MY_TYPE& operator=(const cf::Point& p){
        static_assert(IS_POINTVECTOR, "Error: No convertion from cf::Point to direction vector possible (w component has to be 0)");
        this->m_Data[0] = _ValueType(p.x);
        this->m_Data[1] = _ValueType(p.y);
        this->m_Data[2] = _ValueType(1.0);
        return *this;
    }

    template<typename _VType, glm::precision precision>
    MY_TYPE& operator=(const glm::tvec3<_VType, precision>& rhs) {
        if (!IS_POINTVECTOR && !MY_TYPE::_EqualZero(rhs.z))
            throw std::runtime_error("Error: Cannot convert glm::tvec3 to direction vector (w component has to be 0)");
        for (size_t i = 0; i < 3; ++i)
            this->m_Data[i] = _ValueType(rhs[i]);
		return *this;
	}

    /**
     * @brief Conversion operator from point vector to direction vector and vise versa, may throw an exception if 'w' is not 0 (point to direction vector)
     */
    template<bool PV_RHS, typename _VType>
    operator cf::Vec3<PV_RHS, _VType> () const {
        if (IS_POINTVECTOR && !PV_RHS && !MY_TYPE::_EqualZero(_VType(this->m_Data.z)))
            throw std::runtime_error("Error: Convertion from point vector to direction vector not possible (w != 0)");
        return { _VType(this->m_Data.x), _VType(this->m_Data.y), _VType(this->m_Data.z) };
    }

    /**
     * @brief length Calculates the vector length for Direction type vectors
     * @return Length of the underlying vector
     */
    _ValueType length() const {
        static_assert(!IS_POINTVECTOR, "Error: Length calculation only possible for direction vectors");
        return std::sqrt(this->m_Data[0] * this->m_Data[0] + this->m_Data[1] * this->m_Data[1]);
    }

private:
    template<bool b, typename _VType>
    friend class Vec3;

    template<bool b, typename _VType>
    friend std::ostream& (::operator<<)(std::ostream&, const Vec3<b, _VType>&);

    template<typename _VType>
    static bool _EqualZero(const _VType& v) { return std::abs(v) < _ValueType(0.000001); }

    glmVec3 m_Data;
};

}

template<bool b, typename _VType>
std::ostream& operator<<(std::ostream& os, const cf::Vec3<b, _VType>& rhs){ return os << static_cast<glm::vec3>(rhs); }

#endif // COMPUTER_GEOMETRY_H_H
