#include "windowVercorized.hpp"

#include <string>
#include <sstream>
#include <iostream>

int main() {
// read intervals from console
    cf::Interval range_x;
    std::cout << "Part 1: Demonstration of the correspondence between an \n";
    std::cout << "user defined float 2D-interval and an image matrix\n\n";

    std::cout << "Please provide a float interval for the x-axis:\n";
    std::cout << "(Min and max values between -10.0 and 10.0!)\n";
    std::cout << "Interval: x-min: "; range_x.min = cf::Console::readFloat();
    std::cout << "Interval: x-max: "; range_x.max = cf::Console::readFloat();
    std::cout << std::endl;

    cf::Interval range_y;
    std::cout << "Please provide a float interval for the y-axis:\n";
    std::cout << "(Min and max values between -10.0 and 10.0!)\n";
    std::cout << "Interval: y-min: "; range_y.min = cf::Console::readFloat();
    std::cout << "Interval: y-max: "; range_y.max = cf::Console::readFloat();
    cf::Console::clearConsole();


// create window and print mouse input
    cf::WindowVectorized window(800, range_x, range_y, "Hello world", cf::Color::ORANGE);
    std::cout << "Created window size:\n"
              << "Width : " << window.getWidth()  << std::endl
              << "Height: " << window.getHeight() << std::endl
                                                  << std::endl;
    window.drawAxis();
    window.show();
    cf::Point point1;

    std::cout << "Please set a point on the window" << std::endl;
    window.waitMouseInput(point1.x, point1.y);
    window.drawCircle(point1, 3, -1 /*fill circle*/, cf::Color::WHITE);
    window.show();

    std::cout << "Point coordinates in interval:\n"
              << "Interval: x-position: " << point1.x << std::endl
              << "Interval: y-position: " << point1.y << std::endl
              << std::endl;

    point1 = window.transformPoint_fromInterval_toImage(point1);
    std::cout << "Point coordinates in image (in pixel):\n"
              << "Image: i-position: " << std::round(point1.x) << std::endl
              << "Image: j-position: " << std::round(point1.y) << std::endl
              << std::endl;

    std::cout << "\n\nActivate image window and press enter to continue with part 2: drawing \n";
    window.waitKey();
    cf::Console::clearConsole();


// take 2 points from console and draw a line and a rectangle
//
// first   point: use interval position
// second  point: use pixel position
    std::cout << "Enter float coordinates within interval for first point! \n";
    std::cout << "Please provide a float value for x within the interval: " << range_x << ":\n";
    point1.x = cf::Console::readFloat();

    std::cout << std::endl;
    std::cout << "Please provide a float value for y within the interval: " << range_y << ":\n";
    point1.y = cf::Console::readFloat();

    std::cout << "Circle around first user-point\n";
    window.drawCircle(point1, 3, -1, cf::Color::BLUE);
    window.show();

    cf::Point point2;
    std::cout << std::endl << std::endl;
    std::cout << "Enter integer coordinates within image for second point! \n";
    std::cout << "Please provide an integer value for column i within the image [0-" << window.getWidth() - 1 << "]:\n";
    point2.x = cf::Console::readInt();

    std::cout << std::endl;
    std::cout << "Please provide an integer value for row j within the image [0-" << window.getHeight() - 1 << "]:\n";
    point2.y = cf::Console::readInt();

    // translate position "point2" to interval position
    point2 = window.transformPoint_fromImage_toInterval(point2);
    window.drawCircle(point2, 3, -1, cf::Color::BLUE);
    window.show();


// draw line and rectangle
	cf::Console::clearConsole();
    std::cout << "Press enter to draw a line\n";
    window.waitKey();
    window.drawLine(point1, point2, 2, cf::Color::BLACK);
    window.show();

    std::cout << "\n\n\nPress enter to draw a rectangle\n";
    window.waitKey();
    window.drawRectangle(point1, point2, 2, cf::Color::GREEN);
    window.show();


// draw circle from point with radius to point2
    // calculate radius using pytagora
    cf::Point vec_1_2 = point2 - point1;
    float intervalLength = std::sqrt(vec_1_2.x * vec_1_2.x + vec_1_2.y * vec_1_2.y);

    // convert length from interval length to pixel length
    int pixelLength = std::round(window.convert_intervalLength_to_pixelLength(intervalLength));

    // draw
    std::cout << "\n\n\nPress enter to draw a circle (point1 as center, point2 on circle line)\n";
    window.waitKey();
    window.drawCircle(point1, pixelLength, 2, cf::Color(127, 27, 127));
    window.show();


// draw half circle from point2 towards point
    // calculate angle of straigt line from point to point2 and the positive x-axis
    // this can be calculated by 'tan(slope)'
    float angle = cf::radian2degree(std::atan(vec_1_2.y / vec_1_2.x)); // atan returns radian, we need degree :)

    // if point2 is on the right side of point1 we have to adjust the angle by 180
    // reason:
    //  trigonemetric functions like atan are using nearest angle [-90°, 90°] -> in some cases "wrong"
    if (vec_1_2.x > 0.f)
        angle += 180.f;

    // draw part of the circle
    //  you can think of the angle as the drawn line
    //  from that line we need +/- 60 degrees to reach the other circle sides
    window.drawCriclePart(point2, pixelLength, angle - 60.f, angle + 60.f, 2, cf::Color::GREY);

    std::cout << "\n\n\nPress enter to draw a circle part (point2 center, point1 on circle line)\n";
    window.waitKey();
    window.show();

    std::cout << "\n\n\nPress enter to finish the process\n";
    window.waitKey();
    return 0;
}
