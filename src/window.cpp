
#include "window.h"
#include <stdexcept>
#include <math.h>

#include <GL/glut.h>


namespace cf{


Window3D::Window3D(int width, int height, const char* title) : m_Title(title), m_Width(width), m_Height(height), m_WindowID(-1){
}
Window3D::~Window3D(){
    glutDestroyWindow(this->m_WindowID);
}


void Window3D::draw() const{
    glutSwapBuffers();
    glutPostRedisplay();
    glFlush();
}

void Window3D::clear(const Color& color){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window3D::init(int *argc, char **argv){
    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(this->m_Width, this->m_Height);

    this->m_WindowID = glutCreateWindow(this->m_Title.c_str());
    glLoadIdentity();             // Reset

    // Compute aspect ratio of the new window
    GLfloat aspect = (GLfloat)this->m_Width / (GLfloat)this->m_Height;

    // Set the viewport to cover the new window
    glViewport(0, 0, this->m_Width, this->m_Height);

    // Set the aspect ratio of the clipping volume to match the viewport
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix

    // Enable perspective projection with fovy, aspect, zNear and zFar
    gluPerspective(45.0f, aspect, 0.1f, 1000.0f);
    gluLookAt(0, 0, 10,
              0, 0, 0,
              0, 1, 0);
}


void Window3D::drawCylinder(const glm::vec3 &drawingDirection, const glm::vec3 &position, const Color color) const {

    //glMatrixMode(GL_MODELVIEW);

    // set drawing angles
    const glm::vec3 startDir = glm::vec3(1.f, 0.f, 0.f);//glm::normalize(originalDir);
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
        //glRotatef(data.angle_z, 0, 0, 1);
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

int Window3D::getWindowWidth() const {
    return this->m_Width;
}

int Window3D::getWindowHeight() const{
    return this->m_Height;
}

void Window3D::drawAxis() const{
    glBegin(GL_LINES);
    glColor3f(1, 0, 0); glVertex3f(0, 0, 0); glVertex3f(25, 0, 0);
    glColor3f(0, 1, 0); glVertex3f(0, 0, 0); glVertex3f(0, 25, 0);
    glColor3f(0, 0, 1); glVertex3f(0, 0, 0); glVertex3f(0, 0, 25);
    glEnd();
}


}











