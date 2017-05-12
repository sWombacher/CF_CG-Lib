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
                     const char* windowName = "Chaos and Fractals", const cf::Color& startColor = cf::Color::BLACK);

    /**
     * @brief WindowVectorized Image reading constructoor
     * @param filePath Path to image file
     * @param width Image width, Note: height will be calculated based on ranges and width
     * @param range_x Interval in x direction
     * @param range_y Interval in y direction
     */
    WindowVectorized(const char* filePath, int width, const cf::Interval& range_x, const cf::Interval& range_y);

    virtual ~WindowVectorized() = default;

    /**
     * @brief setInterval Set new interval
     * @param range_x Interval in x direction
     * @param range_y Interval in y direction
     * @param width Image width in pixel (hight will be determind automatically)
     */
    void setInterval(const cf::Interval& range_x, const cf::Interval& range_y, int width);

    /**
     * @brief transformPoint_fromInterval_toImage Transform point from interval position to pixel position
     * @param point Point to be transformed
     * @return Transformed point
     */
    cf::Point transformPoint_fromInterval_toImage(cf::Point point);

    /**
     * @brief transformPoint_fromImage_toInterval Transform point from pixel position to interval position
     * @param point Point to be transformed
     * @return Transformed point
     */
    cf::Point transformPoint_fromImage_toInterval(cf::Point point);

    /**
     * @brief convert_pixelLength_to_intervalLength Converts length from pixel to interval
     * @param pixelLength Length to be converted to the intervall length
     * @return
     */
    float convert_pixelLength_to_intervalLength(float pixelLength) const;

    /**
     * @brief convert_intervalLength_to_pixelLength Converts length from interval to pixel
     * @param intervalLength Length to be converted to pixel length
     * @return
     */
    float convert_intervalLength_to_pixelLength(float intervalLength) const;


    /**
     * @brief getColor_imageSpace Get color from image x/y position
     * @param x X position
     * @param y Y position
     * @return
     */
    cf::Color getColor_imageSpace(int x, int y) const;
    /**
     * @brief setColor_imageSpace Set color from image x/y position
     * @param x X position
     * @param y Y position
     * @param color Color to be set
     */
    void setColor_imageSpace(int x, int y, const cf::Color& color);

    // publicly available functions from class Window2D
    using Window2D::getWindowDisplayScale;
    using Window2D::setWindowDisplayScale;
    using Window2D::drawSpecializedLine;
    using Window2D::drawCirclePart;
    using Window2D::waitMouseInput;
    using Window2D::drawRectangle;
    using Window2D::getIntervalX;
    using Window2D::getIntervalY;
    using Window2D::drawCircle;
    using Window2D::saveImage;
    using Window2D::getHeight;
    using Window2D::floodFill;
    using Window2D::getWidth;
    using Window2D::drawLine;
    using Window2D::getColor;
    using Window2D::setColor;
    using Window2D::drawAxis;
    using Window2D::LineType;
    using Window2D::getImage;
    using Window2D::waitKey;
    using Window2D::clear;
    using Window2D::show;

private:
    static int _CALCULATE_HEIGHT(const cf::Interval& range_x, const cf::Interval& range_y, int width);
    inline void _invertY(cf::Point& p);
};

}

#endif // WINDOW_VECTORIZED_H_H
