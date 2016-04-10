
#ifndef WINDOW_2D_H_H
#define WINDOW_2D_H_H

#include <opencv2/opencv.hpp>
#include "utils.h"


namespace cf{

struct Point;

class Window2D{
public:
    Window2D(int width = 800, int height = 600, const char* windowName = "Chaos and Fractals");
    Window2D(const char* filename);
    ~Window2D();

    void show() const;
    unsigned char waitKey(int delay = 0) const;

    void waitMouseInput(float& x, float& y);

    void  setWindowScale(float scale);
    float getWindowScale() const;

    void setInvertYAxis(bool invert);
    bool getInvertYAxis() const;

    void  setColor(float x, float y, const Color& color);
    Color getColor(float x, float y) const;

    void drawCircle   (cf::Point point ,       int radius, int lineWidth, const cf::Color& color);
    void drawRectangle(cf::Point point1, cf::Point point2, int lineWidth, const cf::Color& color);
    void drawLine     (cf::Point point1, cf::Point point2, int lineWidth, const cf::Color& color);


    void setNewIntervall(const cf::Intervall& intervallX, const cf::Intervall& intervallY);
    void  resetIntervall();


    void saveImage(const char* filename) const;


    int getImageWidth() const;
    int getImageHeight()const;
    cv::Mat& getImage();

private:
    template<typename T>
    void _correctYValue(T& y) const;

    template<typename T>
    void _convertFromNewIntervall(T& x, T& y) const;

    template<typename T>
    void _convertToNewIntervall(T& x, T& y) const;

    cv::Mat m_Image;

    bool  m_InvertYAxis;
    const char* m_WindowName;
    float m_WindowScale;

    cf::Intervall m_IntervallX;
    cf::Intervall m_IntervallY;

    float m_MouseCallBackStorage[2];
    bool m_IntervallChanged = false;
};


// Note: cv::Point only alows int positions NOT floatingpoint
struct Point{
    Point(float val_x, float val_y):x(val_x), y(val_y){}
    float x;
    float y;

    bool operator==(const Point& p) const;
    bool operator!=(const Point& p) const;

    Point  operator+ (const Point& p) const;
    Point& operator+=(const Point& p);

    Point  operator- (const Point& p) const;
    Point& operator-=(const Point& p);

    Point  operator* (float factor) const;
    Point& operator*=(float factor);

    Point  operator/ (float rhs) const;
    Point& operator/=(float rhs);

    friend Point operator* (float  factor, const Point& p);
    friend Point operator/ (float lhs, const Point& p);
};


}

#endif
