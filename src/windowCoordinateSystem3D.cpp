#include "windowCoordinateSystem3D.h"
namespace cf {

cf::WindowCoordinateSystem3D::WindowCoordinateSystem3D(int* argc, char** argv, const cf::Interval& interval, int width,
                                                       int height, const char* title)
    : WindowCoordinateSystem3D::Window3D(argc, argv, width, height, title), m_Interval(interval) {
    Window3D::printWindowUsage();
    this->setCamera(Window3D::CameraType::ROTATION);
    this->clear();
    this->m_LookAtDistance = 2.f * (interval.max - interval.min);

    this->disableLighting();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void cf::WindowCoordinateSystem3D::drawPlane(const glm::vec4& vec, const cf::Color& color, uint8_t alpha) {
    std::lock_guard<std::mutex> guard(this->m_WriteMutex);
    WindowCoordinateSystem3D::Plane p;
    p.color = Window3D::_AdjustColorOpenGL(color);
    p.normal = glm::vec3(vec.x, vec.y, vec.z);
    p.alpha = alpha / 2; // opengl uses 0 to 127... i don't know why

    const auto& min = this->m_Interval.min;
    const auto& max = this->m_Interval.max;
    for (int idx0 = 0; idx0 < 3; ++idx0) {
        for (int idx1 = 0; idx1 < 3; ++idx1) {
            if (idx1 == idx0)
                continue;

            const auto ranges = {std::make_pair(min, min), std::make_pair(min, max), std::make_pair(max, min),
                                 std::make_pair(max, max)};
            for (const auto& e : ranges) {
                int calcIdx = -1;
                for (int i = 0; i < 3; ++i) {
                    if (i != idx0 && i != idx1) {
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
    for (size_t pointIter = 1, correctSorted = 1; correctSorted < p.points.size() - 1;) {
        bool oneComponentEqual = false;
        for (int i = 0; i < 3; ++i) {
            if (WindowCoordinateSystem3D::_CmpZero(p.points[correctSorted - 1][i] - p.points[pointIter][i])) {
                // only accept components which are either at min or max interval
                if (WindowCoordinateSystem3D::_CmpZero(p.points[correctSorted - 1][i] - min) ||
                    WindowCoordinateSystem3D::_CmpZero(p.points[correctSorted - 1][i] - max)) {
                    oneComponentEqual = true;
                    break;
                }
            }
        }
        if (oneComponentEqual) {
            std::swap(p.points[pointIter], p.points[correctSorted++]);
            pointIter = correctSorted;
        } else
            ++pointIter;
    }
    this->m_Planes.push_back(p);
}

void WindowCoordinateSystem3D::drawPlane(const glm::vec3& normal, const glm::vec3& point, const Color& color, uint8_t alpha) {
    glm::vec4 tmp(normal.x, normal.y, normal.z, 0.f);
    tmp.w = -glm::dot(normal, point);
    this->drawPlane(tmp, color, alpha);
}

void WindowCoordinateSystem3D::drawPoint(const glm::vec3& pos, const Color& color, uint8_t alpha, float radius) {
    this->drawSphere(pos, radius, alpha, color);
}

void cf::WindowCoordinateSystem3D::drawLine(const glm::vec3& point, const glm::vec3& dir, const cf::Color& color,
                                            float lineThickness) {
    std::lock_guard<std::mutex> guard(this->m_WriteMutex);
    std::vector<glm::vec3> tmp;
    {
        for (int vecIdx = 0; vecIdx < 3; ++vecIdx) {
            if (WindowCoordinateSystem3D::_CmpZero(dir[vecIdx]))
                continue;

            const auto& min = this->m_Interval.min;
            const auto& max = this->m_Interval.max;
            for (const auto& value : {min, max}) {
                const float lambda = (value - point[vecIdx]) / dir[vecIdx];
                const glm::vec3 p = point + lambda * dir;
                if (p.x >= min && p.x <= max && p.y >= min && p.y <= max && p.z >= min && p.z <= max) {
                    if (tmp.size()) {
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
    l.color = Window3D::_AdjustColorOpenGL(color);
    l.thickness = lineThickness;
    l.point0 = tmp.front();
    l.point1 = tmp.back();
    this->m_Lines.push_back(l);
}

void cf::WindowCoordinateSystem3D::drawSphere(const glm::vec3& point, float radius, uint8_t alpha, const cf::Color& color) {
    std::lock_guard<std::mutex> guard(this->m_WriteMutex);
    WindowCoordinateSystem3D::Sphere p;
    p.color = Window3D::_AdjustColorOpenGL(color);
    p.pos = point;
    p.alpha = char(alpha / 2);
    p.radius = radius;
    this->m_Spheres.push_back(p);
}

void WindowCoordinateSystem3D::drawCircle(const glm::vec3& center, const glm::vec3 normal, float radius, const Color& color, float lineThickness) {
    std::lock_guard<std::mutex> guard(this->m_WriteMutex);
    WindowCoordinateSystem3D::Circle circle;
    circle.center = center;
    circle.normal = normal;
    circle.radius = radius;
    circle.thickness = lineThickness;
    circle.color = cf::Window3D::_AdjustColorOpenGL(color);
    this->m_Circles.push_back(circle);
}

void cf::WindowCoordinateSystem3D::clearWindow(const cf::Color& color) {
    std::lock_guard<std::mutex> guard(this->m_WriteMutex);
    this->m_BackgroundColor = color;
    this->m_Planes.clear();
    this->m_Spheres.clear();
    this->m_Lines.clear();
}

unsigned char cf::WindowCoordinateSystem3D::waitKey() {
    if (!this->m_KeyEventMutex.try_lock())
        throw std::runtime_error("Error: m_KeyEventMutex already locked!");
    std::lock_guard<std::mutex> guard(this->m_KeyEventMutex);
    return this->m_KeyPressed;
}

void cf::WindowCoordinateSystem3D::draw() {
    std::lock_guard<std::mutex> guard(this->m_WriteMutex);
    this->clear(this->m_BackgroundColor);
    this->drawAxis((this->m_Interval.max - this->m_Interval.min) * 0.2f);

    // draw bounding box
    glColor3b(0, 0, 0);
    glutWireCube(double(this->m_Interval.max) * 2.0);

    // draw planes
    for (const auto& e : this->m_Planes) {
        glColor4b(char(e.color.r), char(e.color.g), char(e.color.b), e.alpha);

        // draw one side
        glBegin(GL_POLYGON);
        for (auto point : e.points)
            glVertex3f(point.x, point.y, point.z);
        glEnd();

        // draw other side
        glBegin(GL_POLYGON);
        for (size_t i = e.points.size() - 1; i < e.points.size(); --i)
            glVertex3f(e.points[i].x, e.points[i].y, e.points[i].z);
        glEnd();
    }

    // draw spheres
    for (const auto& e : this->m_Spheres) {
        const cf::Color c = Window3D::_AdjustColorOpenGL(e.color);
        glPushMatrix();
        {
            glColor4b(char(c.r), char(c.g), char(c.b), e.alpha);
            glTranslatef(e.pos.x, e.pos.y, e.pos.z);
            glScalef(e.radius, e.radius, e.radius);
            glutSolidSphere(1.0, 20, 20);
        }
        glPopMatrix();
    }

    // get line width for next iteraton
    float currentLineWidth;
    glGetFloatv(GL_LINE_WIDTH, &currentLineWidth);

    // draw lines
    for (const auto& e : this->m_Lines) {
        glLineWidth(e.thickness);
        glBegin(GL_LINES);
        {
            glColor3b(char(e.color.r), char(e.color.g), char(e.color.b));
            glVertex3f(e.point0.x, e.point0.y, e.point0.z);
            glVertex3f(e.point1.x, e.point1.y, e.point1.z);
        }
        glEnd();
    }

    // draw circles
    for (const auto& e : this->m_Circles) {
        glColor3b(char(e.color.r), char(e.color.g), char(e.color.b));
        glLineWidth(e.thickness);

        static const size_t SLICES = 20;
        glPushMatrix();
        {
            glTranslatef(e.center.x, e.center.y, e.center.z);

            static const glm::vec3 circleNormal(0.f, 0.f, 1.f);
            const float angle = cf::radian2degree(std::acos(glm::dot(e.normal, circleNormal)));
            if (!WindowCoordinateSystem3D::_CmpZero(angle)){
                const glm::vec3 rotVec = glm::cross(e.normal, circleNormal);
                glRotatef(angle, rotVec.x, rotVec.y, rotVec.z);
            }
            glBegin(GL_LINE_STRIP);
            for (float curAngle = 0.f, adjust = glm::pi<float>() * 2.f / float(SLICES); curAngle < glm::pi<float>() * 2.f; curAngle += adjust)
                glVertex3f(std::cos(curAngle), std::sin(curAngle), 0.f);
            glVertex3f(1.f, 0.f, 0.f);
            glEnd();
        }
        glPopMatrix();
    }
    glLineWidth(currentLineWidth);
}

void cf::WindowCoordinateSystem3D::handleKeyboardInput(unsigned char key, int, int) {
    this->m_KeyPressed = key;
    this->m_KeyEventMutex.unlock();
}

bool cf::WindowCoordinateSystem3D::_CmpVec3(const glm::vec3& v0, const glm::vec3& v1) {
    for (int i = 0; i < 3; ++i) {
        if (!WindowCoordinateSystem3D::_CmpZero(v0[i] - v1[i]))
            return false;
    }
    return true;
}
}
