
#ifndef __WINDOW_2D_H_H__
#define __WINDOW_2D_H_H__

#include <opencv2/opencv.hpp>
#include "utils.h"


namespace cf{

struct Point;

class Window2D{
public:
    Window2D(uint width, uint height, const char* windowName = "Chaos and Fractals");
    Window2D(const char* filename);
    ~Window2D();

    void show() const;
    void waitKey() const;

    void waitMouseInput(float& x, float& y);

    void  setWindowScale(float scale);
    float getWindowScale() const;

    void setInvertYAxis(bool invert);
    bool getInvertYAxis() const;

    void  setColor(float x, float y, const Color& color);
    Color getColor(float x, float y) const;

    void drawCircle   (cf::Point point ,      uint radius, int lineWidth, const cf::Color& color);
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
    float m_WindowScale;
    const char* m_WindowName;

    cf::Intervall m_IntervallX;
    cf::Intervall m_IntervallY;

    float m_MouseCallBackStorage[2];
};


struct Point{
    Point(float val_x, float val_y):x(val_x), y(val_y){}
    float x;
    float y;

    bool operator==(Point& p){
        if (this->x == p.x && this->y == p.y)
            return true;

        return false;
    }
    bool operator!=(Point& p){
        return !(*this == p);
    }
};


}

#endif
