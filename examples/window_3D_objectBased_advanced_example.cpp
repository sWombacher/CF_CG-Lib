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
    MyClass()
        : m_DataRead(&this->m_Spheres[0]), m_DataWrite(&this->m_Spheres[1]){}

    void draw(cf::Window3DObject& window){
        window.clear();
        window.drawAxis();

        // only read data from one buffer
        // don't access 'm_ReadData' more than once per draw call!
        const auto& spheres = *this->m_DataRead;

        for (const auto& e : spheres)
            window.drawSphere(e.pos, e.radius, e.color);
    }
    void setNextData(const std::vector<Sphere>& data){
        // write new sphere data
        *this->m_DataWrite = data;

        // ready new content to be drawn
        std::swap(this->m_DataRead, this->m_DataWrite);
    }
private:
    std::array<std::vector<Sphere>, 2> m_Spheres;

    std::vector<Sphere>* m_DataRead;
    std::vector<Sphere>* m_DataWrite;
};


int main(int argc, char** argv) {
    constexpr const size_t updateTime = 1000; // time in ms
    constexpr const float sphereRadius = 1.5f;
    constexpr const float circleRadius = 5.f;
    constexpr const float angle = 20.f;

    auto& window = cf::Window3DObject::createWindow3DObject(&argc, argv);
    window.printWindowUsage();

    MyClass classObj;
    window.setDrawingFunction([&](cf::Window3DObject& window){
        classObj.draw(window);
    });

    // draw circle
    std::vector<Sphere> data;
    for (float degree = 0.f; degree < 360.f; degree += angle){
        const float radian = cf::degree2radian(degree);
        Sphere sphere;
        sphere.color = cf::Color::RandomColor();
        sphere.radius = sphereRadius;
        sphere.pos.z = 0.f;
        sphere.pos.x = std::cos(radian) * circleRadius;
        sphere.pos.y = std::sin(radian) * circleRadius;

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
