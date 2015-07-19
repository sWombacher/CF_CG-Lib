
#ifndef __WINDOW_2D_H_H__
#define __WINDOW_2D_H_H__

#include <opencv2/opencv.hpp>
#include "utils.h"


namespace cf{

struct Point{
    Point(float val_x, float val_y):x(val_x), y(val_y){}
    float x;
    float y;
};

class Window2D{
public:
    Window2D(uint width = 800, uint height = 600, const char* windowName = "Chaos and Fractals");
    ~Window2D();

    void show() const;
    void waitKey() const;

    void waitMouseInput(float& x, float& y);

    void  setScale(float scale);
    float getScale() const;

    void setInvertYAxis(bool invert);
    bool getInvertYAxis() const;

    void  setColor(float x, float y, const Color& color);
    const Color& getColor(float x, float y) const;

    void drawCircle   (cf::Point point1,      uint radius, int lineWidth, const cf::Color& color);
    void drawRectangle(cf::Point point1, cf::Point point2, int lineWidth, const cf::Color& color);
    void drawLine     (cf::Point point1, cf::Point point2, int lineWidth, const cf::Color& color);


    void setNewIntervall(const cf::Intervall& intervallX, const cf::Intervall& intervallY);
    void  resetIntervall();


    int getImageWidth() const;
    int getImageHeight()const;
    cv::Mat& getImage();

protected:
    template<typename T>
    void _correctYValue(T& y) const;

    template<typename T>
    void _convertFromNewIntervall(T& x, T& y) const;

    template<typename T>
    void _convertToNewIntervall(T& x, T& y) const;

    cv::Mat m_Image;

    bool  m_InvertYAxis;
    float m_Scale;
    const char* m_WindowName;

    cf::Intervall m_IntervallX;
    cf::Intervall m_IntervallY;

    float m_MouseCallBackData[2];
};


}

#endif
