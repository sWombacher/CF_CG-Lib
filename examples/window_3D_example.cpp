
#include "utils.h"
#include "window3D.h"

class MyWindow : public cf::Window3D {
public:
    MyWindow(int* argc, char** argv):cf::Window3D(argc, argv, 800, 600, "Example"){
        this->m_AngleAdjustment = 2.0f; // speed up  rotation
        this->m_CameraAdjustment= 0.1f; // slow down camera movement
    }
    virtual ~MyWindow() = default;
    virtual void draw() override{
        // clear all and redraw axis
        this->clear();
        this->drawAxis(2.f);

// create a cylinder
// direction and length of this vector will be drawn as a cylinder later on
        glm::vec4 dir(this->m_Length, 0, 0, 1);
        float radianValue = cf::degree2radian(this->m_Angle); // glm library uses radian format

        // rotation around x-axis,
        // since 'dir' lies on the x-axis the rotation doesn't do anything :)
        // change 'dir' to see a difference
        // glm::mat4x4 rotMat = glm::rotate(radianValue, glm::vec3(1, 0, 0));

        //glm::mat4x4 rotMat = glm::rotate(radianValue, glm::vec3(0, 1, 0)); // rotation around y-axis
        glm::mat4x4 rotMat = glm::rotate(radianValue, glm::vec3(0, 0, 1)); // rotation around z-axis
        dir = rotMat * dir;

// create a 2nd cylinder
// and rotate it 90° around z-axis and half its size
        glm::mat4x4 rotMat2 = glm::rotate(cf::degree2radian(90), glm::vec3(0, 0, 1)); // degree
        //glm::mat4x4 rotMat2 = glm::rotate(glm::pi<float>() / 2.f, glm::vec3(0, 0, 1)); // radian

        glm::vec4 dir2 = rotMat2 * dir;
        dir2 *= 0.5; // half the length of cylinder2

// draw both cylinders
        const cf::Color color  = cf::Color::ORANGE;
        const cf::Color color2 = cf::Color(0, 0, 255);

        this->drawCylinder(dir , this->m_StartPos      , this->m_Diameter        , color );
        this->drawCylinder(dir2, this->m_StartPos + dir, this->m_Diameter * 0.75f, color2);
    }
    void handleKeyboardInput(unsigned char key, int x, int y) override{
        printf("Key: %c pressed at mouse position: %d, %d\r", key, x, y);
        fflush(stdout);

        switch(key){
        // change cylinder positions
        case 'j': this->m_StartPos.x -= 0.1f; break;
        case 'l': this->m_StartPos.x += 0.1f; break;
        case 'k': this->m_StartPos.y -= 0.1f; break;
        case 'i': this->m_StartPos.y += 0.1f; break;

        // change cylinder thickness
        case 'n': this->m_Diameter *= 1.5f; break;
        case 'm': this->m_Diameter /= 1.5f; break;

        // change cylinder rotation
        case 'u': this->m_Angle -= 5.f; break;
        case 'o': this->m_Angle += 5.f; break;
        default : break;
        }

        // change cylinder length
        if (key > '0' && key <= '9')
            this->m_Length = key - '0';
    }

private:
    float m_Diameter = 0.1f;
    float m_Length = 1.f;
    float m_Angle = 0.f;
    glm::vec4 m_StartPos = glm::vec4(0.f, 0.f, 0.f, 0.f);
};


int main(int argc, char** argv){
// print camera usage
    MyWindow::printWindowUsage();

// print object movement options
	std::cout << std::endl << std::endl
		      << "Object movement:\n"
			  << "press:\ti/j/k/l\t to translate the object\n"
			  << "press:\tu/o    \t to   rotate  the object\n"
		      << "press: a number\t to    set    the objests length\n"
			  << "press:\tm/n    \t to   change  the objects diameter\n"
		      << std::endl;

// create window and set up camera
    MyWindow window(&argc, argv);

    //window.setCamera(MyWindow::CameraType::NONE);
    //window.setCamera(MyWindow::CameraType::STATIC_X_AXIS);
    //window.setCamera(MyWindow::CameraType::STATIC_Y_AXIS);
    //window.setCamera(MyWindow::CameraType::STATIC_Z_AXIS);
    window.setCamera(MyWindow::CameraType::ROTATION); // default value, you propably want to use this

// set draw type
    // default: 0 - draw after each key-input
    //window.setMaxFPS(60.f);

// start drawing
    // function returns when the "esc-key" is pressed
    return window.startDrawing();
}
