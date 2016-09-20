#ifndef WINDOW_VECTORIZED_H_H
#define WINDOW_VECTORIZED_H_H

#include "window2D.h"

namespace cf{

/**
 * @brief The WindowVectorized struct Default class for images and raster operations
 */
struct WindowVectorized : protected Window2D {
    /**
     * @brief WindowVectorized constructor
     * @param range_x interval in x direction
     * @param range_y interval in y direction
     * @param width image width in pixel (hight will be determind automatically)
     */
    WindowVectorized(int width, const cf::Interval& range_x, const cf::Interval& range_y,
                     const char* windowName = "Chaos and Fractals", const cf::Color& startColor = cf::Color::BLACK)
        : Window2D(1, 1, windowName, startColor)
    {
        this->setInvertYAxis(false);
        this->setInterval(range_x, range_y, width);
    }
    virtual ~WindowVectorized() = default;

    /**
     * @brief setInterval set new interval
     * @param range_x interval in x direction
     * @param range_y interval in y direction
     * @param width image width in pixel (hight will be determind automatically)
     */
    void setInterval(const cf::Interval& range_x, const cf::Interval& range_y, int width){
        this->setNewInterval(range_x, range_y);
        this->resize(width, WindowVectorized::_CALCULATE_HEIGHT(range_x, range_y, width));
    }

    /**
     * @brief transformPoint_fromInterval_toImage transform point from interval position to pixel position
     * @param p point to be transformed
     * @return transformed point
     */
    cf::Point transformPoint_fromInterval_toImage(const cf::Point p){
        cf::Point tmp = p;
        this->_convertFromNewInterval(tmp.x, tmp.y);
        return tmp;
    }

    /**
     * @brief transformPoint_fromImage_toInterval transform point from pixel position to interval position
     * @param p point to be transformed
     * @return transformed point
     */
    cf::Point transformPoint_fromImage_toInterval(const cf::Point p){
        cf::Point tmp = p;
        this->_convertToNewInterval(tmp.x, tmp.y);
        return tmp;
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

    // publicly available functions from class Window2D
    using Window2D::getWindowDisplayScale;
    using Window2D::setWindowDisplayScale;
    using Window2D::drawSpecializedLine;
    using Window2D::drawCriclePart;
    using Window2D::waitMouseInput;
    using Window2D::getIntervalX;
    using Window2D::getIntervalY;
    using Window2D::drawRectangle;
    using Window2D::drawCircle;
    using Window2D::saveImage;
    using Window2D::getHeight;
    using Window2D::getWidth;
    using Window2D::drawLine;
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
};

}

#endif // WINDOW_VECTORIZED_H_H
