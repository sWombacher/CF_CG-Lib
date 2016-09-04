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
     * @param range_x intervall in x direction
     * @param range_y intervall in y direction
     * @param width image width in pixel (hight will be determind automatically)
     */
    WindowVectorized(int width, const cf::Intervall& range_x, const cf::Intervall& range_y,
                     const char* windowName = "Chaos and Fractals", const cf::Color& startColor = cf::Color::BLACK)
        : Window2D(1, 1, windowName, startColor)
    {
        this->setInvertYAxis(true);
        this->setIntervall(range_x, range_y, width);
    }
    virtual ~WindowVectorized() = default;

    /**
     * @brief setIntervall set new intervall
     * @param range_x intervall in x direction
     * @param range_y intervall in y direction
     * @param width image width in pixel (hight will be determind automatically)
     */
    void setIntervall(const cf::Intervall& range_x, const cf::Intervall& range_y, int width){
        this->setNewIntervall(range_x, range_y);
        this->resize(width, WindowVectorized::_CALCULATE_HEIGHT(range_x, range_y, width));
    }

    /**
     * @brief transformPoint_fromImageIntervall transform point from intervall position to pixel position
     * @param p point to be transformed
     * @return transformed point
     */
    cf::Point transformPoint_fromImageIntervall(const cf::Point p){
        cf::Point tmp = p;
        this->_convertFromNewIntervall(tmp.x, tmp.y);
        return tmp;
    }

    /**
     * @brief transformPoint_fromImageIntervall transform point from pixel position to intervall position
     * @param p point to be transformed
     * @return transformed point
     */
    cf::Point transformPoint_toImgeIntervall(const cf::Point p){
        cf::Point tmp = p;
        this->_convertToNewIntervall(tmp.x, tmp.y);
        return tmp;
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

    // publicly available functions from class Window2D
    using Window2D::getWindowDisplayScale;
    using Window2D::setWindowDisplayScale;
    using Window2D::drawSpecializedLine;
    using Window2D::drawCriclePart;
    using Window2D::waitMouseInput;
    using Window2D::getIntervallX;
    using Window2D::getIntervallY;
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
    static int _CALCULATE_HEIGHT(const cf::Intervall& range_x, const cf::Intervall& range_y, int width){
        float diff_y = range_y.max - range_y.min;
        float diff_x = range_x.max - range_x.min;
        return int(width * (diff_y / diff_x));
    }
};

}

#endif // WINDOW_VECTORIZED_H_H
