
#include "window2D.h"
namespace cf{

Window2D::Window2D(int width, int height, const char* windowName, const cf::Color& c)
: m_Image(height, width, CV_8UC3, cv::Scalar(c.b, c.g, c.r)), m_InvertYAxis(false),
  m_WindowName(windowName), m_WindowScale(1.f),
  m_IntervalX(0, this->m_Image.cols - 1), m_IntervalY(0, this->m_Image.rows -1)
{
    if(!this->m_WindowName)
        this->m_WindowName = "";

    cv::namedWindow(this->m_WindowName);
}

Window2D::Window2D(const char *filePath):m_InvertYAxis(false), m_WindowName(filePath), m_WindowScale(1.f), m_IntervalX(0, 0), m_IntervalY(0, 0) {
    if (!filePath)
        throw std::runtime_error("Error: filePath musn't be a nullpointer");

    this->m_Image = cv::imread(filePath, CV_LOAD_IMAGE_COLOR);
    this->m_IntervalX.max = this->m_Image.cols - 1;
    this->m_IntervalY.max = this->m_Image.rows - 1;
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
	this->_window2foreground();
    return static_cast<unsigned char>(cv::waitKey(delay));
}


void mouseCallBack(int event, int x, int y, int, void* userdata){
    if  (event == cv::EVENT_LBUTTONDOWN) {
        reinterpret_cast<float*>(userdata)[0] = x;
        reinterpret_cast<float*>(userdata)[1] = y;
    }
}

void Window2D::waitMouseInput(float &x, float &y) {
	this->_window2foreground();

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
    this->_convertToNewInterval(this->m_MouseCallBackStorage[0], this->m_MouseCallBackStorage[1]);

    x = this->m_MouseCallBackStorage[0];
    y = this->m_MouseCallBackStorage[1];
}

void Window2D::setColor(float x, float y, const Color& c){
    this->_convertFromNewInterval(x, y);
    this->_correctYValue(y);

    auto& tmp = this->m_Image.at<cv::Vec3b>(int(y), int(x));
	tmp[0] = c.b;
	tmp[1] = c.g;
	tmp[2] = c.r;
}
Color Window2D::getColor(float x, float y) const{
    this->_convertFromNewInterval(x, y);
    this->_correctYValue(y);

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
    this->_convertFromNewInterval(center.x, center.y);
    this->_correctYValue(center.y);
    if (this->m_InvertYAxis){
        startAngle *= -1.f;
        endAngle *= -1.f;
    }

    cv::ellipse(this->m_Image, center, cv::Size(radius, radius), 0.f, startAngle, endAngle, cv::Scalar(c.b, c.g, c.r), lineWidth);
}

void Window2D::floodFill(Point startingPoint, const Color &c) {
    this->_convertFromNewInterval(startingPoint.x, startingPoint.y);
    this->_correctYValue(startingPoint.y);

    cv::floodFill(this->m_Image, startingPoint, cv::Scalar(c.b, c.g, c.r));
}
void Window2D::drawCircle(cf::Point center, int radius, int lineWidth, const cf::Color& c){
    this->_convertFromNewInterval(center.x, center.y);
    this->_correctYValue(center.y);

    cv::circle(this->m_Image, center, radius, cv::Scalar(c.b, c.g, c.r), lineWidth);
}
void Window2D::drawRectangle(cf::Point p1, cf::Point p2, int lineWidth, const cf::Color& c){
    this->_convertFromNewInterval(p1.x, p1.y);
    this->_convertFromNewInterval(p2.x, p2.y);
    this->_correctYValue(p1.y);
    this->_correctYValue(p2.y);

    cv::rectangle(this->m_Image, p1, p2, cv::Scalar(c.b, c.g, c.r), lineWidth);
}
void Window2D::drawLine(cf::Point p1, cf::Point p2, int lineWidth, const cf::Color& c){
    this->_convertFromNewInterval(p1.x, p1.y);
    this->_convertFromNewInterval(p2.x, p2.y);
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

    this->_convertFromNewInterval(p1.x, p1.y);
    this->_convertFromNewInterval(p2.x, p2.y);
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

void Window2D::setNewInterval(const Interval& intervalX, const Interval& intervalY){
    this->m_IntervalX = intervalX;
    this->m_IntervalY = intervalY;

    if (intervalX.min != 0.f || intervalX.max != this->m_Image.cols - 1 ||
        intervalY.min != 0.f || intervalY.max != this->m_Image.rows - 1)
    {
        this->m_IntervalChanged = true;
    }
    else
        this->m_IntervalChanged = false;
}

void Window2D::resetInterval(){
    this->m_IntervalX.min = 0;
    this->m_IntervalX.max = this->m_Image.cols - 1;

    this->m_IntervalY.min = 0;
    this->m_IntervalY.max = this->m_Image.rows - 1;

    this->m_IntervalChanged = false;
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
    static const int helperLines_occurence = 5;
    static const float helperLine_adjustment = 2.5f;

    // draw complete x- and y-axis
    //
    // Note:
    //  opencv takes care of pixel positions out of image
    this->drawLine({this->m_IntervalX.min, 0.f}, {this->m_IntervalX.max, 0.f}, lineWidth, color);
    this->drawLine({0.f, this->m_IntervalY.min}, {0.f, this->m_IntervalY.max}, lineWidth, color);

    if (stepSize_x < 0.f)
        stepSize_x = (this->m_IntervalX.max - this->m_IntervalX.min) / 10.f;
    if (stepSize_y < 0.f)
        stepSize_y = (this->m_IntervalY.max - this->m_IntervalY.min) / 10.f;

    auto drawAxisInterceptions = [&](bool horizontalLine){
        float stepSize = horizontalLine ? stepSize_x : stepSize_y;
        int pixelLength = horizontalLine ? this->m_Image.rows : this->m_Image.cols;
        const cf::Interval& interval = horizontalLine ? this->m_IntervalX : this->m_IntervalY;

        int startPos = int(interval.min / stepSize);
        int iterationCounter = helperLines_occurence - std::abs(startPos % helperLines_occurence);

        float verticalLineHeight = (interval.max - interval.min) / float(pixelLength) * interceptLength * lineWidth;
        for (float start = startPos * stepSize;
             start <= interval.max;
             start += stepSize, ++iterationCounter)
        {
            // draw vertical line
            float adjusted_lineWidth = verticalLineHeight;
            if (iterationCounter == helperLines_occurence){
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


const cf::Interval& Window2D::getIntervalX() const{
    return this->m_IntervalX;
}
const cf::Interval& Window2D::getIntervalY() const{
    return this->m_IntervalY;
}

void Window2D::_correctYValue(float& y) const{
    if (this->m_InvertYAxis)
        y = (this->m_Image.rows - 1) - y;
}
void Window2D::_convertFromNewInterval(float& x, float& y) const{
    if (!this->m_IntervalChanged)
        return;

    x = std::round(Interval::translateIntervalPostion(this->m_IntervalX, Interval(0, this->m_Image.cols - 1), x));
    y = std::round(Interval::translateIntervalPostion(this->m_IntervalY, Interval(0, this->m_Image.rows - 1), y));
}
void Window2D::_convertToNewInterval(float &x, float &y) const{
    if (!this->m_IntervalChanged)
        return;

    x = Interval::translateIntervalPostion(Interval(0, this->m_Image.cols - 1), this->m_IntervalX, x);
    y = Interval::translateIntervalPostion(Interval(0, this->m_Image.rows - 1), this->m_IntervalY, y);
}


#ifdef _WIN32
#include <Windows.h>
#endif

void Window2D::_window2foreground() const {
#ifdef _WIN32
	static HWND windowHandle = (HWND)cvGetWindowHandle(this->m_WindowName);
	SetForegroundWindow(windowHandle);
#else
#endif
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

