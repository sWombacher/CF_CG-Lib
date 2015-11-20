
#ifndef _WINDOW_H_H_
#define _WINDOW_H_H_



#include <vector>
#include <string>
#include <thread>
#include <inttypes.h>
#include <functional>

#include "utils.h"

namespace cf{


class Window3D{
public:
    Window3D(int* argc, char** argv, int width = 800, int height = 600, const char* title = "chaos and fractals");
    ~Window3D();

    enum class CameraType{
        NONE, ROTATION, STATIC_X_AXIS, STATIC_Y_AXIS, STATIC_Z_AXIS
    };

    void clear(const Color& color = Color(0, 0, 0));

    virtual void draw() = 0;

    // this function does NOT return!
    int startDrawing();

    int getWindowWidth()  const;
    int getWindowHeight() const;

    void setCamera(CameraType type, glm::vec3 lookAt = glm::vec3(0, 0, 0), float distance = 10.f);

    void drawAxis(float length = 100.f) const;
    void drawCylinder(const glm::vec3& drawingDirection, const glm::vec3& position, const Color color = Color(255, 255, 255)) const;

    void setKeyboardCallbackFunction(std::function<void(unsigned char key, int x, int y)> foo);

protected:
    friend void myKeyboardCallbackFunction(unsigned char key, int x, int y);
    float   m_DistAdjustment = 1.f;
    float  m_AngleAdjustment = 1.f;
    float m_CameraAdjustment = 1.f;

protected:
    friend void myKeyboardCallbackFunction(unsigned char key, int x, int y);

    std::function<void(unsigned char key, int x, int y)> m_AdditionalKeyboardCallback;
    void _adjustCamera();
    int m_Width;
    int m_Height;

    glm::vec3 m_LookAt;
    float m_LookAtDistance;
    float m_RotationAngle_Z;
    float m_RotationAngle_Y;

    int m_WindowID;
    CameraType m_CameraType;
    const std::string m_Title;
};
}

#endif
