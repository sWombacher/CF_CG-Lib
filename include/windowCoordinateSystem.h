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
    void drawPoint(const cf::Point& pos, const cf::Color& color = cf::Color::BLACK, int lineWidth = 1){
        static const float pixelLength = 7.f;

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
                cf::Console::printWarning("Only default line type may use lineWidth parameter");
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
            if (std::abs(drawingDirection.y) < cf::WindowCoordinateSystem::ZERO_COMPARE){
                cf::Console::printWarning(__func__, " called with no direction x/y -> only the original point will be drawn");
                this->setColor(pointVector.x, pointVector.y, color);
                return;
            }
            this->drawLine({pointVector.x, this->m_IntervalY.min}, {pointVector.x, this->m_IntervalY.max}, color, type, lineWidth);
        }
        else{
            const float slope = drawingDirection.y / drawingDirection.x;
            const float yIntercept = pointVector.y - pointVector.x * slope;
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
                    cf::Console::printWarning(__func__, " called with a=b=c=0, this is ALSWAYS TRUE for all x/y pairs");
                    this->clear(color);
                }
                else{
                    cf::Console::printWarning(__func__, " called with with a=b=0 and c != 0, this is ALWAYS FALSE for all x/y pairs");
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
                        points.erase(points.begin() + int(i));
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
                cf::Console::printWarning("Only default line type may use lineWidth parameter");
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
        int pixelRadius = int(std::round(this->convert_intervalLength_to_pixelLength(radius)));
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
     * @brief drawCirclePart Draw a partition of a circle
     * @param center Circle center
     * @param radius Circle radius (in intervall length)
     * @param startAngle Starting angle for circle (0° -> positive x direction, 90° -> positive y direction)
     * @param endAngle End angle for circle (0° -> positive x-axis, 90° -> positive y-axis)
     * @param color Circle color
     * @param lineWidth Line width of the circle
     */
    void drawCirclePart(const cf::Point& center, float radius, float startAngle, float endAngle, const cf::Color& color = cf::Color::BLACK, int lineWidth = 1){
        const int pixelRadius = static_cast<int>(std::round(this->convert_intervalLength_to_pixelLength(radius)));
        Window2D::drawCirclePart(center, pixelRadius, startAngle, endAngle, lineWidth, color);
    }

    /**
     * @brief drawCirclePart Draw a partition of a circle
     * @param center Circle center
     * @param p0 1st point on the Circle line
     * @param p1 2nd point on the Circle line
     * @param color Circle color
     * @param lineWidth Line width
     * @param smallerAngle Choose wich part of the Circle should be drawn (default: smaller part of the circle)
     */
    void drawCirclePart(const cf::Point& center, const cf::Point& p0, const cf::Point& p1, const cf::Color& color = cf::Color::BLACK, int lineWidth = 1, bool smallerAngle = true){
        const cf::Point dir0 = p0 - center;
        const cf::Point dir1 = p1 - center;
        const float tmp_rad0 = std::sqrt(dir0.x * dir0.x + dir0.y * dir0.y);
        const float tmp_rad1 = std::sqrt(dir1.x * dir1.x + dir1.y * dir1.y);
        if (std::abs(tmp_rad0 - tmp_rad1) > 0.0001f)
            throw std::runtime_error("Error: provided points have different distances towards the center");

        auto calculateAngle = [&center](const cf::Point& p) -> float{
            const cf::Point tmp = p - center;
            if (std::abs(tmp.x) < cf::WindowCoordinateSystem::ZERO_COMPARE){
                if (tmp.y > 0.f)
                    return 90.f;
                else if (tmp.y < 0.f)
                    return 270.f;
                else
                    throw std::runtime_error("Error: Center and a point are equal");
            }
            const float radiant = std::atan(tmp.y / tmp.x);
            float degree = cf::radian2degree(radiant);
            if (tmp.x < 0.f)
                degree -= 180.f;
            while (degree < 0.f)
                degree += 360.f;
            return degree;
        };
        const float radius = tmp_rad0;
        float startAngle = calculateAngle(p0);
        float endAngle = calculateAngle(p1);

        startAngle = std::fmod(startAngle, 360.f);
        endAngle = std::fmod(endAngle, 360.f);
        if ((std::abs(endAngle - startAngle) > 180.f) == smallerAngle){
            auto& angle = startAngle < endAngle ? endAngle : startAngle;
            angle -= 360.f;
        }
        this->drawCirclePart(center, radius, startAngle, endAngle, color, lineWidth);
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
