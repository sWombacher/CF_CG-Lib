#include "windowRasterized.hpp"
#include "window3D.h"

struct MyWindow : public cf::Window3D {
    MyWindow(int* argc, char** argv, const cf::WindowRasterized& heightMap)
        : cf::Window3D(argc, argv), m_HeightMap(heightMap){}

    virtual void draw() override{
        this->clear();
        static const float minHeight = 0.1f;
        static const float maxHeight = 20.f;
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
                glColor3f(cubeHeight / maxHeight, cubeHeight / maxHeight, cubeHeight / maxHeight);
                glTranslatef((x - this->m_HeightMap.getWidth()  / 2) * cubeSize,
                             (y - this->m_HeightMap.getHeight() / 2) * cubeSize,
                             1.f);

                glScalef(1.f, 1.f, cubeHeight);
                glTranslatef(0.f, 0.f, 0.5f);
                glutSolidCube(cubeSize);
            }
            glPopMatrix(); // load original state
        }
        }
    }
private:
    const cf::WindowRasterized& m_HeightMap;
};

int main(int argc, char** argv){
    // read command line parameter
    // (image file of any format opencv does support, tested formats: jpeg, png, bmp)
    std::string filePath;
    if (argc < 2){
        std::cout << "Please provide a heightmap file, if you want a different hightmap";
        filePath = CHAOS_FILE_PATH;
        filePath += "x1.png";
    }
    else
        filePath = argv[1];

    cf::WindowRasterized img = cf::WindowRasterized(filePath.c_str());
    img.show();
    img.waitKey(1000);

    MyWindow::showWindowUsage();
    MyWindow window(&argc, argv, img);

    // setup camera
    //  Type: Rotation (you probably want this)
    //  Look at origin
    //  look at distance of 50
    window.setCamera(MyWindow::CameraType::ROTATION, glm::vec3(0, 0, 0), 50.f);
    return window.startDrawing();
}
