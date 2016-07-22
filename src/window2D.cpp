
#include "window2D.h"
namespace cf{

Window2D::Window2D(int width, int height, const char* windowName, const cf::Color& c)
: m_Image(height, width, CV_8UC3, cv::Scalar(c.b, c.g, c.r)), m_InvertYAxis(false),
  m_WindowName(windowName), m_WindowScale(1.f),
  m_IntervallX(0, this->m_Image.cols - 1), m_IntervallY(0, this->m_Image.rows -1)
{
    if(!this->m_WindowName)
        this->m_WindowName = "";

    cv::namedWindow(this->m_WindowName);
}

Window2D::Window2D(const char *filePath):m_InvertYAxis(false), m_WindowName(filePath), m_WindowScale(1.f), m_IntervallX(0, 0), m_IntervallY(0, 0) {
    if (!filePath)
        throw std::runtime_error("Error: filePath musn't be a nullpointer");

    this->m_Image = cv::imread(filePath, CV_LOAD_IMAGE_COLOR);
    this->m_IntervallX.max = this->m_Image.cols - 1;
    this->m_IntervallY.max = this->m_Image.rows - 1;
}
Window2D::~Window2D(){ cv::destroyWindow(this->m_WindowName); }

void Window2D::show() const{
    if (this->m_WindowScale == 1.f)
        cv::imshow(this->m_WindowName, this->m_Image);
    else{
        cv::Mat tmp;
        cv::resize(this->m_Image, tmp, cv::Size(int(this->m_Image.cols * this->m_WindowScale), int(this->m_Image.rows * this->m_WindowScale)));
        cv::imshow(this->m_WindowName, tmp);
    }

    if (!this->m_FristShowCall)
        cv::waitKey(10);
    else {
        this->m_FristShowCall = false;
        cv::waitKey(300);
    }
}

void Window2D::clear(const Color &c){ this->m_Image = cv::Scalar(c.b, c.g, c.r); }

unsigned char Window2D::waitKey(int delay) const{
    return static_cast<unsigned char>(cv::waitKey(delay));
}


void mouseCallBack(int event, int x, int y, int, void* userdata){
    if  (event == cv::EVENT_LBUTTONDOWN) {
        reinterpret_cast<float*>(userdata)[0] = x;
        reinterpret_cast<float*>(userdata)[1] = y;
    }
}

void Window2D::waitMouseInput(float &x, float &y) {
    this->m_MouseCallBackStorage[0] = -1;
    this->m_MouseCallBackStorage[1] = -1;
    cv::setMouseCallback(this->m_WindowName, mouseCallBack, &this->m_MouseCallBackStorage[0]);

    do{
        cv::waitKey(200);
    } while(this->m_MouseCallBackStorage[0] == -1.f);

    // undo scaling
    this->m_MouseCallBackStorage[0] /= this->m_WindowScale;
    this->m_MouseCallBackStorage[1] /= this->m_WindowScale;

    this->_correctYValue(this->m_MouseCallBackStorage[1]);
    this->_convertToNewIntervall(this->m_MouseCallBackStorage[0], this->m_MouseCallBackStorage[1]);

    x = this->m_MouseCallBackStorage[0];
    y = this->m_MouseCallBackStorage[1];
}

void Window2D::setColor(float x, float y, const Color& c){
    this->_convertFromNewIntervall(x, y);
    this->_correctYValue(y);

    //this->m_Image.at<cf::Color>(int(y), int(x)) = c;
	auto& tmp = this->m_Image.at<cv::Vec3b>(int(y), int(x));
	tmp[0] = c.b;
	tmp[1] = c.g;
	tmp[2] = c.r;
}
Color Window2D::getColor(float x, float y) const{
    this->_convertFromNewIntervall(x, y);
    this->_correctYValue(y);

    //return this->m_Image.at<cf::Color>(int(y), int(x));
    const auto& tmp = this->m_Image.at<cv::Vec3b>(int(y), int(x));
    cf::Color c;
    c.r = tmp[2];
    c.g = tmp[1];
    c.b = tmp[0];
    return c;
}

void Window2D::setWindowDisplayScale(float scale){
    this->m_WindowScale = scale;
}
float Window2D::getWindowDisplayScale() const{
    return this->m_WindowScale;
}

void Window2D::setInvertYAxis(bool invert){
    this->m_InvertYAxis = invert;
}
bool Window2D::getInvertYAxis() const{
    return this->m_InvertYAxis;
}

void Window2D::drawCriclePart(cf::Point center, int radius, float startAngle, float endAngle, int lineWidth, const cf::Color &c){
    this->_convertFromNewIntervall(center.x, center.y);
    this->_correctYValue(center.y);

    cv::ellipse(this->m_Image, center, cv::Size(radius, radius), 0.f, -startAngle, -endAngle, cv::Scalar(c.b, c.g, c.r), lineWidth);
}

void Window2D::floodFill(Point startingPoint, const Color &c) {
    this->_convertFromNewIntervall(startingPoint.x, startingPoint.y);
    this->_correctYValue(startingPoint.y);

    cv::floodFill(this->m_Image, startingPoint, cv::Scalar(c.b, c.g, c.r));
}
void Window2D::drawCircle(cf::Point center, int radius, int lineWidth, const cf::Color& c){
    this->_convertFromNewIntervall(center.x, center.y);
    this->_correctYValue(center.y);

    cv::circle(this->m_Image, center, radius, cv::Scalar(c.b, c.g, c.r), lineWidth);
}
void Window2D::drawRectangle(cf::Point p1, cf::Point p2, int lineWidth, const cf::Color& c){
    this->_convertFromNewIntervall(p1.x, p1.y);
    this->_convertFromNewIntervall(p2.x, p2.y);
    this->_correctYValue(p1.y);
    this->_correctYValue(p2.y);

    cv::rectangle(this->m_Image, p1, p2, cv::Scalar(c.b, c.g, c.r), lineWidth);
}
void Window2D::drawLine(cf::Point p1, cf::Point p2, int lineWidth, const cf::Color& c){
    this->_convertFromNewIntervall(p1.x, p1.y);
    this->_convertFromNewIntervall(p2.x, p2.y);
    this->_correctYValue(p1.y);
    this->_correctYValue(p2.y);

    cv::line(this->m_Image, p1, p2, cv::Scalar(c.b, c.g, c.r), lineWidth);
}

void Window2D::drawSpecializedLine(cf::Point p1, cf::Point p2, cf::Window2D::LineType lineType, const cf::Color& c) {
    if (lineType == cf::Window2D::LineType::DEFAULT)
        return this->drawLine(p1, p2, 1, c);

    // * 3 + 1  are just "better looking", change if you like :)
    static const int FREQ_SCALE = 3;
    static const int FREQ_OFFSET = 1;

    this->_convertFromNewIntervall(p1.x, p1.y);
    this->_convertFromNewIntervall(p2.x, p2.y);
    this->_correctYValue(p1.y);
    this->_correctYValue(p2.y);

    const int type =  int(lineType) & (~int(3));
    const int freq = (int(lineType) &   int(3)) * FREQ_SCALE + FREQ_OFFSET;
    const cv::Vec3b col = cv::Vec3b(c.b, c.g, c.r);

    cv::LineIterator iter = cv::LineIterator(this->m_Image, p1, p2);
    for (int i = 0; i < iter.count; ++i, ++iter){
        if (!(i % freq)){
            if (type & Window2D::DOT_VALUE){
                // draw dot
                cv::circle(this->m_Image, iter.pos(), 1, cv::Scalar(c.b, c.g, c.r), -1);
            }
            if (type == (Window2D::DOT_VALUE | Window2D::DASH_VALUE)){
                // dot dash
                // draw some space between dot and dash
                for (int f = 0; f < freq && i < iter.count; ++f){
                    ++i;
                    ++iter;
                }
            }
            if (type & Window2D::DASH_VALUE){
                // draw dash
                for (int lengthCounter = 0;
                     lengthCounter < 2 * freq && i < iter.count;
                     ++lengthCounter, ++iter, ++i)
                {
                    this->m_Image.at<cv::Vec3b>(iter.pos()) = col;
                }
            }
            if (i >= iter.count)
                break;
        }
    }
}

void Window2D::setNewIntervall(const Intervall& intervallX, const Intervall& intervallY){
    this->m_IntervallX = intervallX;
    this->m_IntervallY = intervallY;

    if (intervallX.min != 0.f || intervallX.max != this->m_Image.cols - 1 ||
        intervallY.min != 0.f || intervallY.max != this->m_Image.rows - 1)
    {
        this->m_IntervallChanged = true;
    }
    else
        this->m_IntervallChanged = false;
}

void Window2D::resetIntervall(){
    this->m_IntervallX.min = 0;
    this->m_IntervallX.max = this->m_Image.cols - 1;

    this->m_IntervallY.min = 0;
    this->m_IntervallY.max = this->m_Image.rows - 1;

    this->m_IntervallChanged = false;
}

void Window2D::saveImage(const char *filePath) const{
    cv::imwrite(filePath, this->m_Image);
}

void Window2D::resize(int pixelWidth, int pixelHeight){
    cv::resize(this->m_Image, this->m_Image, {pixelWidth, pixelHeight});
}

void Window2D::flippHorizontal() {
    cv::flip(this->m_Image, this->m_Image, 0);
}
void Window2D::flippVertical() {
    cv::flip(this->m_Image, this->m_Image, 1);
}

int Window2D::getWidth() const{
    return this->m_Image.cols;
}
int Window2D::getHeight() const{
    return this->m_Image.rows;
}
cv::Mat& Window2D::getImage(){
    return this->m_Image;
}

void Window2D::drawAxis(const cf::Color& color, float stepSize_x, float stepSize_y, float interceptLength){
    static const int lineWidth = 1;
    static const int helperLines_occurence = 4;
    static const float helperLine_adjustment = 2.5f;

    // draw complete x- and y-axis
    //
    // Note:
    //  opencv takes care of pixel positions out of image
    this->drawLine({this->m_IntervallX.min, 0.f}, {this->m_IntervallX.max, 0.f}, lineWidth, color);
    this->drawLine({0.f, this->m_IntervallY.min}, {0.f, this->m_IntervallY.max}, lineWidth, color);

    if (stepSize_x < 0.f)
        stepSize_x = (this->m_IntervallX.max - this->m_IntervallX.min) / 10.f;
    if (stepSize_y < 0.f)
        stepSize_y = (this->m_IntervallY.max - this->m_IntervallY.min) / 10.f;

    auto drawAxisInterceptions = [&](bool horizontalLine){
        float stepSize = horizontalLine ? stepSize_x : stepSize_y;
        int pixelLength = horizontalLine ? this->m_Image.rows : this->m_Image.cols;
        const cf::Intervall& intervall = horizontalLine ? this->m_IntervallX : this->m_IntervallY;

        int startPos = int(intervall.min / stepSize);
        int iterationCounter = helperLines_occurence - std::abs(startPos % helperLines_occurence);

        float verticalLineHeight = (intervall.max - intervall.min) / float(pixelLength) * interceptLength * lineWidth;
        for (float start = startPos * stepSize_x;
             start <= intervall.max;
             start += stepSize_x)
        {
            // draw vertical line
            float adjusted_lineWidth = verticalLineHeight;
            if (iterationCounter++ == helperLines_occurence){
                iterationCounter = 0;
                adjusted_lineWidth *= helperLine_adjustment;
            }
            if (horizontalLine)
                this->drawLine({start, adjusted_lineWidth}, {start, -adjusted_lineWidth}, 1, color);
            else
                this->drawLine({adjusted_lineWidth, start}, {-adjusted_lineWidth, start}, 1, color);
        }
    };
    drawAxisInterceptions(true);  // draw horizontal intercepts
    drawAxisInterceptions(false); // draw  vertical  intercepts
}


const cf::Intervall& Window2D::getIntervallX() const{
    return this->m_IntervallX;
}
const cf::Intervall& Window2D::getIntervallY() const{
    return this->m_IntervallY;
}


template<typename T>
void Window2D::_correctYValue(T& y) const{
    if (this->m_InvertYAxis)
        y = (this->m_Image.rows - 1) - y;
}
template<typename T>
void Window2D::_convertFromNewIntervall(T& x, T& y) const{
    if (!this->m_IntervallChanged)
        return;

    x = Intervall::translateIntervallPostion(this->m_IntervallX, Intervall(0, this->m_Image.cols - 1), x);
    y = Intervall::translateIntervallPostion(this->m_IntervallY, Intervall(0, this->m_Image.rows - 1), y);
}
template<typename T>
void Window2D::_convertToNewIntervall(T& x, T& y) const{
    if (!this->m_IntervallChanged)
        return;

    x = Intervall::translateIntervallPostion(Intervall(0, this->m_Image.cols - 1), this->m_IntervallX, x);
    y = Intervall::translateIntervallPostion(Intervall(0, this->m_Image.rows - 1), this->m_IntervallY, y);
}

bool Point::operator==(const Point &p) const{
    return this->x == p.x && this->y == p.y;
}
bool Point::operator!=(const Point &p) const{
    return !(*this == p);
}

Point  Point::operator+ (const Point& p) const {
    return { this->x + p.x, this->y + p.y };
}
Point& Point::operator+=(const Point& p){
    this->x += p.x;
    this->y += p.y;
    return *this;
}

Point  Point::operator- (const Point& p) const{
    return { this->x - p.x, this->y - p.y };
}
Point& Point::operator-=(const Point& p){
    this->x -= p.x;
    this->y -= p.y;
    return *this;
}

Point  Point::operator* (float factor) const {
    return { this->x * factor, this->y * factor };
}
Point& Point::operator*=(float factor){
    this->x *= factor;
    this->y *= factor;
    return *this;
}

Point  Point::operator/ (float rhs) const{
    return { this->x / rhs, this->y / rhs };
}
Point& Point::operator/=(float rhs){
    this->x /= rhs;
    this->y /= rhs;
    return *this;
}

Point::operator cv::Point() const {
    return cv::Point(std::round(this->x), std::round(this->y));
}

Point operator* (float factor, const Point& p){
    return p * factor;
}
Point operator/ (float lhs, const Point& p){
    return p * lhs;
}


}
