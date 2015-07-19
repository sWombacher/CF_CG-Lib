
#ifndef _WINDOW_H_H_
#define _WINDOW_H_H_



#include <vector>
#include <string>
#include <thread>
#include <inttypes.h>

#include "utils.h"

namespace cf{

class Window3D{
public:
    Window3D(int width = 1280, int height = 720, const char* title = "chaos and fractals");
    ~Window3D();

    void init(int* argc, char** argv);
    void clear(const Color& color = Color());
    void draw() const;


    int getWindowWidth()  const;
    int getWindowHeight() const;

    void drawAxis() const;

    void drawCylinder(const glm::vec3& drawingDirection, const glm::vec3& position, const Color color = Color(255, 255, 255)) const;

private:
    int m_Width;
    int m_Height;

    const std::string m_Title;

    int m_WindowID;
};
}

#endif
