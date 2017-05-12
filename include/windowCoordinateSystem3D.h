#ifndef WINDOW_COORDINATE_SYSTEM_3D_H_H
#define WINDOW_COORDINATE_SYSTEM_3D_H_H

#include <mutex>
#include <thread>
#include "window3D.h"

namespace cf{

struct WindowCoordinateSystem3D : protected Window3D {
    // thread safty:
    // every functin has to start with
    //  std::lock_guard<std::mutex> guard(this->m_Mutex);


    WindowCoordinateSystem3D(int* argc, char** argv, const Interval& interval = {-100.0,100.0}, int width = 800, int height = 600, const char* title = "chaos and fractals");

    void drawPlane(const glm::vec4& vec, const cf::Color& color = cf::Color::RED, uint8_t alpha = 64);
    void drawLine(const glm::vec4& point, const glm::vec4& dir, const cf::Color& color = cf::Color::BLUE, float size = 0.1f);
    void drawPoint(const glm::vec4& point, const cf::Color& color = cf::Color::GREEN, float size = 1.f);

    void clearWindow(const cf::Color& color = cf::Color::WHITE);

    unsigned char waitKey();

    void waitFinish();

private:
    Interval m_Interval;

    std::mutex m_WriteMutex;

    unsigned char m_KeyPressed;
    std::mutex m_KeyEventMutex;

    std::thread m_DrawingThread;

    void draw() override;
    void handleKeyboardInput(unsigned char key, int, int) override;

    template<typename _VType>
    static bool _CmpZero(const _VType& value){ return std::abs(value) < _VType(0.000001); }

    static bool _CmpVec3(const glm::vec3& v0, const glm::vec3& v1);

    struct Line{
        cf::Color color;
        glm::vec3 point0;
        glm::vec3 point1;
        float size;
    };
    struct Point{
        cf::Color color;
        glm::vec3 pos;
        float size;
    };
    struct Plane{
        char alpha;
        cf::Color color;
        glm::vec3 normal;
        std::vector<glm::vec3> points;
    };

    std::vector<Plane> m_Planes;
    std::vector<Point> m_Points;
    std::vector<Line> m_Lines;

    cf::Color m_BackgroundColor = cf::Color::WHITE;
};

}

#endif
