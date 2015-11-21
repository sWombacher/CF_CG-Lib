
/*
#include "window2D.h"
int main(int argc, char** argv) {
    cf::Window2D window(200, 100, "Chaos");
    window.setInvertYAxis(true);

    window.setColor(10, 10, cf::Color(0, 0, 255));
    window.drawCircle(cf::Point(100, 100), 20, -1, cf::Color(255, 0, 0));

    window.drawRectangle(cf::Point(0, 0), cf::Point(20, 20), -1, cf::Color(255, 0, 0));

    window.setWindowScale(1.f);
    window.show();

    window.setNewIntervall(cf::Intervall(1.f, 2.f), cf::Intervall(-0.1f, 1000.f));
    window.setWindowScale(5.f);

    float x, y;
    window.waitMouseInput(x, y);
    std::cout << "x: " << x << "\ty: " << y << std::endl;

    window.waitKey();
    return 0;
}
*/

#include "utils.h"
#include "window3D.h"

class MyWindow : public cf::Window3D {
public:
    MyWindow(int* argc, char** argv):cf::Window3D(argc, argv){
        this->m_AngleAdjustment = 2.0f; // speed up  rotation
        this->m_CameraAdjustment= 0.1f; // slow down camera movement
    }
    virtual ~MyWindow() = default;
    virtual void draw() override{
        this->clear();
        this->drawAxis(2.f);
        glm::vec4 dir(1, 0, 0, 1);
        float radiantValue = cf::degree2radiant(this->m_Angle);

        //glm::mat4x4 rotMat = glm::rotate(radiantValue, glm::vec3(1, 0, 0)); // rotation around x-axis
        //glm::mat4x4 rotMat = glm::rotate(radiantValue, glm::vec3(0, 1, 0)); // rotation around y-axis
        glm::mat4x4 rotMat = glm::rotate(radiantValue, glm::vec3(0, 0, 1)); // rotation around z-axis
        dir = dir * rotMat;

        glm::vec3 direction = glm::vec3(dir.x, dir.y, dir.z);

        //cf::Color color = Color(255, 255, 255);
        cf::Color color = cf::ColorType::ORANGE;
        this->drawCylinder(direction, this->m_StartPos, this->m_Diameter, color);
    }
    void myKeyboardCallback(unsigned char key, int x, int y){
        switch(key){
        case 'j': this->m_StartPos.x -= 0.1f; break;
        case 'l': this->m_StartPos.x += 0.1f; break;
        case 'k': this->m_StartPos.y -= 0.1f; break;
        case 'i': this->m_StartPos.y += 0.1f; break;

        case 'n': this->m_Diameter *= 1.5f; break;
        case 'm': this->m_Diameter /= 1.5f; break;

        case 'u': this->m_Angle += 5.f; break;
        case 'o': this->m_Angle -= 5.f; break;
        default : break;
        }
    }

private:
    float m_Angle = 0.f;
    float m_Diameter = 1.f;
    glm::vec3 m_StartPos = glm::vec3(0.f, 0.f, 0.f);
};


int main(int argc, char** argv){
    // show camera usage
    MyWindow::showWindowUsage();

    // create window and set up camera
    MyWindow window(&argc, argv);
    //window.setCamera(MyWindow::CameraType::STATIC_X_AXIS);
    //window.setCamera(MyWindow::CameraType::STATIC_Y_AXIS);
    //window.setCamera(MyWindow::CameraType::STATIC_Z_AXIS);
    window.setCamera(MyWindow::CameraType::ROTATION); // default value, you propably want to use this

    // get keyboard input
    window.setKeyboardCallbackFunction([&window](unsigned char key, int x, int y){
        std::cout << "Key: " << key << "\t\tpressed at mouse position: " << x << ' ' << y << "      \r" << std::flush;
        window.myKeyboardCallback(key, x, y);
    });

    // if you want to draw all the time
    // default: draw after each key-input
    //window.setMaxFPS(60.f);
    return window.startDrawing(); // function returns when the "esc-key" is pressed
}
