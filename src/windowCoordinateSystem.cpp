#include "windowCoordinateSystem.h"

namespace cf{

cf::WindowCoordinateSystem::WindowCoordinateSystem(int width, const cf::Interval &range_x, const cf::Interval &range_y, const char *windowName, const cf::Color &startColor)
    : Window2D(1, 1, windowName, startColor)
{
    this->setInvertYAxis(true);
    this->setInterval(range_x, range_y, width);
}

void WindowCoordinateSystem::setInterval(const Interval &range_x, const Interval &range_y, int width){
    this->setNewInterval(range_x, range_y);
    this->resize(width, WindowCoordinateSystem::_CALCULATE_HEIGHT(range_x, range_y, width));
}

void WindowCoordinateSystem::drawPoint(const Point &pos, const Color &color, int lineWidth){
    static const float pixelLength = 7.f;

    const float half_intervalLength = std::abs(this->convert_pixelLength_to_intervalLength(pixelLength)) / 2.f;
    cf::Window2D::drawLine({pos.x - half_intervalLength, pos.y - half_intervalLength},
    {pos.x + half_intervalLength, pos.y + half_intervalLength}, lineWidth, color);
    cf::Window2D::drawLine({pos.x - half_intervalLength, pos.y + half_intervalLength},
    {pos.x + half_intervalLength, pos.y - half_intervalLength}, lineWidth, color);
}

void WindowCoordinateSystem::drawLine(const Point &p1, const Point &p2, const Color &color, Window2D::LineType type, int lineWidth)
{
    if (type == cf::Window2D::LineType::DEFAULT)
        cf::Window2D::drawLine(p1, p2, lineWidth, color);
    else {
        if (lineWidth > 1)
            cf::Console::printWarning("Only default line type may use lineWidth parameter");
        cf::Window2D::drawSpecializedLine(p1, p2, type, color);
    }
}

void WindowCoordinateSystem::drawLinearEquation(const Point &pointVector, const glm::vec3 &drawingDirection, const Color &color, Window2D::LineType type, int lineWidth)
{
    if (std::abs(drawingDirection.z) > cf::WindowCoordinateSystem::ZERO_COMPARE)
        throw std::runtime_error("Error: Direction vector may only have values != 0 onf x and y coordinates");

    if (std::abs(drawingDirection.x) < cf::WindowCoordinateSystem::ZERO_COMPARE){
        if (std::abs(drawingDirection.y) < cf::WindowCoordinateSystem::ZERO_COMPARE){
            cf::Console::printWarning(__func__, " called with no direction x/y -> only the original point will be drawn\n"
                                                "If you use points at infinity try to switch to a implicit equation!");
            this->setColor(pointVector.x, pointVector.y, color);
            return;
        }
        this->drawLine({pointVector.x, this->m_IntervalY.min}, {pointVector.x, this->m_IntervalY.max}, color, type, lineWidth);
    }
    else{
        const float slope = drawingDirection.y / drawingDirection.x;
        const float yIntercept = pointVector.y - pointVector.x * slope;
        this->drawLinearEquation(slope, yIntercept, color, type, lineWidth);
    }
}

void WindowCoordinateSystem::drawLinearEquation(float a, float b, float c, const Color &color, Window2D::LineType type, int lineWidth)
{
    if (std::abs(b) < cf::WindowCoordinateSystem::ZERO_COMPARE) {
        if (std::abs(a) < cf::WindowCoordinateSystem::ZERO_COMPARE){
            if (std::abs(c) < cf::WindowCoordinateSystem::ZERO_COMPARE){
                cf::Console::printWarning(__func__, " called with a=b=c=0, this is ALSWAYS TRUE for all x/y pairs");
                this->clear(color);
            }
            else{
                cf::Console::printWarning(__func__, " called with with a=b=0 and c != 0, this is ALWAYS FALSE for all x/y pairs");
                return;
            }
        }
        // 0*y = 0
        // <=>  ax + c = 0
        // <=>  x = -c/a
        this->drawLine({-c/a, this->m_IntervalY.min}, {-c/a, this->m_IntervalY.max}, color, type, lineWidth);
    }
    else {
        //       0 =  ax + by + c
        // <=>  by = -ax -c
        //  =>   y = -a/b x -c/b
        this->drawLinearEquation(-a/b, -c/b, color, type, lineWidth);
    }
}

void WindowCoordinateSystem::drawLinearEquation(const glm::vec3 &vec, const Color &color, Window2D::LineType type, int lineWidth)
{
    this->drawLinearEquation(vec[0], vec[1], vec[2], color, type, lineWidth);
}

void WindowCoordinateSystem::drawLinearEquation(float slope, float yIntercept, const Color &color, Window2D::LineType type, int lineWidth)
{
    cf::Point p1, p2;
    std::vector<cf::Point> points;

    // check crossing points with x min/max
    auto addPoint_horizontal = [&](float xValue){
        cf::Point point(xValue, 0.f);
        point.y = slope * point.x + yIntercept;
        if (point.y >= this->m_IntervalY.min && point.y <= this->m_IntervalY.max)
            points.push_back(point);
    };
    addPoint_horizontal(this->m_IntervalX.min);
    addPoint_horizontal(this->m_IntervalX.max);

    // check crossing points with y min/max
    auto addPoint_vertical = [&](float yValue){
        cf::Point point(0.f, yValue);
        point.x = (point.y - yIntercept) / slope;
        if (point.x >= this->m_IntervalX.min && point.x <= this->m_IntervalX.max)
            points.push_back(point);
    };
    addPoint_vertical(this->m_IntervalY.min);
    addPoint_vertical(this->m_IntervalY.max);

    // get actual points
    if (points.size() <= 1){
        if (points.size())
            this->drawPoint(points[0], color);
        return;
    }
    if (points.size() > 2){
        // remove all duplicate points
        for (size_t i = 0; i < points.size(); ++i){
            const cf::Point& current = points[i];
            for (const auto& e : points){
                if (&current == &e)
                    continue;

                if (current == e){
                    points.erase(points.begin() + int(i));
                    --i;
                    break;
                }
            }
        }
        if (points.size() != 2)
            throw std::runtime_error(std::string("Error: In function \"") + __func__ + "\" this shouldn't have happened :)");
    }
    p1 = points[0];
    p2 = points[1];

    // draw
    if (type == cf::Window2D::LineType::DEFAULT)
        cf::Window2D::drawLine(p1, p2, lineWidth, color);
    else {
        if (lineWidth > 1)
            cf::Console::printWarning("Only default line type may use lineWidth parameter");
        cf::Window2D::drawSpecializedLine(p1, p2, type, color);
    }
}

void WindowCoordinateSystem::drawCircle(const Point &center, float radius, const Color &color, int lineWidth){
    int pixelRadius = int(std::round(this->convert_intervalLength_to_pixelLength(radius)));
    if (pixelRadius <= 0)
        pixelRadius = 1;
    cf::Window2D::drawCircle(center, pixelRadius, lineWidth, color);
}

float WindowCoordinateSystem::convert_pixelLength_to_intervalLength(float pixelLength) const {
    return (this->m_IntervalX.max - this->m_IntervalX.min) / float(this->m_Image.cols) * pixelLength;
}

float WindowCoordinateSystem::convert_intervalLength_to_pixelLength(float intervalLength) const {
    return float(this->m_Image.cols) / (this->m_IntervalX.max - this->m_IntervalX.min) * intervalLength;
}

void WindowCoordinateSystem::drawCirclePart(const Point &center, float radius, float startAngle, float endAngle, const Color &color, int lineWidth){
    const int pixelRadius = static_cast<int>(std::round(this->convert_intervalLength_to_pixelLength(radius)));
    Window2D::drawCirclePart(center, pixelRadius, startAngle, endAngle, lineWidth, color);
}

void WindowCoordinateSystem::drawCirclePart(const Point &center, const Point &p0, const Point &p1, const Color &color, int lineWidth, bool smallerAngle){
    const cf::Point dir0 = p0 - center;
    const cf::Point dir1 = p1 - center;
    const float tmp_rad0 = std::sqrt(dir0.x * dir0.x + dir0.y * dir0.y);
    const float tmp_rad1 = std::sqrt(dir1.x * dir1.x + dir1.y * dir1.y);
    if (std::abs(tmp_rad0 - tmp_rad1) > 0.0001f)
        throw std::runtime_error("Error: provided points have different distances towards the center");

    auto calculateAngle = [&center](const cf::Point& p) -> float{
        const cf::Point tmp = p - center;
        if (std::abs(tmp.x) < cf::WindowCoordinateSystem::ZERO_COMPARE){
            if (tmp.y > 0.f)
                return 90.f;
            else if (tmp.y < 0.f)
                return 270.f;
            else
                throw std::runtime_error("Error: Center and a point are equal");
        }
        const float radiant = std::atan(tmp.y / tmp.x);
        float degree = cf::radian2degree(radiant);
        if (tmp.x < 0.f)
            degree -= 180.f;
        while (degree < 0.f)
            degree += 360.f;
        return degree;
    };
    const float radius = tmp_rad0;
    float startAngle = calculateAngle(p0);
    float endAngle = calculateAngle(p1);

    startAngle = std::fmod(startAngle, 360.f);
    endAngle = std::fmod(endAngle, 360.f);
    if ((std::abs(endAngle - startAngle) > 180.f) == smallerAngle){
        auto& angle = startAngle < endAngle ? endAngle : startAngle;
        angle -= 360.f;
    }
    this->drawCirclePart(center, radius, startAngle, endAngle, color, lineWidth);
}

int WindowCoordinateSystem::_CALCULATE_HEIGHT(const Interval &range_x, const Interval &range_y, int width){
    float diff_y = range_y.max - range_y.min;
    float diff_x = range_x.max - range_x.min;
    return int(width * (diff_y / diff_x));
}

}

const float cf::WindowCoordinateSystem::ZERO_COMPARE = 0.000001f;

