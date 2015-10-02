
#include "window2D.h"
namespace cf{

Window2D::Window2D(uint width, uint height, const char* windowName)
: m_WindowName(windowName), m_Image(height, width, CV_8UC3, cv::Scalar(0, 0, 0)), m_WindowScale(1.f),
  m_InvertYAxis(true), m_IntervallX(0, this->m_Image.cols - 1), m_IntervallY(0, this->m_Image.rows -1)
{
    if(!this->m_WindowName)
        this->m_WindowName = "";

    this->show();
}

Window2D::Window2D(const char *filename):m_IntervallX(0, 0), m_IntervallY(0, 0){
    this->m_Image = cv::imread(filename, CV_LOAD_IMAGE_COLOR);
    this->m_IntervallX.max = this->m_Image.cols - 1;
    this->m_IntervallY.max = this->m_Image.rows - 1;
}
Window2D::~Window2D(){
    cv::destroyWindow(this->m_WindowName);
}

void Window2D::show() const{
    if (this->m_WindowScale == 1.f)
        cv::imshow(this->m_WindowName, this->m_Image);
    else{
        cv::Mat tmp;
        cv::resize(this->m_Image, tmp, cv::Size(this->m_Image.cols * this->m_WindowScale, this->m_Image.rows * this->m_WindowScale));
        cv::imshow(this->m_WindowName, tmp);
    }
}
void Window2D::waitKey() const{
    cv::waitKey();
}


void mouseCallBack(int event, int x, int y, int flags, void* userdata){
    if  (event == cv::EVENT_LBUTTONDOWN) {
        reinterpret_cast<float*>(userdata)[0] = x;
        reinterpret_cast<float*>(userdata)[1] = y;
    }
}

void Window2D::waitMouseInput(float &x, float &y) {
    this->m_MouseCallBackStorage[0] = -1;
    this->m_MouseCallBackStorage[1] = -1;
    this->show();
    cv::setMouseCallback(this->m_WindowName, mouseCallBack, &this->m_MouseCallBackStorage[0]);

    do{
        cv::waitKey(200);
    } while(this->m_MouseCallBackStorage[0] == -1);

    // undo scaling
    this->m_MouseCallBackStorage[0] /= this->m_WindowScale;
    this->m_MouseCallBackStorage[1] /= this->m_WindowScale;

    this->_correctYValue(this->m_MouseCallBackStorage[1]);
    this->_convertToNewIntervall(this->m_MouseCallBackStorage[0], this->m_MouseCallBackStorage[1]);
    x = this->m_MouseCallBackStorage[0];
    y = this->m_MouseCallBackStorage[1];
}

void Window2D::setColor(float x, float y, const Color& c){
    this->_correctYValue(y);
    this->_convertFromNewIntervall(x, y);
    if (sizeof(cf::Color) == sizeof(cv::Vec3b)) // should always be true :)
        this->m_Image.at<cf::Color>(y, x) = c;
    else
        this->m_Image.at<cv::Vec3b>(y, x) = cv::Vec3b(c.b, c.g, c.r);
}
Color Window2D::getColor(float x, float y) const{
    this->_correctYValue(y);
    if (sizeof(cf::Color) == sizeof(cv::Vec3b)) // should always be true :)
        return this->m_Image.at<cf::Color>(y, x);
    else{
        const cv::Vec3b& tmp = this->m_Image.at<cv::Vec3b>(y, x);
        return cf::Color(tmp[2], tmp[1], tmp[0]);
    }
}

void Window2D::setWindowScale(float scale){
    this->m_WindowScale = scale;
}
float Window2D::getWindowScale() const{
    return this->m_WindowScale;
}

void Window2D::setInvertYAxis(bool invert){
    this->m_InvertYAxis = invert;
}
bool Window2D::getInvertYAxis() const{
    return this->m_InvertYAxis;
}


void Window2D::drawCircle(cf::Point point, uint radius, int lineWidth, const cf::Color& c){
    this->_correctYValue(point.y);
    this->_convertFromNewIntervall(point.x, point.y);
    cv::circle(this->m_Image, cv::Point(point.x, point.y), radius, cv::Scalar(c.b, c.g, c.r), lineWidth);
}
void Window2D::drawRectangle(cf::Point p1, cf::Point p2, int lineWidth, const cf::Color& c){
    this->_correctYValue(p1.y);
    this->_correctYValue(p2.y);
    this->_convertFromNewIntervall(p1.x, p1.y);
    this->_convertFromNewIntervall(p2.x, p2.y);
    cv::rectangle(this->m_Image, cv::Point(p1.x, p1.y), cv::Point(p2.x, p2.y), cv::Scalar(c.b, c.g, c.r), lineWidth);
}
void Window2D::drawLine(cf::Point p1, cf::Point p2, int lineWidth, const cf::Color& c){
    this->_correctYValue(p1.y);
    this->_correctYValue(p2.y);
    this->_convertFromNewIntervall(p1.x, p1.y);
    this->_convertFromNewIntervall(p2.x, p2.y);
    cv::line(this->m_Image, cv::Point(p1.x, p1.y), cv::Point(p2.x, p2.y), cv::Scalar(c.b, c.g, c.r), lineWidth);
}

void Window2D::setNewIntervall(const Intervall& intervallX, const Intervall& intervallY){
    this->m_IntervallX = intervallX;
    this->m_IntervallY = intervallY;
}

void Window2D::resetIntervall(){
    this->m_IntervallX.min = 0;
    this->m_IntervallX.max = this->m_Image.cols - 1;

    this->m_IntervallY.min = 0;
    this->m_IntervallY.max = this->m_Image.rows - 1;
}

int Window2D::getImageWidth() const{
    return this->m_Image.cols;
}
int Window2D::getImageHeight() const{
    return this->m_Image.rows;
}
cv::Mat& Window2D::getImage(){
    return this->m_Image;
}


template<typename T>
void Window2D::_correctYValue(T& y) const{
    if (this->m_InvertYAxis)
        y = this->m_Image.rows - y;
}
template<typename T>
void Window2D::_convertFromNewIntervall(T& x, T& y) const{
    x = Intervall::translateInterverllPostion(this->m_IntervallX, Intervall(0, this->m_Image.cols - 1), x);
    y = Intervall::translateInterverllPostion(this->m_IntervallY, Intervall(0, this->m_Image.rows - 1), y);
}
template<typename T>
void Window2D::_convertToNewIntervall(T& x, T& y) const{
    x = Intervall::translateInterverllPostion(Intervall(0, this->m_Image.cols - 1), this->m_IntervallX, x);
    y = Intervall::translateInterverllPostion(Intervall(0, this->m_Image.rows - 1), this->m_IntervallY, y);
}

}
