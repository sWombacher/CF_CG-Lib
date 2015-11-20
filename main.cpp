
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


    std::string str = "bamlee\rpeter";
    cf::removeWindowsSpecificCarriageReturn(str);
    std::cout << str << std::endl;

    window.waitKey();
    return 0;
}
*/


#include "window3D.h"


class MyWindow : public cf::Window3D {
public:
    MyWindow(int* argc, char** argv):cf::Window3D(argc, argv){ }

    virtual void draw() override{
        this->clear();
        this->drawAxis(2.f);
        glm::vec4 dir(1, 0, 0, 1);
        float radiantValue = cf::Convert::degree2radiant(this->m_Angle);
        glm::mat4x4 rotMat = glm::rotate(radiantValue, glm::vec3(0, 0, 1));
        dir = dir * rotMat;
        this->drawCylinder(glm::vec3(dir.x, dir.y, dir.z), this->m_StartPos);
    }
private:
    float m_Angle = 0;
    glm::vec3 m_StartPos = glm::vec3(0.f, 0.f, 0.f);
};


int main(int argc, char** argv){
    MyWindow window(&argc, argv);
    //window.setCamera(cf::Window3D::CameraType::STATIC_X_AXIS);
    //window.setCamera(cf::Window3D::CameraType::STATIC_Y_AXIS);
    //window.setCamera(cf::Window3D::CameraType::STATIC_Z_AXIS);
    window.setCamera(cf::Window3D::CameraType::ROTATION);
    return window.startDrawing();
}

