#include "window3D.h"

#include <chrono>
#include <glm/gtx/quaternion.hpp>
#include <thread>

namespace cf {

Window3D::Window3D(int* argc, char** argv, int width, int height, const char* title) : m_Width(width), m_Height(height) {
    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(this->m_Width, this->m_Height);

    this->m_WindowID = glutCreateWindow(title);
    glLoadIdentity(); // Reset

    // Set the viewport to cover the new window
    glViewport(0, 0, this->m_Width, this->m_Height);

    // Set the aspect ratio of the clipping volume to match the viewport
    glMatrixMode(GL_PROJECTION); // To operate on the Projection matrix

    // Enable perspective projection with fovy, aspect, zNear and zFar

    // Compute aspect ratio of the new window
    GLdouble aspect = GLdouble(this->m_Width) / GLdouble(this->m_Height);
    gluPerspective(45.0, aspect, 0.1, 1000.0);

    this->m_AdditionalKeyboardCallback = [this](unsigned char key, int x, int y) { this->handleKeyboardInput(key, x, y); };
    this->m_AdditionalMouseMotionCallback = [this](MouseButton button, int x, int y) {
        return this->handleMousePressedMovement(button, x, y);
    };
    this->m_AdditionalMouseClickCallback = [this](MouseButton button, MouseButtonEvent event, int x, int y) {
        this->handleMousePressEvent(button, event, x, y);
    };

    // enable lighting by default
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
}
Window3D::~Window3D() { glutDestroyWindow(this->m_WindowID); }

void Window3D::printWindowUsage() {
    std::cout
#ifndef __APPLE__
        << "Close window:\n"
        << "esc-key\n\n"
#endif
        << "Camera usage:\n"
        << "a/d: camera movement left/right\n"
        << "w/s: camera movement up/down\n"
        << "f/r: move camera closer/further away\n\n"
        << "Rotation-Type specific movements:\n"
        << "q/e: rotation y-axis\n"
        << "y/c: rotation x-axis\n"
        << std::endl;
}

static Window3D* windowPtr = nullptr;
void Window3D::_WindowResizeEvent(int w, int h) {
    windowPtr->m_Width = w;
    windowPtr->m_Height = h;
    windowPtr->_AdjustCamera();
}

void Window3D::_DrawingFunction() {
    if (!windowPtr)
        throw std::runtime_error("Error: window ptr not set!");

    static const GLfloat position[] = {-5.f, 15.0f, 10.0f, 0.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    windowPtr->draw();
    glutSwapBuffers();
    glFlush();

    if (windowPtr->m_MaxFPS > 0.f) {
        using namespace std::chrono;
        system_clock::time_point current = system_clock::now(), next;
        auto timePerFrameInMS = int((1.f / windowPtr->m_MaxFPS) * 1000.f);
        next = system_clock::now();
        int numMilliseconds = int(duration_cast<milliseconds>(next - current).count());
        if (numMilliseconds < timePerFrameInMS)
            std::this_thread::sleep_for(milliseconds(timePerFrameInMS - numMilliseconds));

        glutTimerFunc(0, [](int) { glutPostRedisplay(); }, 1);
    }
}
void Window3D::_KeyboardCallbackFunction(unsigned char key, int x, int y) {
    if (!windowPtr)
        throw std::runtime_error("Error: window ptr not set!");

    // user specified function
    if (windowPtr->m_AdditionalKeyboardCallback)
        windowPtr->m_AdditionalKeyboardCallback(key, x, y);

    glm::vec3 left, up;
    switch (windowPtr->m_CameraType) {
    case Window3D::CameraType::NONE:
        // nothing to do!
        break;
    case Window3D::CameraType::ROTATION: {
        glm::vec3 cameraPos(0.f, 0.f, windowPtr->m_LookAtDistance);
        up = glm::vec3(0.f, 1.f, 0.f);

        glm::vec3 angles(cf::degree2radian(windowPtr->m_RotationAngle_X), cf::degree2radian(windowPtr->m_RotationAngle_Y), 0.f);
        glm::quat rot(angles);

        cameraPos = glm::rotate(rot, cameraPos);
        up = glm::rotate(rot, up);
        left = glm::cross(cameraPos, up);
    } break;
    case Window3D::CameraType::FREE_MOVEMENT:
        // this will directly be handled on key-switch case
        break;
    case Window3D::CameraType::STATIC_X_AXIS:
        left = glm::vec3(0.f, 0.f, 1.f);
        up = glm::vec3(0.f, 1.f, 0.f);
        break;
    case Window3D::CameraType::STATIC_Y_AXIS:
        up = glm::vec3(1.f, 0.f, 0.f);
        left = glm::vec3(0.f, 0.f, -1.f);
        break;
    case Window3D::CameraType::STATIC_Z_AXIS:
        left = glm::vec3(-1.f, 0.f, 0.f);
        up = glm::vec3(0.f, 1.f, 0.f);
        break;
    default:
        throw std::runtime_error("Unknown camera type");
    }

    // adjust left/up vectors to correct length
    left = glm::normalize(left);
    left *= windowPtr->m_CameraAdjustment * windowPtr->m_LookAtDistance / 10.f;

    up = glm::normalize(up);
    up *= windowPtr->m_CameraAdjustment * windowPtr->m_LookAtDistance / 10.f;

    switch (key) {
    case 'a':
        windowPtr->m_LookAt += left;
        windowPtr->m_FreeCamera_position += glm::cross(windowPtr->m_FreeCamera_UpVector, windowPtr->m_FreeCamera_LookDirection);
        break;
    case 'd':
        windowPtr->m_LookAt -= left;
        windowPtr->m_FreeCamera_position -= glm::cross(windowPtr->m_FreeCamera_UpVector, windowPtr->m_FreeCamera_LookDirection);
        break;

    case 'w':
        windowPtr->m_LookAt += up;
        windowPtr->m_FreeCamera_position += windowPtr->m_FreeCamera_UpVector;
        break;
    case 's':
        windowPtr->m_LookAt -= up;
        windowPtr->m_FreeCamera_position -= windowPtr->m_FreeCamera_UpVector;
        break;

    case 'c': {
        windowPtr->m_RotationAngle_X += windowPtr->m_AngleAdjustment;

        glm::vec3 rotAxis = glm::cross(windowPtr->m_FreeCamera_UpVector, windowPtr->m_FreeCamera_LookDirection);
        windowPtr->m_FreeCamera_UpVector =
            glm::rotate(windowPtr->m_FreeCamera_UpVector, cf::degree2radian(+windowPtr->m_AngleAdjustment), rotAxis);
        windowPtr->m_FreeCamera_LookDirection =
            glm::rotate(windowPtr->m_FreeCamera_LookDirection, cf::degree2radian(+windowPtr->m_AngleAdjustment), rotAxis);
    } break;
    case 'y': {
        windowPtr->m_RotationAngle_X -= windowPtr->m_AngleAdjustment;

        glm::vec3 rotAxis = glm::cross(windowPtr->m_FreeCamera_UpVector, windowPtr->m_FreeCamera_LookDirection);
        windowPtr->m_FreeCamera_UpVector =
            glm::rotate(windowPtr->m_FreeCamera_UpVector, cf::degree2radian(-windowPtr->m_AngleAdjustment), rotAxis);
        windowPtr->m_FreeCamera_LookDirection =
            glm::rotate(windowPtr->m_FreeCamera_LookDirection, cf::degree2radian(-windowPtr->m_AngleAdjustment), rotAxis);
    } break;

    case 'e':
        windowPtr->m_RotationAngle_Y += windowPtr->m_AngleAdjustment;
        windowPtr->m_FreeCamera_LookDirection =
            glm::rotate(windowPtr->m_FreeCamera_LookDirection, cf::degree2radian(-windowPtr->m_AngleAdjustment),
                        windowPtr->m_FreeCamera_UpVector);
        break;
    case 'q':
        windowPtr->m_RotationAngle_Y -= windowPtr->m_AngleAdjustment;
        windowPtr->m_FreeCamera_LookDirection =
            glm::rotate(windowPtr->m_FreeCamera_LookDirection, cf::degree2radian(+windowPtr->m_AngleAdjustment),
                        windowPtr->m_FreeCamera_UpVector);
        break;

    case 'r':
        // windowPtr->m_LookAtDistance += windowPtr->m_DistAdjustment;
        windowPtr->_ZoomCamera(true);
        windowPtr->m_FreeCamera_position -= windowPtr->m_FreeCamera_LookDirection;
        break;
    case 'f':
        // windowPtr->m_LookAtDistance -= windowPtr->m_DistAdjustment;
        windowPtr->_ZoomCamera(false);
        windowPtr->m_FreeCamera_position += windowPtr->m_FreeCamera_LookDirection;
        break;

#ifndef __APPLE__
    case 27 /*esc key*/:
        glutLeaveMainLoop();
        break;
#endif
    default:
        break;
    }
    if (windowPtr->m_LookAtDistance == 0.f)
        windowPtr->m_LookAtDistance = 0.0001f;

    windowPtr->_AdjustCamera();
}

void Window3D::handleKeyboardInput(unsigned char, int, int) {}
bool Window3D::handleMousePressedMovement(MouseButton, int, int) { return false; }
void Window3D::handleMousePressEvent(MouseButton, MouseButtonEvent, int, int) {}

static int _MouseCtlLastX = -1, _MouseCtlLastY = -1, _MouseCtlBtn = -1;
static bool _MouseCtlLastZoomMode = false;
void Window3D::_MouseCtlClickCallbackFunction(int button, int press, int y, int x) {
    _MouseCtlLastX = _MouseCtlLastY = -1;
    _MouseCtlBtn = button;

    if (button == 3) {
        // windowPtr->m_LookAtDistance -= windowPtr->m_DistAdjustment;
        windowPtr->_ZoomCamera(false);
        windowPtr->_AdjustCamera();
    } else if (button == 4) {
        // windowPtr->m_LookAtDistance += windowPtr->m_DistAdjustment;
        windowPtr->_ZoomCamera(true);
        windowPtr->_AdjustCamera();
    } else
        _MouseCtlLastZoomMode = button == 2; // 0 = left, 1 = middle, 2 = right

    if (windowPtr->m_AdditionalMouseClickCallback)
        windowPtr->m_AdditionalMouseClickCallback(
            static_cast<Window3D::MouseButton>(button),
            (press == GLUT_DOWN) ? Window3D::MouseButtonEvent::PRESSED : Window3D::MouseButtonEvent::RELEASED, x, y);
}

void Window3D::_MouseCtlMotionCallbackFunction(int y, int x) {
    bool preventDefault = false;
    if (windowPtr->m_AdditionalMouseMotionCallback)
        preventDefault = windowPtr->m_AdditionalMouseMotionCallback(static_cast<Window3D::MouseButton>(_MouseCtlBtn), x, y);

    if (!preventDefault) {
        if (_MouseCtlLastZoomMode) {
            windowPtr->m_LookAtDistance += (_MouseCtlLastX > -1 ? (_MouseCtlLastX - x) / glm::pi<float>() : 0.f);
        } else {
            windowPtr->m_RotationAngle_X += (_MouseCtlLastX > -1 ? _MouseCtlLastX - x : 0.f);
            windowPtr->m_RotationAngle_Y += (_MouseCtlLastY > -1 ? _MouseCtlLastY - y : 0.f);
        }
        _MouseCtlLastY = y;
        _MouseCtlLastX = x;
    }
    windowPtr->_AdjustCamera();
}

int Window3D::startDrawing() {
    windowPtr = this;
    glutKeyboardFunc(_KeyboardCallbackFunction);
    glutDisplayFunc(_DrawingFunction);
    glutMotionFunc(_MouseCtlMotionCallbackFunction);
    glutMouseFunc(_MouseCtlClickCallbackFunction);
    glutReshapeFunc(_WindowResizeEvent);

    // enable light 0
    // this only works if 'GL_LIGHTING' is enabled (default)
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);

    // glColorMaterial(GL_FRONT_AND_BACK, GL_EMISSION);
    glEnable(GL_COLOR_MATERIAL);

    // enable z buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    glutMainLoop();
    return 0; // never reached
}

void Window3D::clear(const Color& color) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(color.r, color.g, color.b, 255);
}

void Window3D::drawCylinder(const glm::vec3& drawingDirection, const glm::vec3& position, float diameter,
                            const Color& color) const {
    // set drawing angles
    const glm::vec3 startDir = glm::vec3(1.f, 0.f, 0.0001f);
    const glm::vec3 dir = glm::normalize(drawingDirection);

    const glm::vec3 rotVec = glm::normalize(glm::cross(startDir, dir));

    const cf::Color c = Window3D::_AdjustColorOpenGL(color);
    glPushMatrix();
    {
        glColor3b(char(c.r), char(c.g), char(c.b));
        glTranslatef(position.x, position.y, position.z);

        float angle = glm::angle(startDir, dir) / glm::pi<float>() * 180.f;
        glRotatef(angle, rotVec.x, rotVec.y, rotVec.z);

        // start rotations
        glRotatef(90, 0, 1.f, 0);
        glScalef(diameter, diameter, diameter);

#ifndef __APPLE__
        glutSolidCylinder(1.0, double(glm::length(drawingDirection) / diameter), 10, 10);
#else
        glScalef(1.f, 1.f, glm::length(drawingDirection) / diameter);
        glTranslatef(0.f, 0.f, 0.5f);
        glutSolidCube(1.f);
        glRotatef(45.f, 0.f, 0.f, 1.f);
        glutSolidCube(1.f);
#endif
    }
    glPopMatrix();
}

void Window3D::drawCylinder(const glm::vec4& drawingDirection, const glm::vec4& position, float diameter,
                            const Color& color) const {
    glm::vec3 dir = {drawingDirection.x, drawingDirection.y, drawingDirection.z};
    glm::vec3 pos = {position.x, position.y, position.z};
    this->drawCylinder(dir, pos, diameter, color);
}
void Window3D::drawCylinder(const glm::vec4& drawingDirection, const glm::vec3& position, float diameter,
                            const Color& color) const {
    glm::vec3 dir = {drawingDirection.x, drawingDirection.y, drawingDirection.z};
    this->drawCylinder(dir, position, diameter, color);
}
void Window3D::drawCylinder(const glm::vec3& drawingDirection, const glm::vec4& position, float diameter,
                            const Color& color) const {
    glm::vec3 pos = {position.x, position.y, position.z};
    this->drawCylinder(drawingDirection, pos, diameter, color);
}

// Groch
void Window3D::drawSphere(const glm::vec3& position, float diameter, const Color& color) const {
    const cf::Color c = Window3D::_AdjustColorOpenGL(color);
    glPushMatrix();
    {
        glColor3b(char(c.r), char(c.g), char(c.b));
        glTranslatef(position.x, position.y, position.z);
        glScalef(diameter, diameter, diameter);
        glutSolidSphere(1.0, 10, 10);
    }
    glPopMatrix();
}

// Groch
void Window3D::drawCube(const glm::vec3& position, float size, const Color& color) const {
    const cf::Color c = Window3D::_AdjustColorOpenGL(color);
    glPushMatrix();
    {
        glColor3b(char(c.r), char(c.g), char(c.b));
        glTranslatef(position.x, position.y, position.z);
        glScalef(size, size, size);
        glutSolidCube(1.0);
    }
    glPopMatrix();
}

void Window3D::_AdjustCamera() {
    glLoadIdentity(); // Reset

    // Compute aspect ratio of the new window
    GLdouble aspect = GLdouble(this->m_Width) / GLdouble(this->m_Height);

    // Set the viewport to cover the new window
    glViewport(0, 0, this->m_Width, this->m_Height);

    // Set the aspect ratio of the clipping volume to match the viewport
    glMatrixMode(GL_PROJECTION); // To operate on the Projection matrix

    // Enable perspective projection with fovy, aspect, zNear and zFar
    gluPerspective(45.0, aspect, 0.1, 1000.0);

    switch (this->m_CameraType) {
    case CameraType::NONE:
        break;
    case CameraType::ROTATION: {
        glm::vec4 cameraPos(0, 0, this->m_LookAtDistance, 1);
        glm::vec3 angles(cf::degree2radian(this->m_RotationAngle_X), cf::degree2radian(this->m_RotationAngle_Y), 0.f);
        glm::quat rot(angles);
        cameraPos = glm::rotate(rot, cameraPos);

        glm::vec3 lookUpVector(0.f, 1.f, 0.f);
        lookUpVector = glm::rotate(rot, lookUpVector);
        gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z, this->m_LookAt.x, this->m_LookAt.y, this->m_LookAt.z, lookUpVector.x,
                  lookUpVector.y, lookUpVector.z);
    } break;
    case CameraType::FREE_MOVEMENT: {
        const glm::vec3& pos = this->m_FreeCamera_position;
        const glm::vec3& up = this->m_FreeCamera_UpVector;
        const glm::vec3& dir = this->m_FreeCamera_LookDirection;
        gluLookAt(pos.x, pos.y, pos.z, pos.x + dir.x, pos.y + dir.y, pos.z + dir.z, up.x, up.y, up.z);
    } break;
    case CameraType::STATIC_X_AXIS:
        gluLookAt(this->m_LookAt.x + this->m_LookAtDistance, this->m_LookAt.y, this->m_LookAt.z,
                  this->m_LookAt.x + this->m_LookAtDistance - 1.f, this->m_LookAt.y, this->m_LookAt.z, 0, 1, 0);
        break;
    case CameraType::STATIC_Y_AXIS:
        gluLookAt(this->m_LookAt.x, this->m_LookAt.y + this->m_LookAtDistance, this->m_LookAt.z, this->m_LookAt.x,
                  this->m_LookAt.y + this->m_LookAtDistance - 1.f, this->m_LookAt.z, 1, 0, 0);
        break;
    case CameraType::STATIC_Z_AXIS:
        gluLookAt(this->m_LookAt.x, this->m_LookAt.y, this->m_LookAt.z + this->m_LookAtDistance, this->m_LookAt.x,
                  this->m_LookAt.y, this->m_LookAt.z + this->m_LookAtDistance - 1.f, 0, 1, 0);
        break;
    default:
        throw std::runtime_error("Error: Unknown CameraType");
    }
    glutPostRedisplay();
}

void Window3D::_ZoomCamera(bool positveZoom) {
    this->m_LookAtDistance += positveZoom ? this->m_DistAdjustment : -this->m_DistAdjustment;
    ;
    this->m_DistAdjustment = std::abs(this->m_LookAtDistance) / 10.f + 0.1f;
}

Color Window3D::_AdjustColorOpenGL(const Color& color) {
    return {uint8_t(color.r / 2), uint8_t(color.g / 2), uint8_t(color.b / 2)};
}

void Window3D::setMaxFPS(float maxFPS) {
    if (maxFPS < 0.f)
        this->m_MaxFPS = 0.f;
    else if (maxFPS > 60.f)
        this->m_MaxFPS = 60.f;
    else
        this->m_MaxFPS = maxFPS;
}

int Window3D::getWindowWidth() const { return this->m_Width; }

int Window3D::getWindowHeight() const { return this->m_Height; }

void Window3D::setCamera(Window3D::CameraType type, glm::vec3 lookAt, float distance, glm::vec3 positionCorrection) {
    this->m_CameraType = type;
    this->m_LookAt = lookAt;
    this->m_LookAtDistance = distance;
    this->m_CameraPositionCorrection = positionCorrection;
    this->_AdjustCamera();
}

void Window3D::drawAxis(float len) const {
    glBegin(GL_LINES);
    {
        glColor3f(1, 0, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(len, 0, 0);
        glColor3f(0, 1, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(0, len, 0);
        glColor3f(0, 0, 1);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, len);
    }
    glEnd();
}

void Window3D::forceDisplay() const {
    glutSwapBuffers();
    glFlush();
}
}
