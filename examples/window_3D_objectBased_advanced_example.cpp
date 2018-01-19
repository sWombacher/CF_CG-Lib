#ifdef _WIN32
// enable exception handling for windows
// this requires 'int main(int, char**)' function definition
// therefore 'int main()' is dissabled
#define CFCG_EXCEPTION_HANDLING
#endif

#include "window3DObjectbased.h"

struct Sphere{
    glm::vec3 pos;
    float radius;
    cf::Color color;
};

class MyClass{
public:
    void operator()(cf::Window3DObject& window){
        window.clear();
        window.drawAxis();
        for (const auto& e : this->m_Spheres[this->m_DataRead])
            window.drawSphere(e.pos, e.radius, e.color);

        // signal 'swapBuffer' the end of the frame
        this->m_Signal.fireSignal();
    }

    void setNextData(const std::vector<Sphere>& data){
        this->m_Spheres[this->m_DataWrite] = data;
        this->_swapBuffer();
    }

private:
    void _swapBuffer(){
        std::swap(m_DataRead, m_DataWrite);

        // wait until current frame has been finished
        this->m_Signal.waitSignal();
    }
    size_t m_DataRead = 0;
    size_t m_DataWrite = 1;
    cf::SimpleSignal m_Signal;
    std::array<std::vector<Sphere>, 2> m_Spheres;
};


int main(int argc, char** argv) {
    constexpr const size_t updateTime = 1000; // time in ms
    constexpr const float sphereRadius = 1.5f;
    constexpr const float circleRadius = 5.f;
    constexpr const float angle = 20.f;

    auto& window = cf::Window3DObject::createWindow3DObject(&argc, argv);
    window.printWindowUsage();

    MyClass classObj;
    window.setDrawingFunction([&](cf::Window3DObject& window){ classObj(window); });

    // update frames all the time
    // required for redraw after 'window.waitKeyPressed(updateTime)'
    window.setMaxFPS(60.f);

    // draw circle
    std::vector<Sphere> data;
    for (float degree = 0.f; degree < 360.f; degree += angle){
        Sphere sphere;
        sphere.color = cf::Color::RandomColor();
        sphere.radius = sphereRadius;
        sphere.pos.z = 0.f;
        sphere.pos.x = std::cos(cf::degree2radian(degree)) * circleRadius;
        sphere.pos.y = std::sin(cf::degree2radian(degree)) * circleRadius;

        data.push_back(sphere);
        classObj.setNextData(data);
        window.waitKeyPressed(updateTime);
    }
    std::cout << "Finished" << std::endl;
    window.waitKeyPressed();

    // this function probably throws an exception/erros
    // sorry about that...
    window.exit();
    return 0;
}
