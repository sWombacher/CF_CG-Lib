#ifndef WINDOW_COORDINATE_SYSTEM_3D_H_H
#define WINDOW_COORDINATE_SYSTEM_3D_H_H

#include "computerGeometry3D.hpp"
#include "window3D.h"

#include <mutex>
#include <thread>

namespace cf {

struct WindowCoordinateSystem3D : protected Window3D {
    // thread safty:
    // every functin has to start with (some execptions)
    // std::lock_guard<std::mutex> guard(this->m_WriteMutex);

    WindowCoordinateSystem3D(int* argc, char** argv, const Interval& interval = {-1.5, 1.5}, int width = 800, int height = 600,
                             const char* title = "chaos and fractals");

    void drawPlane(const glm::vec4& vec, const cf::Color& color = cf::Color::RED, uint8_t alpha = 127);
    void drawPlane(const glm::vec3& normal, const glm::vec3& point, const cf::Color& color = cf::Color::RED,
                   uint8_t alpha = 127);

    void drawPoint(const glm::vec3& pos, const cf::Color& color = cf::Color::BLACK, uint8_t alpha = 255, float radius = 0.05f);
    void drawLine(const glm::vec3& point, const glm::vec3& dir, const cf::Color& color = cf::Color::BLUE,
                  float lineThickness = 3.f);
    void drawSphere(const glm::vec3& center, float radius, uint8_t alpha = 255, const cf::Color& color = cf::Color::GREEN);
    void drawCircle(const glm::vec3& center, const glm::vec3 normal, float radius, const cf::Color& color = cf::Color::GREY,
                    float lineThickness = 5.f);

    void clearWindow(const cf::Color& color = cf::Color::WHITE);

    unsigned char waitKey();

    template <typename _Function> int beginDrawing(_Function&& f) {
        std::thread thread(std::forward<_Function>(f));
        const int res = this->startDrawing();
        thread.join();
        return res;
    }

    enum MULTI_VECTOR_TYPE { POINT, POINT_PAIR, LINE, CIRCLE, PLANE, SPHERE, UNKOWN };
    enum SPACE_TYPE { IPNS, OPNS };

    template <typename _ValueType>
    MULTI_VECTOR_TYPE getMultiVectorType(SPACE_TYPE spaceType, const cf::MultiVector<_ValueType>& mulVec) const {
        using VEC = cf::MultiVector<_ValueType>;

        const VEC dual = *mulVec;
        const VEC& ipns_vec = spaceType == SPACE_TYPE::IPNS ? mulVec : dual;
        const VEC& opns_vec = spaceType == SPACE_TYPE::OPNS ? mulVec : dual;
        bool outerproducts = false;
        glm::vec3 euclidian;
        float e0, einf;

        auto extractDataFromVector = [&](const decltype(mulVec.getData())& data) {
            euclidian = glm::vec3();
            outerproducts = false;
            einf = 0.f;
            e0 = 0.f;

            using TYPE = typename VEC::Blade::TYPE;
            for (const auto& e : data) {
                if (e.outerProduct.size()) {
                    outerproducts = true;
                    continue;
                }
                switch (e.type) {
                case TYPE::E1: euclidian.x = e.factor; break;
                case TYPE::E2: euclidian.y = e.factor; break;
                case TYPE::E3: euclidian.z = e.factor; break;
                case TYPE::E0: e0 = e.factor; break;
                case TYPE::EINF: einf = e.factor; break;
                default: break;
                }
            }
        };
        auto cmp_zero = [&](const float& v) -> bool { return cf::WindowCoordinateSystem3D::_CmpZero(v); };

        // extract ipns data
        extractDataFromVector(ipns_vec.getData());

        // point/sphere type?
        if (!outerproducts && !cmp_zero(e0)) {
            if (!cmp_zero(e0 - 1.f))
                throw std::runtime_error("Error: Sphere/Point types require e0 = 1");

            // calc radius
            einf *= 2.f;
            const float simpleInnerProduct = glm::dot(euclidian, euclidian);
            const float radius = std::sqrt(std::abs(einf - simpleInnerProduct));
            return cmp_zero(radius) ? MULTI_VECTOR_TYPE::POINT : MULTI_VECTOR_TYPE::SPHERE;
        }

        // plane type?
        if (!outerproducts && cmp_zero(e0)) {
            // check for normalized normal
            if (!cmp_zero(glm::length(euclidian) - 1.f))
                throw std::runtime_error("Error: Plane type requires normalized normal");

            return MULTI_VECTOR_TYPE::PLANE;
        }

        // point pair?
        // point pairs should have at least one element with 3 blades in outer product, -> outerProduct.size() = 2 in IPNS
        for (const auto& e : ipns_vec.getData()) {
            if (e.outerProduct.size() >= 2)
                return MULTI_VECTOR_TYPE::POINT_PAIR; /// TODO maybe check for valid point pair
        }

        // check for line/circle
        using namespace cf::literals;
        const VEC circleMid = opns_vec * 1.0_einf * opns_vec;
        extractDataFromVector(circleMid.getData());

        if (!outerproducts && !cmp_zero(einf)) {
            // circle/line type?
            // finite circleMid represent a circle
            // infinite circleMid represent a line
            const bool containsEuclidian = !cmp_zero(euclidian.x) | !cmp_zero(euclidian.y) | !cmp_zero(euclidian.z);
            const bool containsE0 = !cmp_zero(e0);
            if (containsE0)
                return MULTI_VECTOR_TYPE::CIRCLE;
            if (!containsEuclidian)
                return MULTI_VECTOR_TYPE::LINE;
        }
        return MULTI_VECTOR_TYPE::UNKOWN;
    }

    template <typename _ValueType>
    void drawMultiVector(SPACE_TYPE spaceType, const cf::MultiVector<_ValueType>& vec, const cf::Color& color, uint8_t alpha) {
        std::cout << "Drawing vector (" << (spaceType == SPACE_TYPE::IPNS ? "IPNS" : "OPNS") << "):\n"  << vec << std::endl;
        const MULTI_VECTOR_TYPE type = getMultiVectorType(spaceType, vec);
        switch (type) {
        case MULTI_VECTOR_TYPE::CIRCLE: this->_drawCircle(spaceType, vec, color, alpha); break;
        case MULTI_VECTOR_TYPE::LINE: this->_drawLine(spaceType, vec, color, alpha); break;
        case MULTI_VECTOR_TYPE::PLANE: this->_drawPlane(spaceType, vec, color, alpha); break;
        case MULTI_VECTOR_TYPE::POINT: this->_drawPoint(spaceType, vec, color, alpha); break;
        case MULTI_VECTOR_TYPE::POINT_PAIR: this->_drawPointPair(spaceType, vec, color, alpha); break;
        case MULTI_VECTOR_TYPE::SPHERE: this->_drawSphere(spaceType, vec, color, alpha); break;

        case MULTI_VECTOR_TYPE::UNKOWN: default: throw std::runtime_error("Error: Unkown Vector type");
        }
    }

  private:
    Interval m_Interval;
    std::mutex m_WriteMutex;

    unsigned char m_KeyPressed;
    std::mutex m_KeyEventMutex;

    template <typename _ValueType>
    void _drawPoint(SPACE_TYPE spaceType, const cf::MultiVector<_ValueType>& v, const cf::Color& color, uint8_t alpha) {
        if (spaceType != SPACE_TYPE::IPNS)
            throw std::runtime_error("Error: Point has to be in IPNS");

		using MultiVectorType = decltype(v);
        using namespace cf::literals;
        cf::MultiVector<_ValueType> vec;

        // search for '^einf'
        const auto iter = std::find_if(v.getData().cbegin(), v.getData().cend(), [](const auto& val){
            auto iter = std::find_if(val.outerProduct.cbegin(), val.outerProduct.cend(), [](const auto& type){
               return type == std::decay<MultiVectorType>::type::Blade::TYPE::EINF;
            });
            return iter != val.outerProduct.cend();
        });
        if (iter != v.getData().cend()){
            const auto tmp = 1.0_e0 * v;
            vec = 1.0_einf * _ValueType(0.5 * cf::abs(tmp) * cf::abs(tmp)) + tmp;
        }
        else
            vec = v;

        vec = vec / (-_ValueType(vec * 1.0_einf)); // normalize point

        glm::tvec3<_ValueType, glm::precision::highp> pos;
        const auto& blades = vec.getData();
        for (const auto& e : blades) {
            if (e.outerProduct.size())
                continue;

            using TYPE = typename cf::MultiVector<_ValueType>::Blade::TYPE;
            switch (e.type) {
            case TYPE::E1: pos.x = e.factor; break;
            case TYPE::E2: pos.y = e.factor; break;
            case TYPE::E3: pos.z = e.factor; break;
            default: break;
            }
        }
        this->drawPoint(pos, color, alpha);
    }
    template <typename _ValueType>
    void _drawSphere(SPACE_TYPE spaceType, const cf::MultiVector<_ValueType>& mulVec, const cf::Color& color, uint8_t alpha) {
        cf::MultiVector<_ValueType> dual;
        if (spaceType != SPACE_TYPE::IPNS)
            dual = *mulVec; // invert blade

        const auto& vec = spaceType == SPACE_TYPE::IPNS ? mulVec : dual;
        glm::tvec3<_ValueType, glm::precision::highp> pos;
        _ValueType einf = 0.f;
        const auto& blades = vec.getData();
        for (const auto& e : blades) {
            if (e.outerProduct.size())
                continue;

            using TYPE = typename cf::MultiVector<_ValueType>::Blade::TYPE;
            switch (e.type) {
            case TYPE::E1: pos.x = e.factor; break;
            case TYPE::E2: pos.y = e.factor; break;
            case TYPE::E3: pos.z = e.factor; break;
            case TYPE::EINF: einf = e.factor; break;
            default: break;
            }
        }
        einf *= 2.f;
        const float simpleInnerProduct = glm::dot(pos, pos);
        if (simpleInnerProduct > einf)
            alpha /= 2;

        const float radius = std::sqrt(std::abs(einf - simpleInnerProduct));
        this->drawSphere(pos, radius, alpha, color);
    }
    template <typename _ValueType>
    void _drawPointPair(SPACE_TYPE spaceType, const cf::MultiVector<_ValueType>& vec, const cf::Color& color, uint8_t alpha) {
        using namespace cf::literals;
        const cf::MultiVector<_ValueType> dual = *vec;
        const auto& v = spaceType == SPACE_TYPE::OPNS ? vec : dual;

        const auto v_squared = _ValueType(v * v);
        const auto v_einf = _ValueType(1.0_einf * v);
        const auto p0 = (v + std::sqrt(v_squared)) / v_einf;
        const auto p1 = (v - std::sqrt(v_squared)) / v_einf;

        // points are IPNS only
        std::cout << "P0: " << p0  << "\nP1: " << p1 << "\nV: " << v << '\n' << std::endl;
        std::cout << "V * V: " << v_squared << "\neinf * V: " << v_einf << std::endl;
        std::cout << "Vec^2:  " << vec * vec << std::endl;

        this->_drawPoint(SPACE_TYPE::IPNS, p0, color, alpha);
        this->_drawPoint(SPACE_TYPE::IPNS, p1, color, alpha);
    }
    template <typename _ValueType>
    void _drawLine(SPACE_TYPE spaceType, const cf::MultiVector<_ValueType>& vec, const cf::Color& color, uint8_t){
        throw std::runtime_error("_drawLine not yet implemented");
    }
    template <typename _ValueType>
    void _drawPlane(SPACE_TYPE spaceType, const cf::MultiVector<_ValueType>& mulVec, const cf::Color& color, uint8_t alpha) {
        cf::MultiVector<_ValueType> dual;
        if (spaceType != SPACE_TYPE::IPNS)
            dual = *mulVec;

        const auto& vec = spaceType == SPACE_TYPE::IPNS ? mulVec : dual;
        glm::vec3 normal;
        _ValueType einf = 0.f;
        const auto& blades = vec.getData();
        for (const auto& e : blades) {
            if (e.outerProduct.size())
                continue;

            using TYPE = typename cf::MultiVector<_ValueType>::Blade::TYPE;
            switch (e.type) {
            case TYPE::E1: normal.x = e.factor; break;
            case TYPE::E2: normal.y = e.factor; break;
            case TYPE::E3: normal.z = e.factor; break;
            case TYPE::EINF: einf = float(e.factor); break;
            default: break;
            }
        }
        const glm::vec3 pos = normal * float(-einf);
        this->drawPlane(normal, pos, color, alpha);
    }
    template <typename _ValueType>
    void _drawCircle(SPACE_TYPE spaceType, const cf::MultiVector<_ValueType>& vec, const cf::Color& color, uint8_t){
        cf::MultiVector<_ValueType> ipns = vec;
        cf::MultiVector<_ValueType> opns = *vec;
        if (spaceType != SPACE_TYPE::IPNS)
            std::swap(ipns, opns);

        glm::vec3 normal;
        for (const auto& e : ipns.getData()){
            if (e.outerProduct.size() == 1){
                // all blades are sorted -> e0 may only be on outerProduct on all e1-3
                using Type = typename std::decay<decltype(e)>::type::TYPE;
                if (Type::E0 == e.outerProduct.front()) {
                    switch(e.type){
                    case Type::E1: normal.x = e.factor; break;
                    case Type::E2: normal.y = e.factor; break;
                    case Type::E3: normal.z = e.factor; break;
                    default: break;
                    }
                }
            }
        }
        if (this->_CmpVec3(normal, glm::vec3(0.f, 0.f, 0.f)))
            throw std::runtime_error("Unable to draw circle");

        using namespace cf::literals;
        MultiVector<_ValueType> circleCenter = opns * 1.0_einf * opns;
        circleCenter = circleCenter / (-_ValueType(circleCenter * 1.0_einf)); // normalize point

        const _ValueType radius = _ValueType(opns * opns);
        const glm::vec3 center(
            float(circleCenter * 1.0_e1),
            float(circleCenter * 1.0_e2),
            float(circleCenter * 1.0_e3)
        );
        this->drawCircle(center, normal, radius, color);
    }

    void draw() override;
    void handleKeyboardInput(unsigned char key, int, int) override;

    template <typename _VType> static bool _CmpZero(const _VType& value) { return std::abs(value) < _VType(0.000001); }

    static bool _CmpVec3(const glm::vec3& v0, const glm::vec3& v1);

    struct Line {
        cf::Color color;
        glm::vec3 point0;
        glm::vec3 point1;
        float thickness;
    };
    struct Sphere {
        char alpha;
        cf::Color color;
        glm::vec3 pos;
        float radius;
    };
    struct Plane {
        char alpha;
        cf::Color color;
        glm::vec3 normal;
        std::vector<glm::vec3> points;
    };
    struct Circle {
        cf::Color color;
        glm::vec3 normal;
        glm::vec3 center;
        float radius;
        float thickness;
    };

    std::vector<Sphere> m_Spheres;
    std::vector<Circle> m_Circles;
    std::vector<Plane> m_Planes;
    std::vector<Line> m_Lines;

    cf::Color m_BackgroundColor = cf::Color::WHITE;
};
}

#endif
