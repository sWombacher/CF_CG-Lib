%module cf
%include <stdint.i>
%include <cpointer.i>
%include <std_string.i>
%include <std_vector.i>

%{
#include "glm/vec3.hpp"
#include "IFS.h"
#include "GEN.h"
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
#include "window3DObjectbased.h"
%}

%include "glm/vec3.hpp"
%include "IFS.h"
%include "GEN.h"
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
    void setElement(size_t idx, const T& element) { *this->_get(idx) = element; }
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

template<typename T>
void print(const T& rhs) { std::cout << rhs << std::endl; }

struct Math {

template<typename Lhs, typename Rhs, typename Result>
static Result multiply(const Lhs& lhs, const Rhs& rhs) { return lhs * rhs; }

template<typename Lhs, typename Rhs, typename Result>
static Result add(const Lhs& lhs, const Rhs& rhs) { return lhs + rhs; }

template<typename Lhs, typename Rhs = Lhs, typename Result = Lhs>
static Result subtract(const Lhs& lhs, const Rhs& rhs) { return lhs - rhs; }

// single type helper
template<typename T>
static T add(const T& lhs, const T& rhs) { return add<T, T, T>(lhs, rhs); }

template<typename T>
static T subtract(const T& lhs, const T& rhs) { return subtract<T, T, T>(lhs, rhs); }

template<typename T>
static T multiply(const T& lhs, const T& rhs) { return multiply<T, T, T>(lhs, rhs); }

};


template<typename T>
struct VecBase {
    VecBase() = default;
    virtual ~VecBase() = default;
    VecBase(const T& rhs) : m_Data(rhs){}

    float at(int idx) const { return this->m_Data[idx]; }
    void setVal(int idx, float val) { this->m_Data[idx] = val; }

    void operator= (const T& rhs) { this->m_Data = rhs; }
    const T& internalFormat() const { return this->m_Data; } // conversion operator doesn't work...

protected:
    T m_Data;
};


template<typename T>
struct MatBase {
    MatBase() = default;
    ~MatBase() = default;
    MatBase(const T& rhs) : m_Data(rhs) {}

    float at(int column, int row) const { return this->m_Data[column][row]; }
    void setVal(int column, int row, float val) { this->m_Data[column][row] = val; }

    const T& internalFormat() const { return this->m_Data; } // conversion operator doesn't work...

private:
    T m_Data;
};

template<typename T> T derefPointer(const T* ptr) { return *ptr; }

%}
%template(PointVectorContainer) ContainerProxy<cf::Vec3<true, double>>;

%template(printVec3) print<glm::vec3>;
%template(printVec4) print<glm::vec4>;
%template(printMat3x3) print<glm::mat3x3>;
%template(printMat4x4) print<glm::mat4x4>;

%template(addVec3Vec3) Math::add<glm::vec3>;
%template(addVec4Vec4) Math::add<glm::vec4>;
%template(addMat3x3Mat3x3) Math::add<glm::mat3x3>;
%template(addMat4x4Mat4x4) Math::add<glm::mat4x4>;

%template(subVec3Vec3) Math::subtract<glm::vec3>;
%template(subVec4Vec4) Math::subtract<glm::vec4>;
%template(subMat3x3Mat3x3) Math::subtract<glm::mat3x3>;
%template(subMat4x4Mat4x4) Math::subtract<glm::mat4x4>;

%template(multiplyMat3x3Mat3x3) Math::multiply<glm::mat3x3>;
%template(multiplyMat4x4Mat4x4) Math::multiply<glm::mat4x4>;
%template(multiplyVec3Mat3x3) Math::multiply<glm::vec3, glm::mat3x3, glm::vec3>;
%template(multiplyVec4Mat4x4) Math::multiply<glm::vec4, glm::mat4x4, glm::vec4>;

%template(GlmVec3) VecBase<glm::vec3>;
%template(GlmVec4) VecBase<glm::vec4>;
%template(GlmMat3x3) MatBase<glm::mat3x3>;
%template(GlmMat4x4) MatBase<glm::mat4x4>;

%template(StdVectorFloat) std::vector<float>;
%template(StdVectorGlmVec3) std::vector<glm::vec3>;

%template(Dereference) derefPointer<std::string>;

%inline %{

struct Lin3DCylinder {
    float diameter;
    cf::Color color;
    VecBase<glm::vec3> position;
    VecBase<glm::vec3> direction;
};

struct WindowLin3D {
    WindowLin3D(int width = 800, int height = 600, const char* title = "chaos and fractals") {
        static std::string str = "";
        char* argv = &str[0];
        static int argc = 1;
        this->m_Window = &cf::Window3DObject::createWindow3DObject(&argc, &argv, width, height, title);
        assert(this->m_Window);
        this->m_Window->setDrawingFunction([this](cf::Window3DObject& window) {
            window.clear();
            window.drawAxis();

            std::lock_guard<std::mutex> lock(this->m_Mutex);
            for (const auto& e : this->m_Cylinders)
                window.drawCylinder(e.direction.internalFormat(), e.position.internalFormat(), e.diameter, e.color);
        });
    }
    void setCylinders(const std::vector<Lin3DCylinder>& cylinders) {
        std::lock_guard<std::mutex> lock(this->m_Mutex);
        this->m_Cylinders = cylinders;
    }
    const std::vector<Lin3DCylinder>& getCylinders() const { return this->m_Cylinders; }

    void waitKeyPressed(size_t delay = 0) { this->m_Window->waitKeyPressed(delay); }

  private:
    std::mutex m_Mutex;
    cf::Window3DObject* m_Window = nullptr;
    std::vector<Lin3DCylinder> m_Cylinders;
};

%}
%template(LinCylinderVec) std::vector<Lin3DCylinder>;
