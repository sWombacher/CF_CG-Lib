#include "computerGeometry.hpp"
#include <iostream>

int main(int argc, char** argv){
// receive file name/path
    std::string filePath;
    if (argc < 2){
        std::cout << "Please provide a .dat file, if you want a different ifs file\n\n\n";
        filePath = GEOMETRY_FILE_PATH; // defined macro directing to <pathToLib>/ChaosAndFractal_Lib/geometry_files/
        filePath += "FIRST.DAT";
    }
    else
        filePath = argv[1];

    // read dat file as cf::PointVector type
    std::vector<cf::PointVector> points = cf::readDATFile<cf::PointVector>(filePath);

/// part 1
// create coordinate system and draw all points and lines
    cf::WindowCoordinateSystem coordinateSystem(800, {-10.f, 350.f}, {-10.f, 350.f});
    coordinateSystem.drawAxis(cf::Color::BLACK, 10.f, 10.f);

    for (const auto& point : points){
        std::cout << point << std::endl;
        coordinateSystem.drawPoint(point);
    }

    // draw all lines
    for (size_t i = 1; i < points.size(); ++i)
        coordinateSystem.drawLine(points[i-1], points[i]);
    coordinateSystem.drawLine(points.front(), points.back());
    coordinateSystem.show();

// wait for user input and draw user points
    std::cout << "Please set a point\n\n";
    cf::PointVector userPoint; // default values of PointVector: (0 0 1)
    coordinateSystem.waitMouseInput(userPoint[0], userPoint[1]);
    coordinateSystem.drawPoint(userPoint, cf::Color::RED);
    coordinateSystem.show();

// check wheter the point is within the triangle
    // function calcSide return true if point is above the line
    enum class PointDescription{ LEFTSIDE_OF_LINE, RIGHTSIDE_OF_LINE, ON_THE_LINE };
    auto calcSide = [&userPoint](const cf::PointVector& line_p1, const cf::PointVector& line_p2) -> PointDescription {
        // normalization not required
        float scalarProduct = line_p1 % line_p2 * userPoint; // % crossproduct, * dot product
        if (scalarProduct == 0.f)
            return PointDescription::ON_THE_LINE;
        else if (scalarProduct < 0.f)
            return PointDescription::RIGHTSIDE_OF_LINE;
        else
            return PointDescription::LEFTSIDE_OF_LINE;
    };
    PointDescription des0 = calcSide(points[0], points[1]);
    PointDescription des1 = calcSide(points[1], points[2]);
    PointDescription des2 = calcSide(points[2], points[0]);

    if (des0 == des1 && des1 == des2){
        // all 3 descriptions have the same value
        if (des0 == PointDescription::LEFTSIDE_OF_LINE)
            std::cout << "The user input point is within the triangle\n\n";
        else if (des0 == PointDescription::ON_THE_LINE)
            std::cout << "The user input point is on the triangle\n\n";
        else
            std::cout << "The user input point is outside the triangle\n\n";
    }
    else
        std::cout << "The user input point is outside the triangle\n\n";


/// part 2
    // reset coordianteSystem
    std::cout << "Press enter to continue with part 2 of example\n\n";
    coordinateSystem.waitKey();
    coordinateSystem.clear();
    coordinateSystem.drawAxis(cf::Color::BLACK, 10.f, 10.f);
    coordinateSystem.show();

// draw line from point2 to point0 (from input file)
//  using line normal
    std::cout << "Press enter to draw a line between point0 and point2\n" << std::flush;
    coordinateSystem.waitKey();
    cf::PointVector normal = points[2] % points[0];
    coordinateSystem.drawLinearEquation(normal, cf::Color::BLACK, cf::Window2D::LineType::DEFAULT, 2);
    //coordinateSystem.drawLinearEquation(normal[0], normal[1], normal[2]); // alternatively
    coordinateSystem.show();

// draw line from point1 to point0 (from input file)
//  using PointVector and a direction
    std::cout << "Press enter to draw a line between point1 and point0\n";
    coordinateSystem.waitKey();
    cf::DirectionVector dir = points[1] - points[0];
    coordinateSystem.drawLinearEquation(points[1], dir, cf::Color::RED, cf::Window2D::LineType::DOT_1 /* cf::WindowCoordinateSystem::LineType::DOT_1 is also available */);
    coordinateSystem.show();

// draw line from point1 to point2 (from input file)
//  using points directly
    std::cout << "Press enter to draw a line between point1 and point2\n" << std::flush;
    coordinateSystem.waitKey();
    coordinateSystem.drawLine(points[1], points[2], cf::Color::BLUE, cf::Window2D::LineType::DOT_DASH_0);
    coordinateSystem.show();

// draw circle around userPoint and fill it with a color
    std::cout << "Press enter to draw a circle (around user point)\n" << std::flush;
    coordinateSystem.waitKey();
    coordinateSystem.drawCircle(userPoint, 20.f, cf::Color::CYAN, -1 /* negative line width will fill the circle */);
    coordinateSystem.drawCircle(userPoint, 20.f, cf::Color::MAGENTA);
    coordinateSystem.drawPoint(userPoint);
    coordinateSystem.show();

    std::cout << "Press enter to finish the process\n";
    coordinateSystem.waitKey();
    return 0;
}
