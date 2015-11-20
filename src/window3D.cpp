
#include "window3D.h"
#include <stdexcept>
#include <math.h>

#include <GL/glut.h>
#include <chrono>
#include <thread>
#include <glm/gtx/quaternion.hpp>


namespace cf{


Window3D::Window3D(int* argc, char** argv, int width, int height, const char* title)
    : m_Title(title), m_Width(width), m_Height(height), m_WindowID(-1),
      m_LookAt(0.f, 0.f, 0.f), m_LookAtDistance(10.f), m_RotationAngle_Z(0.f), m_RotationAngle_Y(0.f)
{
    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(this->m_Width, this->m_Height);

    this->m_WindowID = glutCreateWindow(this->m_Title.c_str());
    glLoadIdentity();             // Reset

    // Set the viewport to cover the new window
    glViewport(0, 0, this->m_Width, this->m_Height);

    // Set the aspect ratio of the clipping volume to match the viewport
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix

    // Enable perspective projection with fovy, aspect, zNear and zFar

    // Compute aspect ratio of the new window
    GLfloat aspect = (GLfloat)this->m_Width / (GLfloat)this->m_Height;
    gluPerspective(45.0f, aspect, 0.1f, 1000.0f);
    /*
    gluLookAt(0, 0, 10,
              0, 0, 0,
              0, 1, 0);
    */
    // set default camera
    this->setCamera(Window3D::CameraType::STATIC_Z_AXIS);
}
Window3D::~Window3D(){
    glutDestroyWindow(this->m_WindowID);
}







int Window3D::startDrawing(){
    // cap fps
    using namespace std::chrono;
    system_clock::time_point current = system_clock::now(), next;
    constexpr float maxFPS = 60.0f;
    constexpr int timePerFrameInMS = (1.f / maxFPS) * 1000.f;

    while(true){
        this->draw();
        glutSwapBuffers();
        glutPostRedisplay();
        glFlush();

        next = system_clock::now();
        int numMilliseconds = duration_cast<milliseconds>(next - current).count();
        if (numMilliseconds < timePerFrameInMS)
            std::this_thread::sleep_for(milliseconds(timePerFrameInMS - numMilliseconds));

        current = next;
    }
    return 0;
}


void Window3D::clear(const Color& color){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Window3D::drawCylinder(const glm::vec3 &drawingDirection, const glm::vec3 &position, const Color color) const {
    // set drawing angles
    const glm::vec3 startDir = glm::vec3(1.f, 0.f, 0.f);
    const glm::vec3 dir = glm::normalize(drawingDirection);

    const glm::vec3 rotVec = glm::normalize(glm::cross(startDir, dir));

    glPushMatrix();
    {
        static GLUquadricObj* obj = gluNewQuadric();
        glColor3f(color.r, color.g, color.b);
        glTranslatef(position.x, position.y, position.z);

        float angle = glm::angle(startDir, dir) / glm::pi<float>() * 180.f;
        glRotatef(angle, rotVec.x, rotVec.y, rotVec.z);

        float vx = drawingDirection.x;
        float vy = drawingDirection.y;
        float vz = drawingDirection.z;

        float scale = sqrt(vx * vx + vy * vy + vz * vz);

        // start rotations
        glRotatef(90, 0, 1, 0);
        glScalef(scale, scale, scale);

        glBegin(GL_POLYGON);
        {
            gluQuadricDrawStyle(obj, GLU_LINE);
            gluCylinder(obj, 0.1, 0.1, 1, 10, 10);
        }
        glEnd();
    }
    glPopMatrix();
}

void Window3D::_adjustCamera(){
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
    this->_adjustCamera();
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











