#include "windowVercorized.hpp"

#include <string>
#include <sstream>
#include <iostream>

int main() {
// read intervalls from console
    cf::Intervall range_x;
    std::cout << "Please provide a Intervall for the x-axes:\n";
    std::cout << "Intervall min: "; range_x.min = cf::Console::readFloat();
    std::cout << "Intervall max: "; range_x.max = cf::Console::readFloat();
    std::cout << std::endl;

    cf::Intervall range_y;
    std::cout << "Please provide a Intervall for the y-axes:\n";
    std::cout << "Intervall min: "; range_y.min = cf::Console::readFloat();
    std::cout << "Intervall max: "; range_y.max = cf::Console::readFloat();
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

    std::cout << "Read intervall position:\n"
              << "Pos x: " << point1.x << std::endl
              << "Pos y: " << point1.y << std::endl
                                       << std::endl;

    point1 = window.transformPoint_fromImageIntervall(point1);
    std::cout << "Read image position (in pixel):\n"
              << "Pos x: " << std::round(point1.x) << std::endl
              << "Pos y: " << std::round(point1.y) << std::endl
                                                   << std::endl;

    std::cout << "\n\nPress enter to continue\n";
	cf::Console::waitKey();
    cf::Console::clearConsole();


// take 2 points from console and draw a line and a rectangle
//
// first   point: use interval position
// second  point: use pixel position
    std::cout << "Please provide a floatingpoint value within the intervall " << range_x << ":\n";
    point1.x = cf::Console::readFloat();

    std::cout << std::endl;
    std::cout << "Please provide a floatingpoint value within the intervall " << range_y << ":\n";
    point1.y = cf::Console::readFloat();
    window.drawCircle(point1, 3, -1, cf::Color::BLUE);
    window.show();

    cf::Point point2;
    std::cout << std::endl << std::endl;
    std::cout << "Please provide a integer value within the intervall [0-" << window.getWidth() - 1 << "]:\n";
    point2.x = cf::Console::readInt();

    std::cout << std::endl;
    std::cout << "Please provide a integer value within the intervall [0-" << window.getHeight() - 1 << "]:\n";
    point2.y = cf::Console::readInt();

    // translate position "point2" to intervall position
    point2 = window.transformPoint_toImgeIntervall(point2);
    window.drawCircle(point2, 3, -1, cf::Color::BLUE);
    window.show();


// draw line and rectangle
	cf::Console::clearConsole();
    std::cout << "Press enter to draw a line\n";
	cf::Console::waitKey();
    window.drawLine(point1, point2, 2, cf::Color::BLACK);
    window.show();

    std::cout << "\n\n\nPress enter to draw a rectangle\n";
	cf::Console::waitKey();
    window.drawRectangle(point1, point2, 2, cf::Color::GREEN);
    window.show();


// draw circle from point with radius to point2
    // calculate radius using pytagora
    cf::Point vec_1_2 = point2 - point1;
    float intervallLength = std::sqrt(vec_1_2.x * vec_1_2.x + vec_1_2.y * vec_1_2.y);

    // convert length from intervall length to pixel length
    int pixelLength = window.convert_intervallLength_to_pixelLength(intervallLength);

    // draw
    std::cout << "\n\n\nPress enter to draw a circle (point1 as center, point2 on circle line)\n";
	cf::Console::waitKey();
    window.drawCircle(point1, pixelLength, 2, cf::Color(127, 27, 127));
    window.show();


// draw half circle from point2 towards point
    // calculate angle of straigt line from point to point2 and the positive x-axis
    // this can be calculated by 'tan(slope)'
    float angle = cf::radiant2degree(std::atan(vec_1_2.y / vec_1_2.x)); // atan returns radiant, we need degree :)

    // if point2 is on the right side of point1 we have to adjust the angle by 180
    // reason:
    //  trigonemetric functions like atan are using nearest angle [-90°, 90°] -> in same cases "wrong"
    if (vec_1_2.x > 0.f)
        angle += 180.f;

    // draw part of the circle
    //  you can think of the angle as the drawn line
    //  from that line we need +/- 60 degrees to reach the other circle sides
    window.drawCriclePart(point2, pixelLength, angle - 60.f, angle + 60.f, 2, cf::Color::GREY);

    std::cout << "\n\n\nPress enter to draw a circle part (point2 center, point1 on circle line)\n";
	cf::Console::waitKey();
    window.show();

    std::cout << "\n\n\nPress enter to finish the process\n";
	cf::Console::waitKey();
    return 0;
}
