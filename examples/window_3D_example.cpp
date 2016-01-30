
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
        this->clear();
        this->drawAxis(2.f);
        glm::vec4 dir(this->m_Length, 0, 0, 1);
        float radiantValue = cf::degree2radiant(this->m_Angle);

        //glm::mat4x4 rotMat = glm::rotate(radiantValue, glm::vec3(1, 0, 0)); // rotation around x-axis
        //glm::mat4x4 rotMat = glm::rotate(radiantValue, glm::vec3(0, 1, 0)); // rotation around y-axis
        glm::mat4x4 rotMat = glm::rotate(radiantValue, glm::vec3(0, 0, 1)); // rotation around z-axis
        dir = dir * rotMat;

        glm::vec3 direction = glm::vec3(dir.x, dir.y, dir.z);

        //cf::Color color = Color(255, 255, 255);
        cf::Color color = cf::Color::ORANGE;
        this->drawCylinder(direction, this->m_StartPos, this->m_Diameter, color);
    }
    void handleKeyboardInput(unsigned char key, int x, int y) override{
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
        if (key >= '0' && key <= '9') // adjust the length by typing a number key
            this->m_Length = key - '0';
    }

private:
    float m_Angle = 0.f;
    float m_Diameter = 1.f;
    float m_Length = 1.f;
    glm::vec3 m_StartPos = glm::vec3(0.f, 0.f, 0.f);
};


int main(int argc, char** argv){
    // show camera usage
    MyWindow::showWindowUsage();

	// print object movement options
	std::cout << "Object movement:\n"
			  << "press:\ti/j/k/l\t to translate the object\n"
			  << "press:\tu/o    \t to   rotate  the object\n"
		      << "press: a number\t to    set    the objests length\n"
			  << "press:\tm/n    \t to   change  the objects diameter\n"
		      << std::endl;

    // create window and set up camera
    MyWindow window(&argc, argv);
    //window.setCamera(MyWindow::CameraType::STATIC_X_AXIS);
    //window.setCamera(MyWindow::CameraType::STATIC_Y_AXIS);
    //window.setCamera(MyWindow::CameraType::STATIC_Z_AXIS);
    window.setCamera(MyWindow::CameraType::ROTATION); // default value, you propably want to use this

    // set draw type
    // default: 0 - draw after each key-input
    //window.setMaxFPS(60.f);
    return window.startDrawing(); // function returns when the "esc-key" is pressed
}
