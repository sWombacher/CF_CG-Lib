#include "computerGeometry.hpp"
#include <iostream>

int main(){
    cf::WindowCoordinateSystem system = cf::WindowCoordinateSystem(600, {-2.f, 50.f}, {-2.f, 50.f});
    system.drawAxis(cf::Color::BLACK);
    system.show();

// receive 3 points by mouse clicks
// and draw a line from p0 to p1 and p1 to p3
    cf::PointVector p0, p1, p2;
    cf::PointVector line_01, line_12, line_halfAngle;
    system.waitMouseInput(p0[0], p0[1]); // receive p0
    system.drawPoint(p0);
    system.show();

    system.waitMouseInput(p1[0], p1[1]); // receive p1

    // calculate line p0 to p1 and draw it
    line_01 = p0 % p1;
    system.drawLinearEquation(line_01);
    system.drawPoint(p1);
    system.show();

    system.waitMouseInput(p2[0], p2[1]); // receive p2

    // calculate line p1 to p2 and draw it
    line_12 = p1 % p2;
    system.drawLinearEquation(line_12);
    system.drawPoint(p2);
    system.show();

// calculate a angle halfing line through p3
    // calculate angle
    // m = tan(alpha) where
    //  - m is the sloope a the line (and there for y by x)
    //  - alpha is the angle the line compared to the x-axis
    //
    float angle_01 = std::atan(line_01[1] / line_01[0]); // angle in radiant
    float angle_12 = std::atan(line_12[1] / line_12[0]); // angle in radiant
    float half_angle = (angle_01 + angle_12) / 2.f;      // angle in radiant

// calculate line_halfAngle with the line slope and point p1
    // set x value to 1 -> y value will be the slope
    line_halfAngle[0] = 1.f;
    line_halfAngle[1] = std::tan(half_angle) /* line slope */;

    // ax + by + c = 0  <- default equation for each line
    // we got x and y values, and p1 is element of the line
    //  c = -ax -by = -(ax + by)
    line_halfAngle[2] = -(line_halfAngle[0] * p1[0] + line_halfAngle[1] * p1[1]);

// calculate circles, which are intercepting line_01 in point0 and line_12 in point p2 respectively
    // calculate orthogonal lines through p0 and p2
    cf::PointVector orthogonal_0 = p0 - p1; // vector 01 is line's orthogonal vector
    orthogonal_0[2] = -(orthogonal_0[0] * p0[0] + orthogonal_0[1] * p0[1]);

    cf::PointVector orthogonal_2 = p1 - p2; // vector 12 is line's orthogonal vector
    orthogonal_2[2] = -(orthogonal_2[0] * p2[0] + orthogonal_2[1] * p2[1]);

    // calculate interception points of those lines and line_halfAngle
    cf::PointVector intercept_0 = orthogonal_0 % line_halfAngle;
    cf::PointVector intercept_2 = orthogonal_2 % line_halfAngle;
    intercept_0.normalize();
    intercept_2.normalize();

// calculate circle draw both circles
    // calculate radius of both circles
    // each radius is of length pX to interception point,
    //  where pX is p0 or p2 and their intercept point is intercept_0 and intercpt_2
    cf::DirectionVector dir0 = p0 - intercept_0;
    cf::DirectionVector dir2 = p2 - intercept_2;
    float radius_0 = std::sqrt(dir0[0] * dir0[0] + dir0[1] * dir0[1]);
    float radius_2 = std::sqrt(dir2[0] * dir2[0] + dir2[1] * dir2[1]);

    auto continueFunction = [&system](){
        system.show();
        std::cout << "Press enter to continue";
		cf::Console::waitKey();
    };

    system.drawLinearEquation(line_halfAngle, cf::Color::BLUE);
    continueFunction();

    system.drawPoint(intercept_0, cf::Color::RED);
    system.drawLinearEquation(orthogonal_0, cf::Color::RED);
    continueFunction();

    system.drawCircle(intercept_0, radius_0, cf::Color::RED);
    continueFunction();

    system.drawPoint(intercept_2, cf::Color::GREEN);
    system.drawLinearEquation(orthogonal_2, cf::Color::GREEN);
    continueFunction();

    system.drawCircle(intercept_2, radius_2, cf::Color::GREEN);
    system.show();
    std::cout << "Press enter to finish the process\n";
	cf::Console::waitKey();
    return 0;
}
