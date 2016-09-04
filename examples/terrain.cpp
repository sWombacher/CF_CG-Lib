#include "windowRasterized.hpp"
#include "window3D.h"

struct MyWindow : public cf::Window3D {
    MyWindow(int* argc, char** argv, const cf::WindowRasterized& heightMap, const std::vector<cf::Color>& colorMapping)
        : cf::Window3D(argc, argv), m_HeightMap(heightMap), m_ColorMapping(colorMapping){}

    virtual void draw() override{
        this->clear();
        static const float minHeight = 0.f;
        static const float maxHeight = 10.f;
        static const float cubeSize  = 0.5f;

        // this loop could be paralllized,
        // however opengl has to syncronize these calls -> no performance gains
        for (int y = 0; y < this->m_HeightMap.getHeight(); ++y){
        for (int x = 0; x < this->m_HeightMap.getWidth() ; ++x){
            //
            // reminder opengl 1.x:
            //  reading direction from bottom to top
            //  here:
            //      1. cube will be drawn
            //      2. cube will be scaled
            //      3. cube will be translated
            //
            glPushMatrix(); // save current state
            {
                // calculate cube height
                float cubeHeight = this->m_HeightMap.getColor(x, y).r;
                cubeHeight /= 255.f;
                cubeHeight *= maxHeight - minHeight;
                cubeHeight += minHeight;

                // colorize each cube based on its height
                //glColor3f(cubeHeight / maxHeight, cubeHeight / maxHeight, cubeHeight / maxHeight); // gray scale
                const cf::Color& color = this->m_ColorMapping[this->m_HeightMap.getColor(x, y).r];
                glColor3f(float(color.r) / 255.f, float(color.g) / 255.f, float(color.b) / 255.f);

                glTranslatef((x - this->m_HeightMap.getWidth()  / 2) * cubeSize,
                             1.f,
                             (y - this->m_HeightMap.getHeight() / 2) * cubeSize);

                glScalef(1.f, cubeHeight, 1.f);
                glTranslatef(0.f, 0.5f, 0.f);
                glutSolidCube(cubeSize);
            }
            glPopMatrix(); // load original state
        }
        }
    }
private:
    const cf::WindowRasterized& m_HeightMap;
    const std::vector<cf::Color>& m_ColorMapping;
};

int main(int argc, char** argv){
    // read command line parameter
    // (image file of any format opencv does support, tested formats: jpeg, png, bmp)
    std::string filePath_heightMap;
    std::string filePath_palFile;
    if (argc < 3){
        std::cout << "Please provide a heightmap and a *.pal file, if you want to use your own files";
        filePath_heightMap = CHAOS_FILE_PATH;
        filePath_heightMap += "Heightmap.png";

        filePath_palFile = CHAOS_FILE_PATH;
        filePath_palFile += "Topo.pal";
    }
    else {
        filePath_heightMap = argv[1];
        filePath_palFile = argv[2];
    }

    cf::WindowRasterized img = cf::WindowRasterized(filePath_heightMap.c_str());
    // if you have very large images, you may want to resize that image for performance
    //img.resize(255, 255);

    std::vector<cf::Color> col = cf::readPaletteFromFile(filePath_palFile.c_str());
    img.show();
    img.waitKey(1000);

    MyWindow::showWindowUsage();
    MyWindow window(&argc, argv, img, col);

    // setup camera
    //  Type: STATIC_X_AXIS
    //  Set start height 10
    //  look at distance of 100
    window.setCamera(MyWindow::CameraType::STATIC_X_AXIS, glm::vec3(0, 10, 0), 100.f);
    return window.startDrawing();
}
