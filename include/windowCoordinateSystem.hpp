#ifndef WINDOW_COORDINATE_SYSTEM_H_H
#define WINDOW_COORDINATE_SYSTEM_H_H

#include "window2D.h"

namespace cf{

/**
 * @brief The WindowCoordinateSystem struct Default class for images and raster operations
 */
struct WindowCoordinateSystem : protected Window2D {
    /**
     * @brief WindowCoordinateSystem constructor
     * @param range_x intervall in x direction
     * @param range_y intervall in y direction
     * @param width image width in pixel (hight will be determind automatically)
     */
    WindowCoordinateSystem(int width, const cf::Intervall& range_x, const cf::Intervall& range_y,
                     const char* windowName = "Chaos and Fractals", const cf::Color& startColor = cf::Color::WHITE)
        : Window2D(1, 1, windowName, startColor)
    {
        this->setInvertYAxis(true);
        this->setIntervall(range_x, range_y, width);
    }
    virtual ~WindowCoordinateSystem() = default;

    /**
     * @brief setIntervall set new intervall
     * @param range_x intervall in x direction
     * @param range_y intervall in y direction
     * @param width image width in pixel (hight will be determind automatically)
     */
    void setIntervall(const cf::Intervall& range_x, const cf::Intervall& range_y, int width){
        this->setNewIntervall(range_x, range_y);
        this->resize(width, WindowCoordinateSystem::_CALCULATE_HEIGHT(range_x, range_y, width));
    }

    /**
     * @brief drawPoint draws a cross-shaed point
     * @param pos cross position
     * @param color cross color
     */
    void drawPoint(const cf::Point& pos, const cf::Color& color = cf::Color::BLACK){
        static const float pixelLength = 7.f;
        static const int lineWidth = 1;

        const float half_intervallLength = std::abs(this->convert_pixelLength_to_intervallLength(pixelLength)) / 2.f;
        cf::Window2D::drawLine({pos.x - half_intervallLength, pos.y - half_intervallLength},
                               {pos.x + half_intervallLength, pos.y + half_intervallLength}, lineWidth, color);
        cf::Window2D::drawLine({pos.x - half_intervallLength, pos.y + half_intervallLength},
                               {pos.x + half_intervallLength, pos.y - half_intervallLength}, lineWidth, color);
    }

    /**
     * @brief drawLine Draw a simple line of width 1
     * @param p1 First point
     * @param p2 Second point
     * @param color Line color
     * @param type Line type
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
     * @brief drawLinearEquation Draws line from a point on line and direction vector
     * @param pointVector Point on the line
     * @param drawingDirection Line direction
     * @param color line color
     * @param type Change line type to dot/dash/dot-dash
     */
    void drawLinearEquation(const cf::Point pointVector, const glm::vec3& drawingDirection, const cf::Color& color = cf::Color::BLACK,
                            cf::Window2D::LineType type = cf::Window2D::LineType::DEFAULT, int lineWidth = 1)
    {
        //if (drawingDirection.z != 0.f)
        if (this->_compareZero_3val(drawingDirection.z, drawingDirection.x, drawingDirection.y) == false)
            throw std::runtime_error("Error: direction vector may only have values != 0 onf x and y coordinates");

        //if (drawingDirection.x == 0.f)
        if (this->_compareZero_3val(drawingDirection.x, drawingDirection.y, drawingDirection.z))
            this->drawLine({pointVector.x, this->m_IntervallY.min}, {pointVector.x, this->m_IntervallY.max}, color, type, lineWidth);
        else{
            float slope = drawingDirection.y / drawingDirection.x;
            float yIntercept = pointVector.y - pointVector.x * slope;
            this->drawLinearEquation(slope, yIntercept, color, type, lineWidth);
        }
    }

    /**
     * @brief drawLinearEquation draw line from linear equation: ax + by + c = 0
     * @param a coefficent of x
     * @param b coefficent of y
     * @param c constant
     * @param color line color
     * @param type Change line type to dot/dash/dot-dash
     */
    void drawLinearEquation(float a, float b, float c, const cf::Color& color = cf::Color::BLACK,
                            cf::Window2D::LineType type = cf::Window2D::LineType::DEFAULT, int lineWidth = 1)
    {
        //if (b == 0.f) {
        if (this->_compareZero_3val(b, a, c)){
            // 0*y = 0
            // <=>  ax + c = 0
            // <=>  x = -c/a
            this->drawLine({-c/a, this->m_IntervallY.min}, {-c/a, this->m_IntervallY.max}, color, type, lineWidth);
        }
        else {
            //       0 =  ax + by + c
            // <=>  by = -ax -c
            //  =>   y = -a/b x -c/b
            this->drawLinearEquation(-a/b, -c/b, color, type, lineWidth);
        }
    }

    /**
     * @brief drawLinearEquation draw line from linear equation: ax + by + c = 0, where a b and c are part of the vector v(a, b, c)
     * @param vec Vector of cooefficents a b and see
     * @param color line color
     * @param type Change line type to dot/dash/dot-dash
     */
    void drawLinearEquation(const glm::vec3& vec, const cf::Color& color = cf::Color::BLACK,
                            cf::Window2D::LineType type = cf::Window2D::LineType::DEFAULT, int lineWidth = 1)
    {
        this->drawLinearEquation(vec[0], vec[1], vec[2], color, type, lineWidth);
    }

    /**
     * @brief drawLinearEquation draw line from standard format y = m*x + t
     * @param slope m of equation y = m*x + t
     * @param yIntercept t of equation y = m*x + t
     * @param color line color
     * @param type Change line type to dot/dash/dot-dash
     */
    void drawLinearEquation(float slope, float yIntercept, const cf::Color& color = cf::Color::BLACK,
                            cf::Window2D::LineType type = cf::Window2D::LineType::DEFAULT, int lineWidth = 1)
    {
        // calculate points from min/max x-intervall
        float y_min = this->m_IntervallX.min * slope + yIntercept;
        float y_max = this->m_IntervallX.max * slope + yIntercept;
        if (type == cf::Window2D::LineType::DEFAULT)
            cf::Window2D::drawLine({this->m_IntervallX.min, y_min}, {this->m_IntervallX.max, y_max}, lineWidth, color);
        else {
            if (lineWidth > 1)
                std::cerr << "Warning: Only default line type may use lineWidth parameter" << std::endl;
            cf::Window2D::drawSpecializedLine({this->m_IntervallX.min, y_min}, {this->m_IntervallX.max, y_max}, type, color);
        }
    }

    /**
     * @brief drawCircle Draw circle with intervall radius
     * @param center Circle center
     * @param radius Circle radius
     * @param color  Curcle color
     */
    void drawCircle(const cf::Point& center, float radius, const cf::Color& color = cf::Color::BLACK, int lineWidth = 1){
        int pixelRadius = std::round(this->convert_intervallLength_to_pixelLength(radius));
        if (pixelRadius <= 0)
            pixelRadius = 1;
        cf::Window2D::drawCircle(center, pixelRadius, lineWidth, color);
    }

    /**
     * @brief convert_pixelLength_to_intervallLength Converts length from pixel to intervall
     * @param pixelLength
     * @return
     */
    float convert_pixelLength_to_intervallLength(float pixelLength) const {
        return (this->m_IntervallX.max - this->m_IntervallX.min) / float(this->m_Image.cols) * pixelLength;
    }

    /**
     * @brief convert_intervallLength_to_pixelLength Converts length from intervall to pixel
     * @param intervallLength
     * @return
     */
    float convert_intervallLength_to_pixelLength(float intervallLength) const {
        return float(this->m_Image.cols) / (this->m_IntervallX.max - this->m_IntervallX.min) * intervallLength;
    }


    void drawCriclePart(const cf::Point& center, float radius, float startAngle, float endAngle, const cf::Color& color = cf::Color::BLACK, int lineWidth = 1){
        float pixelRadius = this->convert_intervallLength_to_pixelLength(radius);
        Window2D::drawCriclePart(center, pixelRadius, startAngle, endAngle, lineWidth, color);
    }

    // publicly available functions from class Window2D
    using Window2D::getWindowDisplayScale;
    using Window2D::setWindowDisplayScale;
    using Window2D::waitMouseInput;
    using Window2D::getIntervallX;
    using Window2D::getIntervallY;
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
    static int _CALCULATE_HEIGHT(const cf::Intervall& range_x, const cf::Intervall& range_y, int width){
        float diff_y = range_y.max - range_y.min;
        float diff_x = range_x.max - range_x.min;
        return int(width * (diff_y / diff_x));
    }

    // com will be compared to zero
    // _1 and _2 are used for "normalization"
    bool _compareZero_3val(float com, float _1, float _2){
        com = std::abs(com);
        _1 = std::abs(_1);
        _2 = std::abs(_2);

        float max = com > _1 ? com : _1;
        max = max > _2 ? max : _2;
        if (max == 0.f)
            return true;

        com /= max;
        _1 /= max;
        _2 /= max;
        return std::abs(com) < 1.e-4f;
    }
};

}

#endif // WINDOW_COORDINATE_SYSTEM_H_H
