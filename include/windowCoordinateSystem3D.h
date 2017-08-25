#ifndef WINDOW_COORDINATE_SYSTEM_3D_H_H
#define WINDOW_COORDINATE_SYSTEM_3D_H_H

#include "window3D.h"
#include <mutex>
#include <thread>

namespace cf {

struct WindowCoordinateSystem3D : protected Window3D {
    // thread safty:
    // every functin has to start with (some execptions)
    //  std::lock_guard<std::mutex> guard(this->m_Mutex);

    WindowCoordinateSystem3D(int* argc, char** argv, const Interval& interval = {-1.5, 1.5}, int width = 800,
                             int height = 600, const char* title = "chaos and fractals");

    void drawPlane(const glm::vec4& vec, const cf::Color& color = cf::Color::RED, uint8_t alpha = 127);
    void drawPlane(const glm::vec3& normal, const glm::vec3& point, const cf::Color& color = cf::Color::RED, uint8_t alpha = 127);

    void drawPoint(const glm::vec3& pos, const cf::Color& color = cf::Color::BLACK, uint8_t alpha = 255, float radius = 0.05f);
    void drawLine(const glm::vec3 &point, const glm::vec3 &dir, const cf::Color& color = cf::Color::BLUE, float lineThickness = 3.f);
    void drawSphere(const glm::vec3& center, float radius, uint8_t alpha = 255, const cf::Color& color = cf::Color::GREEN);
    void drawCircle(const glm::vec3& center, const glm::vec3 normal, float radius, const cf::Color& color = cf::Color::GREY, float lineThickness = 5.f);

    void clearWindow(const cf::Color& color = cf::Color::WHITE);

    unsigned char waitKey();

    template<typename _Function>
    int beginDrawing(_Function&& f){
        std::thread thread(std::forward<_Function>(f));
        const int res = this->startDrawing();
        thread.join();
        return res;
    }

  private:
    Interval m_Interval;

    std::mutex m_WriteMutex;

    unsigned char m_KeyPressed;
    std::mutex m_KeyEventMutex;

    void draw() override;
    void handleKeyboardInput(unsigned char key, int, int) override;

    template <typename _VType>
    static bool _CmpZero(const _VType& value) { return std::abs(value) < _VType(0.000001); }

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
