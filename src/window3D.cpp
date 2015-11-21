
#include "window3D.h"

#include <chrono>
#include <thread>
#include <glm/gtx/quaternion.hpp>



namespace cf{


Window3D::Window3D(int* argc, char** argv, int width, int height, const char* title) : m_Width(width), m_Height(height) {
    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(this->m_Width, this->m_Height);

    this->m_WindowID = glutCreateWindow(title);
    glLoadIdentity();             // Reset

    // Set the viewport to cover the new window
    glViewport(0, 0, this->m_Width, this->m_Height);

    // Set the aspect ratio of the clipping volume to match the viewport
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix

    // Enable perspective projection with fovy, aspect, zNear and zFar

    // Compute aspect ratio of the new window
    GLfloat aspect = (GLfloat)this->m_Width / (GLfloat)this->m_Height;
    gluPerspective(45.0f, aspect, 0.1f, 1000.0f);
}
Window3D::~Window3D(){
    glutDestroyWindow(this->m_WindowID);
}

void Window3D::showWindowUsage(){
    std::cout << "Close window:\n"
              << "esc-key\n\n"
              << "Camera usage:\n"
              << "a/d: camera movement left/right\n"
              << "w/s: camera movement up/down\n"
              << "f/r: move camera closer/further away\n\n"
              << "Rotation-Type specific movements:\n"
              << "q/e: rotation y-axis\n"
              << "y/c: rotation z-axis\n"
              << std::endl;
}


Window3D* windowPtr = nullptr;
void _DrawingFunction(){
    if (!windowPtr)
        throw "Error: window ptr not set!";

    windowPtr->draw();
    glutSwapBuffers();
    glFlush();

    if (windowPtr->m_MaxFPS){
        using namespace std::chrono;
        system_clock::time_point current = system_clock::now(), next;

        int timePerFrameInMS = (1.f / windowPtr->m_MaxFPS) * 1000.f;
        next = system_clock::now();
        int numMilliseconds = duration_cast<milliseconds>(next - current).count();
        if (numMilliseconds < timePerFrameInMS)
            std::this_thread::sleep_for(milliseconds(timePerFrameInMS - numMilliseconds));

        glutTimerFunc(0, [](int v) { glutPostRedisplay(); }, 1);
    }
}
void _KeyboardCallbackFunction(unsigned char key, int x, int y){
    if (!windowPtr)
        throw "Error: window ptr not set!";

    // user specified function
    if (windowPtr->m_AdditionalKeyboardCallback)
        windowPtr->m_AdditionalKeyboardCallback(key, x, y);

    glm::vec3 left, up;
    switch(windowPtr->m_CameraType){
    case Window3D::CameraType::NONE:
        // nothing to do!
        break;
    case Window3D::CameraType::ROTATION:{
            glm::vec3 cameraPos(0.f, 0.f, windowPtr->m_LookAtDistance);
            up = glm::vec3(0.f, 1.f, 0.f);

            glm::vec3 angles(cf::Convert::degree2radiant(windowPtr->m_RotationAngle_Y), cf::Convert::degree2radiant(windowPtr->m_RotationAngle_Z), 0.f);
            glm::quat rot(angles);

            cameraPos = glm::rotate(rot, cameraPos);
            up   = glm::rotate(rot, up);
            left = glm::cross(cameraPos, up);
        }
        break;
    case Window3D::CameraType::STATIC_X_AXIS:
        left = glm::vec3( 0.f, 0.f, 1.f);
        up   = glm::vec3( 0.f, 1.f, 0.f);
        break;
    case Window3D::CameraType::STATIC_Y_AXIS:
        up   = glm::vec3(1.f, 0.f,  0.f);
        left = glm::vec3(0.f, 0.f, -1.f);
        break;
    case Window3D::CameraType::STATIC_Z_AXIS:
        left = glm::vec3(-1.f, 0.f, 0.f);
        up   = glm::vec3( 0.f, 1.f, 0.f);
        break;
    default:
        throw "Unknown camera type";
    }

    // adjust left/up vectors to correct length
    left = glm::normalize(left);
    left *= windowPtr->m_CameraAdjustment;

    up = glm::normalize(up);
    up *= windowPtr->m_CameraAdjustment;

    switch (key){
    case 'a': windowPtr->m_LookAt += left; break;
    case 'd': windowPtr->m_LookAt -= left; break;

    case 'w': windowPtr->m_LookAt += up; break;
    case 's': windowPtr->m_LookAt -= up; break;

    case 'y': windowPtr->m_RotationAngle_Y += windowPtr->m_AngleAdjustment; break;
    case 'c': windowPtr->m_RotationAngle_Y -= windowPtr->m_AngleAdjustment; break;

    case 'e': windowPtr->m_RotationAngle_Z += windowPtr->m_AngleAdjustment; break;
    case 'q': windowPtr->m_RotationAngle_Z -= windowPtr->m_AngleAdjustment; break;

    case 'r': windowPtr->m_LookAtDistance += windowPtr->m_DistAdjustment; break;
    case 'f': windowPtr->m_LookAtDistance -= windowPtr->m_DistAdjustment; break;

    case 27 /*esc key*/: glutLeaveMainLoop(); break;
    default: break;
    }
    glutPostRedisplay();
    windowPtr->_AdjustCamera();
}

int Window3D::startDrawing(){
    windowPtr = this;
    glutKeyboardFunc(_KeyboardCallbackFunction);
    glutDisplayFunc(_DrawingFunction);
    glutMainLoop();
    return 0; // never reached
}

void Window3D::clear(const Color& color){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Window3D::drawCylinder(const glm::vec3& drawingDirection, const glm::vec3& position, float diameter, const Color color) const {
    // set drawing angles
    const glm::vec3 startDir = glm::vec3(1.f, 0.f, 0.f);
    const glm::vec3 dir = glm::normalize(drawingDirection);

    const glm::vec3 rotVec = glm::normalize(glm::cross(startDir, dir));

    glPushMatrix();
    {
        static GLUquadricObj* obj = gluNewQuadric();

        glColor3f((float)color.r / 255.f, (float)color.g / 255.f, (float)color.b / 255.f);
        glTranslatef(position.x, position.y, position.z);

        float angle = glm::angle(startDir, dir) / glm::pi<float>() * 180.f;
        glRotatef(angle, rotVec.x, rotVec.y, rotVec.z);

        // start rotations
        glRotatef(90, 0, 1.f, 0);
        glScalef(diameter, diameter, diameter);

        glBegin(GL_POLYGON);
        {
            gluQuadricDrawStyle(obj, GLU_LINE);
            gluCylinder(obj, 0.1, 0.1, glm::length(drawingDirection) / diameter, 10, 10);
        }
        glEnd();
    }
    glPopMatrix();
}

void Window3D::_AdjustCamera(){
    glLoadIdentity();             // Reset

    // Compute aspect ratio of the new window
    GLfloat aspect = (GLfloat)this->m_Width / (GLfloat)this->m_Height;

    // Set the viewport to cover the new window
    glViewport(0, 0, this->m_Width, this->m_Height);

    // Set the aspect ratio of the clipping volume to match the viewport
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix

    // Enable perspective projection with fovy, aspect, zNear and zFar
    gluPerspective(45.0f, aspect, 0.1f, 1000.0f);

    switch(this->m_CameraType){
    case CameraType::NONE:
        break;
    case CameraType::ROTATION:{
        glm::vec4 cameraPos(0,0,this->m_LookAtDistance,1);
        glm::vec3 angles(cf::Convert::degree2radiant(this->m_RotationAngle_Y), cf::Convert::degree2radiant(this->m_RotationAngle_Z), 0.f);
        glm::quat rot(angles);
        cameraPos = glm::rotate(rot, cameraPos);

        glm::vec3 lookUpVector(0.f, 1.f, 0.f);
        lookUpVector = glm::rotate(rot, lookUpVector);
        gluLookAt(cameraPos.x     , cameraPos.y     , cameraPos.z     ,
                  this->m_LookAt.x, this->m_LookAt.y, this->m_LookAt.z,
                  lookUpVector.x  , lookUpVector.y  , lookUpVector.z  );
        }
        break;
    case CameraType::STATIC_X_AXIS:
        gluLookAt(this->m_LookAt.x + this->m_LookAtDistance, this->m_LookAt.y, this->m_LookAt.z,
                  this->m_LookAt.x                         , this->m_LookAt.y, this->m_LookAt.z,
                  0, 1, 0);
        break;
    case CameraType::STATIC_Y_AXIS:
        gluLookAt(this->m_LookAt.x, this->m_LookAt.y + this->m_LookAtDistance, this->m_LookAt.z,
                  this->m_LookAt.x, this->m_LookAt.y                         , this->m_LookAt.z,
                  1, 0, 0);
        break;
    case CameraType::STATIC_Z_AXIS:
        gluLookAt(this->m_LookAt.x, this->m_LookAt.y, this->m_LookAt.z + this->m_LookAtDistance,
                  this->m_LookAt.x, this->m_LookAt.y, this->m_LookAt.z,
                  0, 1, 0);
        break;
    default:
        throw "Error: Unknown CameraType";
    }
}

void Window3D::setKeyboardCallbackFunction(std::function<void(unsigned char, int, int)> foo){
    this->m_AdditionalKeyboardCallback = foo;
}

void Window3D::setMaxFPS(float maxFPS){
    if (maxFPS < 0.f)
        this->m_MaxFPS = 0.f;
    else if (maxFPS > 60.f)
        this->m_MaxFPS = 60.f;
    else
        this->m_MaxFPS = maxFPS;
}

int Window3D::getWindowWidth() const {
    return this->m_Width;
}

int Window3D::getWindowHeight() const{
    return this->m_Height;
}

void Window3D::setCamera(Window3D::CameraType type, glm::vec3 lookAt, float distance){
    this->m_CameraType     = type;
    this->m_LookAt         = lookAt;
    this->m_LookAtDistance = distance;
    this->_AdjustCamera();
}

void Window3D::drawAxis(float len) const{
    glBegin(GL_LINES);
    {
        glColor3f(1, 0, 0); glVertex3f(0, 0, 0); glVertex3f(len, 0  , 0  );
        glColor3f(0, 1, 0); glVertex3f(0, 0, 0); glVertex3f(0  , len, 0  );
        glColor3f(0, 0, 1); glVertex3f(0, 0, 0); glVertex3f(0  , 0  , len);
    }
    glEnd();
}


}











