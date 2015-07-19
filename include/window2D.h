
#ifndef __WINDOW_2D_H_H__
#define __WINDOW_2D_H_H__

#include <opencv2/opencv.hpp>
#include "utils.h"


namespace cf{

class Window_2D{
public:
    Window_2D(std::size_t width = 800, std::size_t height = 600, const char* windowName = "Chaos and Fractals");
    ~Window_2D();

    void show() const;
    void waitKey() const;
    void waitMouseInput(std::size_t& x, std::size_t& y) const;  /// TODO: implementation

    void  setScale(float scale);
    float getScale() const;

    void setInvertYAxis(bool invert);
    bool getInvertYAxis() const;

    void setColor(std::size_t x, std::size_t y, const Color& color);
    const Color& getColor(std::size_t x, std::size_t y) const;

    void drawCircle   (cv::Point point,std::size_t radius, int lineWidth, const cf::Color& color);
    void drawRectangle(cv::Point point1, cv::Point point2, int lineWidth, const cf::Color& color);
    void drawLine     (cv::Point point1, cv::Point point2, int lineWidth, const cf::Color& color);


    void setNewIntervall(const cf::Intervall& intervallX, const cf::Intervall& intervallY);

protected:
    template<typename T>
    inline void _getCorrectYValue(T& y);

    cv::Mat m_Image;

    bool  m_InvertYAxis;
    float m_Scale;
    const char* m_WindowName;

    cf::Intervall m_IntervallX;
    cf::Intervall m_IntervallY;
};


}

#endif
