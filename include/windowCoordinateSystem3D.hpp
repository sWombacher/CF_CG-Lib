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


    WindowCoordinateSystem3D(int* argc, char** argv, const Interval& interval = {-100.0,100.0}, int width = 800, int height = 600, const char* title = "chaos and fractals")
        : WindowCoordinateSystem3D::Window3D(argc, argv, width, height, title), m_Interval(interval)
    {
        Window3D::printWindowUsage();
        this->setCamera(Window3D::CameraType::ROTATION);
        this->clear();
        this->m_LookAtDistance = 2.f * (interval.max - interval.min);

        static bool window_already_created = false;
        if (window_already_created)
            throw std::runtime_error("Error: Only one 3D-Window is allowed!");

        this->disableLighting();
        window_already_created = true;
        m_DrawingThread = std::thread([this](){ this->startDrawing(); });
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void drawPlane(const glm::vec4& vec, const cf::Color& color = cf::Color::RED, uint8_t alpha = 64){
        std::lock_guard<std::mutex> guard(this->m_WriteMutex);
        WindowCoordinateSystem3D::Plane p;
        p.color = Window3D::_AdjustColorOpgenGL(color);
        p.normal = vec;
        p.alpha = alpha / 2;

        const auto& min = this->m_Interval.min;
        const auto& max = this->m_Interval.max;
        for (int idx0 = 0; idx0 < 3; ++idx0){
        for (int idx1 = 0; idx1 < 3; ++idx1){
            if (idx1 == idx0)
                continue;

            for (const auto& e : { std::make_pair(min, min), std::make_pair(min, max), std::make_pair(max, min), std::make_pair(max, max) }){
                int calcIdx = -1;
                for (int i = 0; i < 3; ++i){
                    if (i != idx0 && i != idx1){
                        calcIdx = i;
                        break;
                    }
                }

                glm::vec3 point;
                point[idx0] = e.first;
                point[idx1] = e.second;
                point[calcIdx] = -1.f * (vec[3] + vec[idx0] * point[idx0] + vec[idx1] * point[idx1]) / vec[calcIdx];

                if (point[calcIdx] < min || point[calcIdx] > max || std::isinf(point[calcIdx]) || std::isnan(point[calcIdx]))
                    continue;

                bool pointAlreadyExists = false;
                for (size_t i = 0; i < p.points.size() && !pointAlreadyExists; ++i)
                    pointAlreadyExists = WindowCoordinateSystem3D::_CmpVec3(point, p.points[i]);

                if (!pointAlreadyExists)
                    p.points.push_back(point);
            }
        }
        }
        // correct point sorting
        for (size_t pointIter = 1, correctSorted = 1; correctSorted < p.points.size() - 1; ){
            bool oneComponentEqual = false;
            for (int i = 0; i < 3; ++i){
                if (WindowCoordinateSystem3D::_CmpZero(p.points[correctSorted - 1][i] - p.points[pointIter][i])) {
                    // only accept components which are either at min or max interval
                    if (WindowCoordinateSystem3D::_CmpZero(p.points[correctSorted - 1][i] - min) ||
                        WindowCoordinateSystem3D::_CmpZero(p.points[correctSorted - 1][i] - max))
                    {
                        oneComponentEqual = true;
                        break;
                    }
                }
            }
            if (oneComponentEqual){
                std::swap(p.points[pointIter], p.points[correctSorted++]);
                pointIter = correctSorted;
            }
            else
                ++pointIter;
        }
        this->m_Planes.push_back(p);
    }
    void drawLine(const glm::vec4& point, const glm::vec4& dir, const cf::Color& color = cf::Color::BLUE, float size = 0.1f){
        std::lock_guard<std::mutex> guard(this->m_WriteMutex);
        std::vector<glm::vec3> tmp;
        {
            for (int vecIdx = 0; vecIdx < 3; ++vecIdx){
                if (WindowCoordinateSystem3D::_CmpZero(dir[vecIdx]))
                    continue;

                const auto& min = this->m_Interval.min;
                const auto& max = this->m_Interval.max;
                for (const auto& value : { min, max }){
                    const float lambda = (value - point[vecIdx]) / dir[vecIdx];
                    const glm::vec3 p = point + lambda * dir;
                    if (p.x >= min && p.x <= max &&
                        p.y >= min && p.y <= max &&
                        p.z >= min && p.z <= max)
                    {
                        if (tmp.size()){
                            // check for same point
                            if (WindowCoordinateSystem3D::_CmpVec3(p, tmp.front()))
                                continue;
                        }
                        tmp.push_back(p);
                        if (tmp.size() == 2)
                            break;
                    }
                }
                if (tmp.size() == 2)
                    break;
            }
        }
        if (tmp.size() < 2)
            return; // line outside of the interval

        WindowCoordinateSystem3D::Line l;
        l.color = Window3D::_AdjustColorOpgenGL(color);
        l.size = size;
        l.point0 = tmp.front();
        l.point1 = tmp.back();
        this->m_Lines.push_back(l);
    }
    void drawPoint(const glm::vec4& point, const cf::Color& color = cf::Color::GREEN, float size = 1.f){
        std::lock_guard<std::mutex> guard(this->m_WriteMutex);
        WindowCoordinateSystem3D::Point p;
        p.color = Window3D::_AdjustColorOpgenGL(color);
        p.pos = point;
        p.size = size;
        this->m_Points.push_back(p);
    }

    void clearWindow(const cf::Color& color = cf::Color::WHITE){
        std::lock_guard<std::mutex> guard(this->m_WriteMutex);
        this->m_BackgroundColor = color;
        this->m_Planes.clear();
        this->m_Points.clear();
        this->m_Lines.clear();
    }

    unsigned char waitKey(){
        if (!this->m_KeyEventMutex.try_lock())
            throw std::runtime_error("Error: m_KeyEventMutex already locked!");
        std::lock_guard<std::mutex> guard(this->m_KeyEventMutex);
        return this->m_KeyPressed;
    }

    void waitFinish(){ this->m_DrawingThread.join(); }

private:
    Interval m_Interval;

    std::mutex m_WriteMutex;

    unsigned char m_KeyPressed;
    std::mutex m_KeyEventMutex;

    std::thread m_DrawingThread;

    void draw() override{
        std::lock_guard<std::mutex> guard(this->m_WriteMutex);
        this->clear(this->m_BackgroundColor);
        this->drawAxis((this->m_Interval.max - this->m_Interval.min) / 10.f);

        // draw bounding box
        glColor3b(0,0,0);
        glutWireCube(double(this->m_Interval.max) * 2.0);

        // draw points
        for (const auto& e : this->m_Points)
            this->drawSphere(e.pos, e.size, e.color);

        // draw lines
        glBegin(GL_LINES);
        for (const auto& e : this->m_Lines){
            glLineWidth(e.size);
            glColor3b(char(e.color.r), char(e.color.g), char(e.color.b));
            glVertex3f(e.point0.x, e.point0.y, e.point0.z);
            glVertex3f(e.point1.x, e.point1.y, e.point1.z);
        }
        glEnd();

        // draw planes
        for (const auto& e : this->m_Planes){
            glColor4b(char(e.color.r), char(e.color.g), char(e.color.b), e.alpha);

            // draw quad from both sides
            glBegin(GL_POLYGON);
            for (size_t i = 0; i < e.points.size(); ++i)
                glVertex3f(e.points[i].x, e.points[i].y, e.points[i].z);
            glEnd();
        }
    }
    void handleKeyboardInput(unsigned char key, int, int) override{
        this->m_KeyPressed = key;
        this->m_KeyEventMutex.unlock();
    }

    template<typename _VType>
    static bool _CmpZero(const _VType& value){ return std::abs(value) < _VType(0.000001); }

    static bool _CmpVec3(const glm::vec3& v0, const glm::vec3& v1){
        for (int i = 0; i < 3; ++i){
            if (!WindowCoordinateSystem3D::_CmpZero(v0[i] - v1[i]))
                return false;
        }
        return true;
    }

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
