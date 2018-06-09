%module cf
%include <stdint.i>
%include <std_string.i>

%{
#include "IFS.h"
#include "ORB.h"
#include "utils.h"
#include "LSystem.h"
#include "window3D.h"
#include "window3DObjectbased.h"
#include "window2D.h"
#include "windowRasterized.h"
#include "windowVectorized.h"
#include "computerGeometry.hpp"
#include "windowCoordinateSystem.h"
%}

%include "IFS.h"
%include "ORB.h"
%include "utils.h"
%include "LSystem.h"
%include "window3D.h"
%include "window3DObjectbased.h"
%include "window2D.h"
%include "windowRasterized.h"
%include "windowVectorized.h"
%include "computerGeometry.hpp"
%include "windowCoordinateSystem.h"

// long double provided some problems in python
%extend cf::Vec3<true, double> {
    cf::Vec3<true, double> add(const cf::Vec3<true , double>& rhs){ return (*self) + rhs; }
    cf::Vec3<true, double> add(const cf::Vec3<false, double>& rhs){ return (*self) + rhs; }
    cf::Vec3<true, double> sub(const cf::Vec3<true, double>& rhs){ return (*self) - rhs; }
    cf::Vec3<true, double> sub(const cf::Vec3<false, double>& rhs){ return (*self) - rhs; }
    double innerProduct(const cf::Vec3<true, double>& rhs){ return (*self) * rhs; }
    double innerProduct(const cf::Vec3<false, double>& rhs){ return (*self) * rhs; }
    cf::Vec3<true, double> crossProduct(const cf::Vec3<true, double>& rhs){ return (*self) % rhs; }
    cf::Vec3<true, double> crossProduct(const cf::Vec3<false, double>& rhs){ return (*self) % rhs; }
    cf::Vec3<true, double> multiply(const double& rhs) { return (*self) * rhs; }

    cf::Vec3<true, double>(const cf::Vec3<false, double>& rhs){
        return new cf::Vec3<true, double>(rhs.getX(), rhs.getY(), 0.0);
    }
}
%extend cf::Vec3<false, double> {
    cf::Vec3<true, double> add(const cf::Vec3<true , double>& rhs){ return (*self) + rhs; }
    cf::Vec3<false, double> add(const cf::Vec3<false, double>& rhs){ return (*self) + rhs; }
    cf::Vec3<true, double> sub(const cf::Vec3<true, double>& rhs){ return (*self) - rhs; }
    cf::Vec3<false, double> sub(const cf::Vec3<false, double>& rhs){ return (*self) - rhs; }
    double innerProduct(const cf::Vec3<true, double>& rhs){ return (*self) * rhs; }
    double innerProduct(const cf::Vec3<false, double>& rhs){ return (*self) * rhs; }
    cf::Vec3<true, double> crossProduct(const cf::Vec3<true, double>& rhs){ return (*self) % rhs; }
    cf::Vec3<true, double> crossProduct(const cf::Vec3<false, double>& rhs){ return (*self) % rhs; }
    cf::Vec3<false, double> multiply(const double& rhs) { return (*self) * rhs; }

    cf::Vec3<false, double>(const cf::Vec3<true, double>& rhs){
        if (std::abs(rhs.getW()) > 0.000001)
            throw std::runtime_error("Error: Unable to convert point vector to direction vector!");
        return new cf::Vec3<false, double>(rhs.getX(), rhs.getY());
    }
}

%extend cf::WindowCoordinateSystem {
    void drawLinearEquation(const cf::Point& pointVector, const cf::Vec3<false, double>& drawingDirection,
                            const cf::Color& color = cf::Color::BLACK,
                            cf::Window2D::LineType type = cf::Window2D::LineType::DEFAULT, int lineWidth = 1) {
        self->drawLinearEquation(pointVector, drawingDirection, color, type, lineWidth);
    }

    void drawLinearEquation(const cf::Vec3<true, double>& vec, const cf::Color& color = cf::Color::BLACK,
                            cf::Window2D::LineType type = cf::Window2D::LineType::DEFAULT, int lineWidth = 1) {
        self->drawLinearEquation(vec, color, type, lineWidth);
    }
}

%extend cf::Point {
    Point(const cf::Vec3<true, double>& vec) {
        const double x = vec.getX() / vec.getW();
        const double y = vec.getY() / vec.getW();
        return new cf::Point(x, y);
    }
}
%template(PointVector) cf::Vec3<true, double>;
%template(DirectionVector) cf::Vec3<false, double>;


%inline %{
template<typename T>
struct ContainerProxy {
    ContainerProxy(const std::vector<T>& data) : m_Data(data){}
    const T& get(size_t idx) { return *this->_get(idx); }
    void set(size_t idx, const T& element) { *this->_get(idx) = element; }
    void push_back(const T& element) { this->m_Data.push_back(element); }
    void erase(size_t idx) { this->m_Data.erase(this->_get(idx)); }
    size_t size() const { return this->m_Data.size(); }
private:
    typename std::vector<T>::iterator _get(size_t idx) {
        if (idx >= this->m_Data.size())
            throw std::runtime_error("Out of bound");
        auto itr = this->m_Data.begin();
        std::advance(itr, idx);
        return itr;
    }
    std::vector<T> m_Data;
};
ContainerProxy<cf::Vec3<true, double>> readDatFilePointVector(const std::string& filePath)  {
    return { cf::readDATFile<cf::Vec3<true, double>>(filePath) };
}
%}
%template(PointVectorContainer) ContainerProxy<cf::Vec3<true, double>>;
