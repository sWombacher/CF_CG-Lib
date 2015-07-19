
#ifndef _WINDOW_H_H_
#define _WINDOW_H_H_



#include <vector>
#include <string>
#include <thread>
#include <inttypes.h>

#include "utils.h"

namespace cf{


class Window2D{

public:

    Window2D(int width = 1280, int height = 720, const char* title = "chaos and fractals");
    ~Window2D();

    //int button, int state, int x, int y
    void init(int* argc, char** argv, void (*foo)(int, int, int, int) = nullptr);

    void clear(const Color& color = Color());
    void draw() const;

    Color getPixelColor(int x, int y);
    void  setPixelColor(int x, int y, const Color& color);

    int getWindowWidth()  const;
    int getWindowHeight() const;

    /// TODO mouse position

private:

    std::thread m_Thread;

    int m_Width;
    int m_Height;

    const std::string m_Title;
    std::vector<uint8_t> m_PixelData;
    int m_WindowID;
};


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
