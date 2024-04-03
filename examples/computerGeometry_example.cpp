#ifdef _WIN32
// enable exception handling for windows
// this requires 'int main(int, char**)' function definition
// therefore 'int main()' is dissabled
#define CFCG_EXCEPTION_HANDLING
#endif

#include "computerGeometry.hpp"
#include <iostream>

int main(int argc, char** argv) {
    // receive file name/path
    std::string filePath;
    if (argc < 2) {
        std::cout << "Please provide a .dat file, if you want a different dat file\n\n\n";
        filePath = GEOMETRY_FILE_PATH; // defined macro directing to <pathToLib>/ChaosAndFractal_Lib/geometry_files/
        filePath += "FIRST.DAT";
    } else
        filePath = argv[1];

    // read dat file as cf::PointVector type
    const std::vector<cf::PointVector> points = cf::readDATFile<cf::PointVector>(filePath);

    /// part 1
    // create coordinate system and draw all points and lines
    cf::WindowCoordinateSystem coordinateSystem(600, {-10.f, 350.f}, {-10.f, 350.f});
    coordinateSystem.drawAxis(cf::Color::BLACK, 10.f, 10.f);
    coordinateSystem.setWindowDisplayScale(1.5);

    for (const auto& point : points) {
        std::cout << point << std::endl;
        coordinateSystem.drawPoint(point);
    }

    // draw all lines
    for (size_t i = 1; i < points.size(); ++i)
        coordinateSystem.drawLine(points[i - 1], points[i]);
    coordinateSystem.drawLine(points.front(), points.back());
    coordinateSystem.show();

    // wait for user input and draw user points
    std::cout << "Please set a point\n\n";
    const cf::PointVector userPoint = coordinateSystem.waitMouseInput();
    coordinateSystem.drawPoint(userPoint, cf::Color::RED);
    coordinateSystem.show();

    // check wheter the point is within the triangle
    // function calcSide return true if point is above the line
    enum class PointDescription { LEFTSIDE_OF_LINE, RIGHTSIDE_OF_LINE, ON_THE_LINE };
    auto calcSide = [&userPoint](const cf::PointVector& line_p1, const cf::PointVector& line_p2) -> PointDescription {
        // normalization not required
        const double scalarProduct = line_p1 % line_p2 * userPoint; // % crossproduct, * dot product
        if (std::abs(scalarProduct) < 0.000001)                     // compare zero
            return PointDescription::ON_THE_LINE;
        else if (scalarProduct < 0.0)
            return PointDescription::RIGHTSIDE_OF_LINE;
        else
            return PointDescription::LEFTSIDE_OF_LINE;
    };
    const PointDescription des0 = calcSide(points[0], points[1]);
    const PointDescription des1 = calcSide(points[1], points[2]);
    const PointDescription des2 = calcSide(points[2], points[0]);

    if (des0 == des1 && des1 == des2) {
        // all 3 descriptions have the same value
        if (des0 == PointDescription::LEFTSIDE_OF_LINE)
            std::cout << "The user input point is within the triangle\n\n";
        else if (des0 == PointDescription::ON_THE_LINE)
            std::cout << "The user input point is on the triangle\n\n";
        else
            std::cout << "The user input point is outside the triangle\n\n";
    } else
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

    const cf::PointVector normal = points[2] % points[0];
    coordinateSystem.drawLinearEquation(normal, cf::Color::BLACK, cf::Window2D::LineType::DEFAULT, 2);
    // coordinateSystem.drawLinearEquation(normal[0], normal[1], normal[2]); // alternatively
    coordinateSystem.show();

    // draw line from point1 to point0 (from input file)
    //  using PointVector and a direction
    std::cout << "Press enter to draw a line between point1 and point0\n";
    coordinateSystem.waitKey();

    const cf::DirectionVector dir = points[1] - points[0];
    coordinateSystem.drawLinearEquation(
        points[1], dir, cf::Color::RED,
        cf::Window2D::LineType::DOT_1 /* cf::WindowCoordinateSystem::LineType::DOT_1 is also available */);
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

    // draw circle partitions around userPoint
    coordinateSystem.waitKey();
    coordinateSystem.drawCirclePart(userPoint, 23.f, 0.f, 90.f, cf::Color::RED, 2);
    coordinateSystem.drawCirclePart(userPoint, 23.f, 90.f, 360.f, cf::Color::BLUE, 2);
    coordinateSystem.show();

    // alternatively
    // Note:
    //  start/end have to be the same distance to center point!
    const cf::PointVector start = userPoint + cf::DirectionVector(17, -23, 0.0);
    const cf::PointVector end = userPoint + cf::DirectionVector(23, 17, 0.0);
    coordinateSystem.waitKey();
    coordinateSystem.drawCirclePart(userPoint, start, end, cf::Color::GREEN, 3, true /* small circle part */);
    coordinateSystem.drawCirclePart(userPoint, start, end, cf::Color::ORANGE, 3, false /* big circle part */);
    coordinateSystem.show();

    // end
    std::cout << "Press enter to finish the process\n";
    coordinateSystem.waitKey();
    return 0;
}
