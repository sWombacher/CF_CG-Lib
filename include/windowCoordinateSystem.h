#ifndef WINDOW_COORDINATE_SYSTEM_H_H
#define WINDOW_COORDINATE_SYSTEM_H_H

#include "window2D.h"

namespace cf {

/**
 * @brief The WindowCoordinateSystem struct Default class for images and raster operations
 */
struct WindowCoordinateSystem : protected Window2D {
    /**
     * @brief WindowCoordinateSystem Constructor
     * @param range_x Interval in x direction
     * @param range_y Interval in y direction
     * @param width Image width in pixel (hight will be determind automatically)
     */
    WindowCoordinateSystem(int width, const cf::Interval& range_x, const cf::Interval& range_y,
                           const char* windowName = "Computer Geometry", const cf::Color& startColor = cf::Color::WHITE);

    virtual ~WindowCoordinateSystem() = default;

    /**
     * @brief setInterval Set new interval
     * @param range_x Interval in x direction
     * @param range_y Interval in y direction
     * @param width Image width in pixel (hight will be determind automatically)
     */
    void setInterval(const cf::Interval& range_x, const cf::Interval& range_y, int width);

    /**
     * @brief drawPoint Draws a cross-shaped point
     * @param pos Cross position
     * @param color Cross color
     */
    void drawPoint(const cf::Point& pos, const cf::Color& color = cf::Color::BLACK, int lineWidth = 1);

    /**
     * @brief drawLine Draw a simple line of width 1
     * @param p1 First point
     * @param p2 Second point
     * @param color Line color
     * @param type Line type
     * @param lineWidth Width of the line, Note: only available on default line type
     */
    void drawLine(const cf::Point& p1, const cf::Point& p2, const cf::Color& color = cf::Color::BLACK,
                  cf::Window2D::LineType type = cf::Window2D::LineType::DEFAULT, int lineWidth = 1);

    /**
     * @brief drawLinearEquation Draws a line from a point on line and direction vector
     * @param pointVector Point on the line
     * @param drawingDirection Line direction
     * @param color Line color
     * @param type Change line type to dot/dash/dot-dash
     * @param lineWidth Width of the line, Note: only available on default line type
     */
    void drawLinearEquation(const cf::Point& pointVector, const glm::vec3& drawingDirection,
                            const cf::Color& color = cf::Color::BLACK,
                            cf::Window2D::LineType type = cf::Window2D::LineType::DEFAULT, int lineWidth = 1);

    /**
     * @brief drawLinearEquation Draw a line from a linear equation: ax + by + c = 0
     * @param a Coefficent of x
     * @param b Coefficent of y
     * @param c Constant
     * @param color Line color
     * @param type Change line type to dot/dash/dot-dash
     * @param lineWidth Width of the line, Note: only available on default line type
     */
    void drawLinearEquation(float a, float b, float c, const cf::Color& color = cf::Color::BLACK,
                            cf::Window2D::LineType type = cf::Window2D::LineType::DEFAULT, int lineWidth = 1);

    /**
     * @brief drawLinearEquation Draw line from linear equation: ax + by + c = 0, where a b and c are part of coefficent vector
     * @param vec Vector of cooefficents a b and see
     * @param color Line color
     * @param type Change line type to dot/dash/dot-dash
     * @param lineWidth Width of the line, Note: only available on default line type
     */
    void drawLinearEquation(const glm::vec3& vec, const cf::Color& color = cf::Color::BLACK,
                            cf::Window2D::LineType type = cf::Window2D::LineType::DEFAULT, int lineWidth = 1);

    /**
     * @brief drawLinearEquation Draw line from standard format y = m*x + t
     * @param slope Slope m of equation y = m*x + t
     * @param yIntercept y-Intercept t of equation y = m*x + t
     * @param color Line color
     * @param type Change line type to dot/dash/dot-dash
     * @param lineWidth Width of the line, Note: only available on default line type
     */
    void drawLinearEquation(float slope, float yIntercept, const cf::Color& color = cf::Color::BLACK,
                            cf::Window2D::LineType type = cf::Window2D::LineType::DEFAULT, int lineWidth = 1);

    /**
     * @brief drawCircle Draws a circle with interval radius
     * @param center Circle center
     * @param radius Circle radius
     * @param color Circle color
     * @param lineWidth Width of the line, Note: only available on default line type
     */
    void drawCircle(const cf::Point& center, float radius, const cf::Color& color = cf::Color::BLACK, int lineWidth = 1);

    /**
     * @brief convert_pixelLength_to_intervalLength Converts length from pixel to interval
     * @param pixelLength
     * @return
     */
    float convert_pixelLength_to_intervalLength(float pixelLength) const;

    /**
     * @brief convert_intervalLength_to_pixelLength Converts length from interval to pixel
     * @param intervalLength
     * @return
     */
    float convert_intervalLength_to_pixelLength(float intervalLength) const;

    /**
     * @brief drawCirclePart Draw a partition of a circle
     * @param center Circle center
     * @param radius Circle radius (in intervall length)
     * @param startAngle Starting angle for circle (0° -> positive x direction, 90° -> positive y direction)
     * @param endAngle End angle for circle (0° -> positive x-axis, 90° -> positive y-axis)
     * @param color Circle color
     * @param lineWidth Line width of the circle
     */
    void drawCirclePart(const cf::Point& center, float radius, float startAngle, float endAngle,
                        const cf::Color& color = cf::Color::BLACK, int lineWidth = 1);

    /**
     * @brief drawCirclePart Draw a partition of a circle
     * @param center Circle center
     * @param p0 1st point on the Circle line
     * @param p1 2nd point on the Circle line
     * @param color Circle color
     * @param lineWidth Line width
     * @param smallerAngle Choose wich part of the Circle should be drawn (default: smaller part of the circle)
     */
    void drawCirclePart(const cf::Point& center, const cf::Point& p0, const cf::Point& p1,
                        const cf::Color& color = cf::Color::BLACK, int lineWidth = 1, bool smallerAngle = true);

    // publicly available functions from class Window2D
    using Window2D::getWindowDisplayScale;
    using Window2D::setWindowDisplayScale;
    using Window2D::waitMouseInput;
    using Window2D::getIntervalX;
    using Window2D::getIntervalY;
    using Window2D::saveImage;
    using Window2D::getHeight;
    using Window2D::floodFill;
    using Window2D::getWidth;
    using Window2D::getColor;
    using Window2D::setColor;
    using Window2D::drawAxis;
    using Window2D::LineType;
    using Window2D::waitKey;
    using Window2D::clear;
    using Window2D::show;

  private:
    static int _CALCULATE_HEIGHT(const cf::Interval& range_x, const cf::Interval& range_y, int width);
    static const float ZERO_COMPARE;
};
}

#endif // WINDOW_COORDINATE_SYSTEM_H_H
