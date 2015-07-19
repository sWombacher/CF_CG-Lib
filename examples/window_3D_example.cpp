

#include "window.h"


int main(int argc, char** argv) {

    cf::Window3D w(800, 600);

    w.init(&argc, argv);
    w.clear();

    w.drawCylinder(glm::vec3(0, 1, 0), glm::vec3(0, 0, 0));
    w.drawCylinder(glm::vec3(1, 0, 0), glm::vec3(0, 1, 0));
    w.draw();

    getchar();

    w.clear();
    w.drawAxis();
    w.drawCylinder(glm::vec3(0, -1, 0), glm::vec3(0, 0, 0), cf::Color(0, 0, 255));
    w.draw();


    getchar();
    return 0;
}
