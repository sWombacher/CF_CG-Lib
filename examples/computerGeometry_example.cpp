#include "computerGeometry.hpp"
#include "windowVercorized.hpp"
#include <iostream>

int main(int argc, char** argv){
// receive file name/path
    std::string filePath;
    if (argc < 2){
        std::cout << "Please provide a .dat file, if you want a different ifs file\n\n\n";
        filePath = CHAOS_FILE_PATH; // defined macro directing to <pathToLib>/ChaosAndFractal_Lib/chaos_files
        filePath += "Life1.dat";
    }
    else
        filePath = argv[1];
    std::vector<cf::PointVector> points = cf::readDATFile(filePath);

    // print all points to console
    for (const auto& point : points)
        std::cout << point << std::endl;

// create coordinate system and draw all points
    char name[100] = "Hello world";
    cf::WindowVectorized coordinateSystem(800, {-2.f, 50.f}, {-2.f, 50.f}, name);
    coordinateSystem.drawAxis();
    coordinateSystem.drawLinearEquation(0.5f, 0.f);
    coordinateSystem.show();

    for (const auto& point : points)
        coordinateSystem.drawPoint(point);
    coordinateSystem.show();

    cf::PointVector straight(10.f, 10.f);
    cf::DirectionVector direction(1.f, 1.f);

    coordinateSystem.drawLinearEquation(straight, direction, cf::Color::BLUE);
    coordinateSystem.drawPoint(straight);
    coordinateSystem.show();

    // draw example circle with radius 10
    coordinateSystem.drawCircle(cf::PointVector(), 10.0f);
    coordinateSystem.show();

// draw circle part from first point in vector 'points' towards axis (first quadrant)
    float point_x = points[0].getX();
    float point_y = points[0].getY();
    float radius = std::sqrt(point_x * point_x + point_y * point_y); // pytagora

    coordinateSystem.drawCriclePart(cf::Point(), radius, 0.f, 90.f);
    coordinateSystem.show();

    cf::PointVector point(10.f, 10.f);
    cf::DirectionVector dir(2.f, 1.f);

    point += point;
    point.normalize();
    std::cout << point << std::endl;

    std::cout << "Press enter to finish" << std::endl;
    getchar();
    return 0;
}
