#ifndef WINDOW_VECTORIZED_H_H
#define WINDOW_VECTORIZED_H_H

#include "window2D.h"

namespace cf{

/**
 * @brief The WindowVectorized struct Default class for images and raster operations
 */
struct WindowVectorized : protected Window2D {
    /**
     * @brief WindowVectorized Constructor
     * @param width Image width in pixel (hight will be determind automatically)
     * @param range_x Interval in x direction
     * @param range_y Interval in y direction
     */
    WindowVectorized(int width, const cf::Interval& range_x, const cf::Interval& range_y,
                     const char* windowName = "Chaos and Fractals", const cf::Color& startColor = cf::Color::BLACK)
        : Window2D(1, 1, windowName, startColor)
    {
        this->setInvertYAxis(true);
        this->setInterval(range_x, range_y, width);
    }

    /**
     * @brief WindowVectorized Image reading constructoor
     * @param filePath Path to image file
     * @param width Image width, Note: height will be calculated based on ranges and width
     * @param range_x Interval in x direction
     * @param range_y Interval in y direction
     */
    WindowVectorized(const char* filePath, int width, const cf::Interval& range_x, const cf::Interval& range_y)
        : Window2D(filePath)
    {
        this->setInvertYAxis(true);
        this->setInterval(range_x, range_y, width);
    }

    virtual ~WindowVectorized() = default;

    /**
     * @brief setInterval Set new interval
     * @param range_x Interval in x direction
     * @param range_y Interval in y direction
     * @param width Image width in pixel (hight will be determind automatically)
     */
    void setInterval(const cf::Interval& range_x, const cf::Interval& range_y, int width){
        this->setNewInterval(range_x, range_y);
        this->resize(width, WindowVectorized::_CALCULATE_HEIGHT(range_x, range_y, width));
    }

    /**
     * @brief transformPoint_fromInterval_toImage Transform point from interval position to pixel position
     * @param point Point to be transformed
     * @return Transformed point
     */
    cf::Point transformPoint_fromInterval_toImage(cf::Point point){
        this->_convertFromNewInterval(point.x, point.y);
        this->_invertY(point);
        return point;
    }

    /**
     * @brief transformPoint_fromImage_toInterval Transform point from pixel position to interval position
     * @param point Point to be transformed
     * @return Transformed point
     */
    cf::Point transformPoint_fromImage_toInterval(cf::Point point){
        this->_invertY(point);
        this->_convertToNewInterval(point.x, point.y);
        return point;
    }

    /**
     * @brief convert_pixelLength_to_intervalLength Converts length from pixel to interval
     * @param pixelLength Length to be converted to the intervall length
     * @return
     */
    float convert_pixelLength_to_intervalLength(float pixelLength) const {
        return (this->m_IntervalX.max - this->m_IntervalX.min) / float(this->m_Image.cols) * pixelLength;
    }

    /**
     * @brief convert_intervalLength_to_pixelLength Converts length from interval to pixel
     * @param intervalLength Length to be converted to pixel length
     * @return
     */
    float convert_intervalLength_to_pixelLength(float intervalLength) const {
        return float(this->m_Image.cols) / (this->m_IntervalX.max - this->m_IntervalX.min) * intervalLength;
    }


    /**
     * @brief getColor_imageSpace Get color from image i/j position
     * @param i I position
     * @param j J position
     * @return
     */
    cf::Color getColor_imageSpace(int i, int j) const {
        const cv::Vec3b& c = this->m_Image.at<cv::Vec3b>(i, j);
        return {c[2], c[1], c[0]};
    }
    /**
     * @brief setColor_imageSpace Set color from image i/j position
     * @param i I position
     * @param j J position
     * @param color Color to be set
     */
    void setColor_imageSpace(int i, int j, const cf::Color& color) {
        this->m_Image.at<cv::Vec3b>(i, j)[0] = color.b;
        this->m_Image.at<cv::Vec3b>(i, j)[1] = color.g;
        this->m_Image.at<cv::Vec3b>(i, j)[2] = color.r;
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
    inline void _invertY(cf::Point& p){ p.y = this->getHeight() - 1 - p.y; }
};

}

#endif // WINDOW_VECTORIZED_H_H
