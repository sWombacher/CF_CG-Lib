#ifdef _WIN32
// enable exception handling for windows
// this requires 'int main(int, char**)' function definition
// therefore 'int main()' is dissabled
#define CFCG_EXCEPTION_HANDLING
#endif

#include "window3DObjectbased.h"
#include <vector>

void simpleDrawingFunction(cf::Window3DObject& window) {
    window.clear();
    window.drawAxis();
    window.drawCube(glm::vec3(0.f, 0.f, 0.f), 2.5f, cf::Color::ORANGE);
}
void simpleKeyboardHandler(unsigned char key, int x, int y) {
    std::cout << "Key pressed: " << key << ", very good!\n"
              << "Key was pressed at: " << x << ' ' << y << "\n\n"
              << std::flush;
}

class MyClass {
  public:
    // draw functon
    void draw(cf::Window3DObject& window) {
        window.clear();
        window.drawAxis();
        window.drawCylinder(glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 0.f), 1.f, cf::Color::GREEN);
    }

    // key input handler
    void handleInput(unsigned char key, int x, int y) {
        std::cout << "Key pressed: " << key << ", at position: " << x << "  " << y << std::endl;
    }
};

int main(int argc, char** argv) {
    auto& window = cf::Window3DObject::createWindow3DObject(&argc, argv);
    window.printWindowUsage();

    // use a function pointer as drawing/keyinput function
    window.setDrawingFunction(simpleDrawingFunction);
    window.setKeyboardInputFunction(simpleKeyboardHandler);
    std::cout << "Press any key to continue (3D Windiw)" << std::endl;
    window.waitKeyPressed();

    // use a class as a drawing/keyinput function
    MyClass obj;
    window.setDrawingFunction([&](cf::Window3DObject& window){
        obj.draw(window);
    });
    window.setKeyboardInputFunction([&](unsigned char k, int x, int y){
        obj.handleInput(k, x, y);
    });
    std::cout << "Press any key to exit (3D Window)" << std::endl;
    window.waitKeyPressed();

    // this function probably throws an exception/erros
    // sorry about that...
    window.exit();
    return 0;
}
