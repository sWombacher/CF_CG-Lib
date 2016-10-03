#ifndef WINDOW_RASTERIZED_H_H
#define WINDOW_RASTERIZED_H_H

#include "window2D.h"

namespace cf{

/**
 * @brief The WindowRasterized struct Default struct for verctorized operations within a custom interval
 */
struct WindowRasterized : protected Window2D {
    /**
     * @brief WindowRasterized Constructor
     * @param width Pixel width of the image
     * @param height Pixel height of the image
     * @param windowName Name of the window
     * @param startColor Background color
     */
    WindowRasterized(int width = 800, int height = 600, const char* windowName = "Chaos and Fractals", const cf::Color& startColor = {0, 0, 0})
        : Window2D(width, height, windowName, startColor)
    {
        this->setInvertYAxis(false);
    }

    /**
     * @brief WindowRasterized Load image from file path
     * @param filePath Path to file
     */
    WindowRasterized(const char* filePath) : Window2D(filePath) {}
    virtual ~WindowRasterized() = default;

    // publicly available functions  from class Window2D
    using Window2D::getWindowDisplayScale;
    using Window2D::setWindowDisplayScale;
    using Window2D::drawSpecializedLine;
    using Window2D::flippHorizontal;
    using Window2D::waitMouseInput;
    using Window2D::drawRectangle;
    using Window2D::flippVertical;
    using Window2D::drawCircle;
    using Window2D::saveImage;
    using Window2D::getHeight;
    using Window2D::getWidth;
    using Window2D::drawLine;
    using Window2D::getImage;
    using Window2D::getColor;
    using Window2D::setColor;
    using Window2D::LineType;
    using Window2D::waitKey;
    using Window2D::resize;
    using Window2D::clear;
    using Window2D::show;
};

}

#endif // WINDOW_RASTERIZED_H_H
