#ifndef WINDOW_COORDINATE_SYSTEM_H_H
#define WINDOW_COORDINATE_SYSTEM_H_H

#include "window2D.h"

namespace cf{

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
                     const char* windowName = "Computer Geometry", const cf::Color& startColor = cf::Color::WHITE)
        : Window2D(1, 1, windowName, startColor)
    {
        this->setInvertYAxis(true);
        this->setInterval(range_x, range_y, width);
    }
    virtual ~WindowCoordinateSystem() = default;

    /**
     * @brief setInterval Set new interval
     * @param range_x Interval in x direction
     * @param range_y Interval in y direction
     * @param width Image width in pixel (hight will be determind automatically)
     */
    void setInterval(const cf::Interval& range_x, const cf::Interval& range_y, int width){
        this->setNewInterval(range_x, range_y);
        this->resize(width, WindowCoordinateSystem::_CALCULATE_HEIGHT(range_x, range_y, width));
    }

    /**
     * @brief drawPoint Draws a cross-shaped point
     * @param pos Cross position
     * @param color Cross color
     */
    void drawPoint(const cf::Point& pos, const cf::Color& color = cf::Color::BLACK){
        static const float pixelLength = 7.f;
        static const int lineWidth = 1;

        const float half_intervalLength = std::abs(this->convert_pixelLength_to_intervalLength(pixelLength)) / 2.f;
        cf::Window2D::drawLine({pos.x - half_intervalLength, pos.y - half_intervalLength},
                               {pos.x + half_intervalLength, pos.y + half_intervalLength}, lineWidth, color);
        cf::Window2D::drawLine({pos.x - half_intervalLength, pos.y + half_intervalLength},
                               {pos.x + half_intervalLength, pos.y - half_intervalLength}, lineWidth, color);
    }

    /**
     * @brief drawLine Draw a simple line of width 1
     * @param p1 First point
     * @param p2 Second point
     * @param color Line color
     * @param type Line type
     * @param lineWidth Width of the line, Note: only available on default line type
     */
    void drawLine(const cf::Point& p1, const cf::Point& p2, const cf::Color& color = cf::Color::BLACK,
                  cf::Window2D::LineType type = cf::Window2D::LineType::DEFAULT, int lineWidth = 1)
    {
        if (type == cf::Window2D::LineType::DEFAULT)
            cf::Window2D::drawLine(p1, p2, lineWidth, color);
        else {
            if (lineWidth > 1)
                std::cerr << "Warning: Only default line type may use lineWidth parameter" << std::endl;
            cf::Window2D::drawSpecializedLine(p1, p2, type, color);
        }
    }

    /**
     * @brief drawLinearEquation Draws a line from a point on line and direction vector
     * @param pointVector Point on the line
     * @param drawingDirection Line direction
     * @param color Line color
     * @param type Change line type to dot/dash/dot-dash
     * @param lineWidth Width of the line, Note: only available on default line type
     */
    void drawLinearEquation(const cf::Point& pointVector, const glm::vec3& drawingDirection, const cf::Color& color = cf::Color::BLACK,
                            cf::Window2D::LineType type = cf::Window2D::LineType::DEFAULT, int lineWidth = 1)
    {
        if (std::abs(drawingDirection.z) > cf::WindowCoordinateSystem::ZERO_COMPARE)
            throw std::runtime_error("Error: Direction vector may only have values != 0 onf x and y coordinates");

        if (std::abs(drawingDirection.x) < cf::WindowCoordinateSystem::ZERO_COMPARE){
            if (std::abs(drawingDirection.y) < cf::WindowCoordinateSystem::ZERO_COMPARE)
                throw std::runtime_error("Error: At least one direction component (x/y) must be set!");
            this->drawLine({pointVector.x, this->m_IntervalY.min}, {pointVector.x, this->m_IntervalY.max}, color, type, lineWidth);
        }
        else{
            float slope = drawingDirection.y / drawingDirection.x;
            float yIntercept = pointVector.y - pointVector.x * slope;
            this->drawLinearEquation(slope, yIntercept, color, type, lineWidth);
        }
    }

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
                            cf::Window2D::LineType type = cf::Window2D::LineType::DEFAULT, int lineWidth = 1)
    {
        if (std::abs(b) < cf::WindowCoordinateSystem::ZERO_COMPARE) {
            if (std::abs(a) < cf::WindowCoordinateSystem::ZERO_COMPARE){
                if (std::abs(c) < cf::WindowCoordinateSystem::ZERO_COMPARE){
                    std::cerr << "Warning: " << __func__ << " called with a=b=c=0, this is ALSWAYS TRUE for all x/y pairs" << std::endl;
                    this->clear(color);
                }
                else{
                    std::cerr << "Warning: " << __func__ << " called with with a=b=0 and c != 0, this is ALWAYS FALSE for all x/y pairs" << std::endl;
                    return;
                }
            }

            // 0*y = 0
            // <=>  ax + c = 0
            // <=>  x = -c/a
            this->drawLine({-c/a, this->m_IntervalY.min}, {-c/a, this->m_IntervalY.max}, color, type, lineWidth);
        }
        else {
            //       0 =  ax + by + c
            // <=>  by = -ax -c
            //  =>   y = -a/b x -c/b
            this->drawLinearEquation(-a/b, -c/b, color, type, lineWidth);
        }
    }

    /**
     * @brief drawLinearEquation Draw line from linear equation: ax + by + c = 0, where a b and c are part of coefficent vector
     * @param vec Vector of cooefficents a b and see
     * @param color Line color
     * @param type Change line type to dot/dash/dot-dash
     * @param lineWidth Width of the line, Note: only available on default line type
     */
    void drawLinearEquation(const glm::vec3& vec, const cf::Color& color = cf::Color::BLACK,
                            cf::Window2D::LineType type = cf::Window2D::LineType::DEFAULT, int lineWidth = 1)
    {
        this->drawLinearEquation(vec[0], vec[1], vec[2], color, type, lineWidth);
    }

    /**
     * @brief drawLinearEquation Draw line from standard format y = m*x + t
     * @param slope Slope m of equation y = m*x + t
     * @param yIntercept y-Intercept t of equation y = m*x + t
     * @param color Line color
     * @param type Change line type to dot/dash/dot-dash
     * @param lineWidth Width of the line, Note: only available on default line type
     */
    void drawLinearEquation(float slope, float yIntercept, const cf::Color& color = cf::Color::BLACK,
                            cf::Window2D::LineType type = cf::Window2D::LineType::DEFAULT, int lineWidth = 1)
    {
        cf::Point p1, p2;
        std::vector<cf::Point> points;

        // check crossing points with x min/max
        auto addPoint_horizontal = [&](float xValue){
            cf::Point point(xValue, 0.f);
            point.y = slope * point.x + yIntercept;
            if (point.y >= this->m_IntervalY.min && point.y <= this->m_IntervalY.max)
                points.push_back(point);
        };
        addPoint_horizontal(this->m_IntervalX.min);
        addPoint_horizontal(this->m_IntervalX.max);

        // check crossing points with y min/max
        auto addPoint_vertical = [&](float yValue){
            cf::Point point(0.f, yValue);
            point.x = (point.y - yIntercept) / slope;
            if (point.x >= this->m_IntervalX.min && point.x <= this->m_IntervalX.max)
                points.push_back(point);
        };
        addPoint_vertical(this->m_IntervalY.min);
        addPoint_vertical(this->m_IntervalY.max);

        // get actual points
        if (points.size() <= 1){
            if (points.size())
                this->drawPoint(points[0], color);
            return;
        }
        if (points.size() > 2){
            // remove all duplicate points
            for (size_t i = 0; i < points.size(); ++i){
                const cf::Point& current = points[i];
                for (const auto& e : points){
                    if (&current == &e)
                        continue;

                    if (current == e){
                        points.erase(points.begin() + i);
                        --i;
                        break;
                    }
                }
            }
            if (points.size() != 2)
                throw std::runtime_error(std::string("Error: In function \"") + __func__ + "\" this shouldn't have happened :)");
        }
        p1 = points[0];
        p2 = points[1];

        // draw
        if (type == cf::Window2D::LineType::DEFAULT)
            cf::Window2D::drawLine(p1, p2, lineWidth, color);
        else {
            if (lineWidth > 1)
                std::cerr << "Warning: Only default line type may use lineWidth parameter" << std::endl;
            cf::Window2D::drawSpecializedLine(p1, p2, type, color);
        }
    }

    /**
     * @brief drawCircle Draws a circle with interval radius
     * @param center Circle center
     * @param radius Circle radius
     * @param color Circle color
     * @param lineWidth Width of the line, Note: only available on default line type
     */
    void drawCircle(const cf::Point& center, float radius, const cf::Color& color = cf::Color::BLACK, int lineWidth = 1){
        int pixelRadius = std::round(this->convert_intervalLength_to_pixelLength(radius));
        if (pixelRadius <= 0)
            pixelRadius = 1;
        cf::Window2D::drawCircle(center, pixelRadius, lineWidth, color);
    }

    /**
     * @brief convert_pixelLength_to_intervalLength Converts length from pixel to interval
     * @param pixelLength
     * @return
     */
    float convert_pixelLength_to_intervalLength(float pixelLength) const {
        return (this->m_IntervalX.max - this->m_IntervalX.min) / float(this->m_Image.cols) * pixelLength;
    }

    /**
     * @brief convert_intervalLength_to_pixelLength Converts length from interval to pixel
     * @param intervalLength
     * @return
     */
    float convert_intervalLength_to_pixelLength(float intervalLength) const {
        return float(this->m_Image.cols) / (this->m_IntervalX.max - this->m_IntervalX.min) * intervalLength;
    }


    /**
     * @brief drawCriclePart Draw a partition of a circle
     * @param center Circle center
     * @param radius Circle radius (in intervall length)
     * @param startAngle Starting angle for circle (0° -> positive x direction, 90° -> positive y direction)
     * @param endAngle End angle for circle (0° -> positive x-axis, 90° -> positive y-axis)
     * @param color Circle color
     * @param lineWidth Line width of the circle
     */
    void drawCriclePart(const cf::Point& center, float radius, float startAngle, float endAngle, const cf::Color& color = cf::Color::BLACK, int lineWidth = 1){
        int pixelRadius = std::round(this->convert_intervalLength_to_pixelLength(radius));
        Window2D::drawCriclePart(center, pixelRadius, startAngle, endAngle, lineWidth, color);
    }

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
    static int _CALCULATE_HEIGHT(const cf::Interval& range_x, const cf::Interval& range_y, int width){
        float diff_y = range_y.max - range_y.min;
        float diff_x = range_x.max - range_x.min;
        return int(width * (diff_y / diff_x));
    }
    static const float ZERO_COMPARE;
};
const float cf::WindowCoordinateSystem::ZERO_COMPARE = 0.000001f;

}

#endif // WINDOW_COORDINATE_SYSTEM_H_H
