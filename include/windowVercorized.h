#include "window2D.h"

namespace cf{

struct WindowRasterized : protected Window2D {
    WindowRasterized(int width, const cf::Intervall& range_x, const cf::Intervall& range_y,
                     const char* windowName = "Chaos and Fractals", const cf::Color& startColor = {0, 0, 0})
        : Window2D(1, 1, windowName, startColor)
    {
        this->setInvertYAxis(true);
        this->setIntervall(range_x, range_y, width);
    }
    virtual ~WindowRasterized() = default;
    void setIntervall(const cf::Intervall& range_x, const cf::Intervall& range_y, int width){
        this->setNewIntervall(range_x, range_y);
        this->resize(width, WindowRasterized::_CALCULATE_HEIGHT(range_x, range_y, width));
    }

    cf::Point transformPoint_fromImageIntervall(const cf::Point p){
        cf::Point tmp = p;
        this->_convertFromNewIntervall(tmp.x, tmp.y);
        return tmp;
    }
    cf::Point transformPoint_toImgeIntervall(const cf::Point p){
        cf::Point tmp = p;
        this->_convertToNewIntervall(tmp.x, tmp.y);
        return tmp;
    }

    // publicly available functions from class Window2D
    using Window2D::getWindowDisplayScale;
    using Window2D::setWindowDisplayScale;
    using Window2D::waitMouseInput;
    using Window2D::getImageHeight;
    using Window2D::getImageWidth;
    using Window2D::drawRectangle;
    using Window2D::drawCircle;
    using Window2D::saveImage;
    using Window2D::drawLine;
    using Window2D::getColor;
    using Window2D::setColor;
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
