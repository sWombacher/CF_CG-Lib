
#include "window2D.h"
namespace cf{

Window_2D::Window_2D(std::size_t width, std::size_t height, const char* windowName)
: m_WindowName(windowName), m_Image(height, width, CV_8UC3, cv::Scalar(0, 0, 0)), m_Scale(1.f)
{
    if(!this->m_WindowName)
        this->m_WindowName = "";

    this->show();
}
Window_2D::~Window_2D(){
    cv::destroyWindow(this->m_WindowName);
}

void Window_2D::show() const{
    if (this->m_Scale == 1.f)
        cv::imshow(this->m_WindowName, this->m_Image);
    else{
        cv::Mat tmp;
        cv::resize(this->m_Image, tmp, cv::Size(this->m_Image.cols * this->m_Scale, this->m_Image.rows * this->m_Scale));
        cv::imshow(this->m_WindowName, tmp);
    }
}
void Window_2D::waitKey() const{
    cv::waitKey();
}

void Window_2D::setColor(std::size_t x, std::size_t y, const Color& c){
    this->_getCorrectYValue(y);
    if (sizeof(cf::Color) == sizeof(cv::Vec3b)) // should always be true :)
        this->m_Image.at<cf::Color>(y, x) = c;
    else
        this->m_Image.at<cv::Vec3b>(y, x) = cv::Vec3b(c.b, c.g, c.r);
}
const Color& Window_2D::getColor(std::size_t x, std::size_t y) const{
    this->_getCorrectYValue(y);
    if (sizeof(cf::Color) == sizeof(cv::Vec3b)) // should always be true :)
        return this->m_Image.at<cf::Color>(y, x);
    else{
        const cv::Vec3b& tmp = this->m_Image.at<cv::Vec3b>(y, x);
        return cf::Color(tmp[2], tmp[1], tmp[0]);
    }
}

void Window_2D::setScale(float scale){
    this->m_Scale = scale;
}
float Window_2D::getScale() const{
    return this->m_Scale;
}

void Window_2D::setInvertYAxis(bool invert){
    this->m_InvertYAxis = invert;
}
bool Window_2D::getInvertYAxis() const{
    return this->m_InvertYAxis;
}


void Window_2D::drawCircle(cv::Point point, std::size_t radius, int lineWidth, const cf::Color& c){
    this->_getCorrectYValue(point.y);
    cv::circle(this->m_Image, point, radius, cv::Scalar(c.b, c.g, c.r), lineWidth);
}
void Window_2D::drawRectangle(cv::Point point1, cv::Point point2, int lineWidth, const cf::Color& c){
    this->_getCorrectYValue(point1.y);
    this->_getCorrectYValue(point2.y);
    cv::rectangle(this->m_Image, point1, point2, cv::Scalar(c.b, c.g, c.r), lineWidth);
}
void Window_2D::drawLine(cv::Point point1, cv::Point point2, int lineWidth, const cf::Color& c){
    this->_getCorrectYValue(point1.y);
    this->_getCorrectYValue(point2.y);
    cv::line(this->m_Image, point1, point2, cv::Scalar(c.b, c.g, c.r), lineWidth);
}

template<typename T>
void Window_2D::_getCorrectYValue(cf::Window_2D::T &y){
    if (this->m_InvertYAxis)
        y = this->m_Image.rows - y;
}

}
