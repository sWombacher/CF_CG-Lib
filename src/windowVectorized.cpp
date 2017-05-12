#include "windowVectorized.h"
namespace cf {

cf::WindowVectorized::WindowVectorized(int width, const cf::Interval& range_x, const cf::Interval& range_y,
                                       const char* windowName, const cf::Color& startColor)
    : Window2D(1, 1, windowName, startColor) {
    this->setInvertYAxis(true);
    this->setInterval(range_x, range_y, width);
}

cf::WindowVectorized::WindowVectorized(const char* filePath, int width, const cf::Interval& range_x,
                                       const cf::Interval& range_y)
    : Window2D(filePath) {
    this->setInvertYAxis(true);
    this->setInterval(range_x, range_y, width);
}

void cf::WindowVectorized::setInterval(const cf::Interval& range_x, const cf::Interval& range_y, int width) {
    this->setNewInterval(range_x, range_y);
    this->resize(width, WindowVectorized::_CALCULATE_HEIGHT(range_x, range_y, width));
}

cf::Point cf::WindowVectorized::transformPoint_fromInterval_toImage(cf::Point point) {
    this->_convertFromNewInterval(point.x, point.y);
    this->_invertY(point);
    return point;
}

cf::Point cf::WindowVectorized::transformPoint_fromImage_toInterval(cf::Point point) {
    this->_invertY(point);
    this->_convertToNewInterval(point.x, point.y);
    return point;
}

float cf::WindowVectorized::convert_pixelLength_to_intervalLength(float pixelLength) const {
    return (this->m_IntervalX.max - this->m_IntervalX.min) / float(this->m_Image.cols) * pixelLength;
}

float cf::WindowVectorized::convert_intervalLength_to_pixelLength(float intervalLength) const {
    return float(this->m_Image.cols) / (this->m_IntervalX.max - this->m_IntervalX.min) * intervalLength;
}

cf::Color cf::WindowVectorized::getColor_imageSpace(int x, int y) const {
    const cv::Vec3b& c = this->m_Image.at<cv::Vec3b>(y, x);
    return {c[2], c[1], c[0]};
}

void cf::WindowVectorized::setColor_imageSpace(int x, int y, const cf::Color& color) {
    this->m_Image.at<cv::Vec3b>(y, x)[0] = color.b;
    this->m_Image.at<cv::Vec3b>(y, x)[1] = color.g;
    this->m_Image.at<cv::Vec3b>(y, x)[2] = color.r;
}

int cf::WindowVectorized::_CALCULATE_HEIGHT(const cf::Interval& range_x, const cf::Interval& range_y, int width) {
    float diff_y = range_y.max - range_y.min;
    float diff_x = range_x.max - range_x.min;
    return int(std::round(width * (diff_y / diff_x)));
}

void cf::WindowVectorized::_invertY(cf::Point& p) { p.y = this->getHeight() - 1 - p.y; }
}
