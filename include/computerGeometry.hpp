#ifndef COMPUTER_GEOMETRY_H_H
#define COMPUTER_GEOMETRY_H_H

#include "windowCoordinateSystem.hpp"

namespace cf{ struct Vec3; }
std::ostream& operator<<(std::ostream& os, const cf::Vec3& rhs);

namespace cf{
struct PointVector;
std::vector<cf::PointVector> readDATFile(const std::string& filePath);

struct Vec3{
    Vec3(bool pointVector, float x = 0.f, float y = 0.f, float w = 0.f);

    Vec3  operator+ (const Vec3& rhs) const;
    Vec3& operator+=(const Vec3& rhs);

    Vec3  operator- (const Vec3& rhs) const;
    Vec3& operator-=(const Vec3& rhs);

    cf::Vec3  operator* (float rhs) const;
    cf::Vec3& operator*=(float rhs);

    friend cf::Vec3 operator*(float lhs, const cf::Vec3& vec);

    Vec3  operator% (const Vec3& rhs) const;
    Vec3& operator%=(const Vec3& rhs);

    void normalize();
    bool isPointVector() const;
    float scalarProduct(const Vec3& rhs) const;

    float getX() const;
    float getY() const;
    float getW() const;

    void setX(float value);
    void setY(float value);
    void setW(float value);

    float& operator[](int idx);

    operator glm::vec3& ();
    operator cf::Point () const;

private:
    friend std::ostream& (::operator<<)(std::ostream&, const Vec3&);
    friend std::vector<cf::PointVector> readDATFile(const std::string&);
    const bool m_PointVector;
    glm::vec3 m_Data;
};

struct PointVector     : public Vec3{ PointVector    (float x = 0.f, float y = 0.f) : PointVector::Vec3(true , x, y, 1.f){} };
struct DirectionVector : public Vec3{ DirectionVector(float x = 0.f, float y = 0.f) : PointVector::Vec3(false, x, y, 0.f){} };

}

#endif // COMPUTER_GEOMETRY_H_H
