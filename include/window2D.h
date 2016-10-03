#ifndef WINDOW_2D_H_H
#define WINDOW_2D_H_H

#include <opencv2/opencv.hpp>
#include "utils.h"


namespace cf{
struct Point;

/**
 * @brief The Window2D struct offers advanced features used by WindowRasterized/WindowVertorized
 */
class Window2D{
    static const int  DOT_VALUE = 4;
    static const int DASH_VALUE = 8;

public:
    Window2D(int width = 800, int height = 600, const char* windowName = "Chaos and Fractals", const cf::Color& startColor = {0, 0, 0});
    Window2D(const char* filePath);
    virtual ~Window2D();

    /**
     * @brief show Show image, on first call it may require additional time to display content correctly (in those cases use waitKey(1000) )
     */
    void show() const;
    void clear(const cf::Color& color = cf::Color::WHITE);

    /**
     * @brief waitKey Block access until key input on window
     * @param delay Value > 0 -> wait till key input on window or 'delay'ms else wait till user input
     * @return
     */
    unsigned char waitKey(int delay = 0) const;

    /**
     * @brief waitMouseInput Blocks until mouse input has been given
     * @param x X-Window position
     * @param y Y-Window position
     */
    void waitMouseInput(float& x, float& y);

    /**
     * @brief setWindowDisplayScale Scales the image before displaying
     * @param scale Window scale size
     */
    void  setWindowDisplayScale(float scale);
    float getWindowDisplayScale() const;

    /**
     * @brief setInvertYAxis Invert y values on all 'cf::Point' functions
     * @param invert
     */
    void setInvertYAxis(bool invert);
    bool getInvertYAxis() const;

    void  setColor(float x, float y, const Color& color);
    Color getColor(float x, float y) const;

    /**
     * @brief drawCircle Draws a circle around the center
     * @param point Point within interval_x and interval_y
     * @param radius Circle radius in pixel (not effected by intervals)
     * @param lineWidth Pixelwidth of line (not effected by intervals)
     * @param color Circle color
     */
    void drawCircle(cf::Point center, int radius, int lineWidth, const cf::Color& color);

    /**
     * @brief drawRectangle Draws a rectangle from two diagonal points
     * @param point1 Point within interval_x and interval_y, has to be the diagonal point to point2
     * @param point2 Point within interval_x and interval_y, has to be the diagonal point to point1
     * @param lineWidth LineWidth pixelwidth of line (not effected by intervals)
     * @param color Rectangle color
     */
    void drawRectangle(cf::Point point1, cf::Point point2, int lineWidth, const cf::Color& color);

    /**
     * @brief drawLine Draws a line from point1 to point2
     * @param point1 Point within interval_x and interval_y
     * @param point2 Point within interval_x and interval_y
     * @param lineWidth Line width in pixel size
     * @param color Line color
     */
    void drawLine(cf::Point point1, cf::Point point2, int lineWidth, const cf::Color& color);

    /**
     * @brief The LineType enum Special line type used by one function of 'drawLine'
     */
    enum class LineType{    DEFAULT = 0 /* normal line */ ,
                              DOT_0 = Window2D::DOT_VALUE                          | 1,      DOT_1,      DOT_2,
                             DASH_0 = Window2D::DASH_VALUE                         | 1,     DASH_1,     DASH_2,
                         DOT_DASH_0 = Window2D::DOT_VALUE  | Window2D:: DASH_VALUE | 1, DOT_DASH_1, DOT_DASH_2
                       };

    /**
     * @brief drawSpecializedLine Draws specialized line of width 1 (dotted and/or dashed lines)
     * @param point1 Point within interval_x and interval_y
     * @param point2 Point within interval_x and interval_y
     * @param lineType Type of line to be drawn
     * @param color Line color
     */
    void drawSpecializedLine(cf::Point point1, cf::Point point2, LineType lineType, const cf::Color& color);

    /**
     * @brief setNewInterval Set new interval
     * @param intervalX Interval in x direction
     * @param intervalY Interval in y direction
     */
    void setNewInterval(const cf::Interval& intervalX, const cf::Interval& intervalY);
    /**
     * @brief resetInterval Set default interval (interval x: [0, image widht - 1], interval y: [0, image height - 1])
     */
    void  resetInterval();

    /**
     * @brief saveImage Saves current image to harddrive
     * @param filePath File path and name, format will be determind based on file ending (*.png, *.jpeg, ...)
     */
    void saveImage(const char* filePath) const;
    /**
     * @brief resize Resize underlying image
     * @param pixelWidth New width
     * @param pixelHeight New height
     */
    void resize(int pixelWidth, int pixelHeight);

    /**
     * @brief flippHorizontal Flipp image horizontally
     */
    void flippHorizontal();
    /**
     * @brief flippHorizontal Flipp image vertically
     */
    void flippVertical();


    /**
     * @brief getIntervalX Const access to interval in x direction
     * @return
     */
    const cf::Interval& getIntervalX() const;
    /**
     * @brief getIntervalY Const access to interval in y direction
     * @return
     */
    const cf::Interval& getIntervalY() const;

    /**
     * @brief getWidth Acess to underlying image width
     * @return Width
     */
    int getWidth() const;
    /**
     * @brief getHeight Acess to underlying image height
     * @return Height
     */
    int getHeight()const;

    /**
     * @brief getImage Direct access to the underlying image
     * @return Image handle
     */
    cv::Mat& getImage();

    /**
     * @brief drawAxis This function draws x and y axis based on Interval
     * @param color Axis color, default is white
     * @param stepSize_x Dynamially set step size (x-axis), negative numbers indicate 10 steps for interval x
     * @param stepSize_y Dynamially set step size (y-axis), negative numbers indicate 10 steps for interval y
     */
    void drawAxis(const cf::Color& color = cf::Color::BLACK, float stepSize_x = 1.f, float stepSize_y = 1.f, float interceptLength = 3.f);

    /**
     * @brief drawCriclePart Draws a part of a circle
     * @param center Center point of the circle
     * @param radius Radius of the circle
     * @param startAngle Start position (in degrees)
     * @param endAngle End position (in degrees)
     * @param color Color of the drawn line
     */
    void drawCriclePart(cf::Point center, int radius, float startAngle, float endAngle, int lineWidth, const cf::Color& color = cf::Color::BLACK);

    /**
     * @brief floodFill Fills an area
     * @param startingPoint First point to be colored
     * @param color Fill color
     */
    void floodFill(cf::Point startingPoint, const cf::Color& color);

protected:
    void _correctYValue(float& y) const;
    void _convertFromNewInterval(float& x, float& y) const;
    void _convertToNewInterval(float& x, float& y) const;

	void _window2foreground() const;

    cv::Mat m_Image;

    bool  m_InvertYAxis;
    const char* m_WindowName;
    float m_WindowScale;

    cf::Interval m_IntervalX;
    cf::Interval m_IntervalY;

    float m_MouseCallBackStorage[2];
    bool m_IntervalChanged = false;

    mutable bool m_FristShowCall = true;
};


// Note: cv::Point only alows int positions NOT floatingpoint
/**
 * @brief The Point struct is a simple class for positon access on 2D images (imilar to cv::Point, but uses floats instead of integer)
 */
struct Point{
    Point(float val_x = 0.f, float val_y = 0.f):x(val_x), y(val_y){}
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

    operator cv::Point () const;

    friend Point operator*(float lhs, const Point& p);
    friend Point operator/(float lhs, const Point& p);
};

}

#endif
