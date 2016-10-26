#include "windowRasterized.hpp"
#include <string>

static const int TERRAIN_WIDTH  = 256;
static const int TERRAIN_HEIGHT = 256;

int main(int argc, char** argv){
    std::string filePath;
    if (argc < 2){
        std::cout << "Please add a filename as a command line argument to specify output path.\n";
        filePath = "Heightmap.png";
    }
    else
        filePath = argv[1];

    // generate a simple heightmap, center color will be white,
    // corners will be black (all other points will be smoothed)
    cf::WindowRasterized window(TERRAIN_WIDTH, TERRAIN_HEIGHT);
    auto calculateDistance2Center = [&window](glm::vec2 p) -> float {
        const glm::vec2 distanceVector = glm::vec2(window.getWidth() / 2, window.getHeight() / 2) - p;
        return std::sqrt(distanceVector.x * distanceVector.x + distanceVector.y * distanceVector.y);
    };

    // calculate max distance (corner to center)
    const float maxDistance2center = calculateDistance2Center({0, 0});
    for (int y = 0; y < window.getHeight(); ++y){
    for (int x = 0; x < window.getWidth (); ++x){
        // calculate distance to center
        const float tmpDist = calculateDistance2Center({x, y});
		
		// set color based on distance to center
        cf::Color scaledColor = cf::Color::WHITE;
        scaledColor *= 1.f - tmpDist / maxDistance2center;
        window.setColor(x, y, scaledColor);
    }
    }
	
	// save heightmap
    window.saveImage(filePath.c_str());
    std::cout << "Finished generating terrain!" << std::endl;
    return 0;
}

