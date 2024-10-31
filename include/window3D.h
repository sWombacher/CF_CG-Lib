#ifndef WINDOW_3D_H_H
#define WINDOW_3D_H_H

#ifndef __APPLE__
#include <GL/freeglut.h>
#else
#include <GLUT/glut.h>
#endif

#include <functional>
#include <string>
#include <vector>

#include "utils.h"

namespace cf {

/**
 * @brief The Window3D struct is the default class for accessing 3D content, creating more than 1 instance results in undefined
 * behavior
 */
struct Window3D {
    Window3D(int* argc, char** argv, int width = 800, int height = 600, const char* title = "chaos and fractals");
    virtual ~Window3D();

    /**
     * @brief Friendly mousebutton mnmes
     */
    enum class MouseButton { LEFT, CENTER, RIGHT, WHEEL_UP, WHEEL_DOWN };

    /**
     * @brief Friendly mousebutton events
     */
    enum class MouseButtonEvent { RELEASED, PRESSED };

    /**
     * @brief The CameraType enum providing access to camera types, default: 'CameraType::ROTATION'
     */
    enum class CameraType { NONE, ROTATION, FREE_MOVEMENT, STATIC_X_AXIS, STATIC_Y_AXIS, STATIC_Z_AXIS };

    /**
     * @brief handleMousePressedMovement Access mouse movement position while one mousebutton is pressed. Should return true if
     * the default behavior (rotate Camera around object) should be dismissed.
     * @param button The pressed Mousebutton
     * @param x Mouse-x-position
     * @param y Mouse-y-position
     * @return Should return true if the default behavior (rotate Camera around object) should be dismissed.
     */
    virtual bool handleMousePressedMovement(MouseButton button, int x, int y);

    /**
     * @brief handleMousePress Access mousebutton presses or releases on position
     * @param button The pressed Mousebutton
     * @param event The button event
     * @param x Mouse-x-position of the event
     * @param y Mouse-y-position of the event
     */
    virtual void handleMousePressEvent(MouseButton button, MouseButtonEvent event, int x, int y);

    /**
     * @brief printWindowUsage Print camera usage to console
     */
    static void printWindowUsage();

    void clear(const Color& color = Color::WHITE);

    /**
     * @brief draw Draw function, this has to be implemented
     */
    virtual void draw() = 0;

    /**
     * @brief handleKeyboardInput Access key input by simple override this function
     * @param key Key pressed
     * @param x Mouse-x-position of the key press event
     * @param y Mouse-y-position of the key press event
     */
    virtual void handleKeyboardInput(unsigned char key, int x, int y);

    /**
     * @brief startDrawing Start drawing, this function only returns afer 'ESC'-key press
     * @brief fistClearColor Fist clear color (clear in 'draw' function might be ignored the first time)
     * @return
     */
    int startDrawing();

    int getWindowWidth() const;
    int getWindowHeight() const;

    /**
     * @brief setCamera Set or change current camera type
     * @param type Camera type
     * @param lookAt
     * @param distance
     */
    void setCamera(CameraType type, glm::vec3 lookAt = glm::vec3(0, 0, 0), float distance = 10.f,
                   glm::vec3 positionCorrection = glm::vec3(0, 0, 0));

    /**
     * @brief drawAxis Draw x-,y- and z-axis
     * @param length Axis length
     */
    void drawAxis(float length = 10.f) const;

    /**
     * @brief forceDisplay Displays all content, it may be used for displaying the current process of the draw function
     */
    void forceDisplay() const;

    /**
     * @brief drawCylinder Draws a solid clynder
     * @param drawingDirection Cylinder direction
     * @param position Start position
     * @param diameter Cylinder diamenter
     * @param color Cylinder color
     */
    void drawCylinder(const glm::vec3& drawingDirection, const glm::vec3& position, float diameter = 1.f,
                      const Color& color = Color::WHITE) const;

    /**
     * @brief Type adjusted version of Window3D::drawCylinder
     */
    void drawCylinder(const glm::vec4& drawingDirection, const glm::vec3& position, float diameter = 1.f,
                      const Color& color = Color::WHITE) const;

    /**
     * @brief Type adjusted version of Window3D::drawCylinder
     */
    void drawCylinder(const glm::vec3& drawingDirection, const glm::vec4& position, float diameter = 1.f,
                      const Color& color = Color::WHITE) const;

    /**
     * @brief Type adjusted version of Window3D::drawCylinder
     */
    void drawCylinder(const glm::vec4& drawingDirection, const glm::vec4& position, float diameter = 1.f,
                      const Color& color = Color::WHITE) const;

    // Groch
    /**
    * @brief drawSphere Draws a solid Sphere
    * @param position Midpoint position
    * @param diameter Sphere diamenter
    * @param color Sphere color
    */
    void drawSphere(const glm::vec3& position, float diameter = 1.f, const Color& color = Color::WHITE) const;

    // Groch
    /**
    * @brief drawCube Draws a solid Cube
    * @param position Midpoint position
    * @param size Cube size
    * @param color Cube color
    */
    void drawCube(const glm::vec3& position, float size = 1.f, const Color& color = Color::WHITE) const;

    /**
     * @brief setMaxFPS Set maximum frames per second
     * @param maxFPS values > 0 indicates capped fps, value of 0 indicates "only draw after key-input", 0 is default
     */
    void setMaxFPS(float maxFPS = 0.f);

    /**
     * @brief enableLighting Enable lightning (Default: lightning is enabled)
     */
    void enableLighting() { glEnable(GL_LIGHTING); }
    /**
     * @brief disableLighting Disable lightning (Default: lightning is enabled)
     */
    void disableLighting() { glDisable(GL_LIGHTING); }

  protected:
    float m_DistAdjustment = 1.f;
    float m_AngleAdjustment = 1.f;
    float m_CameraAdjustment = 1.f;
    void _AdjustCamera();
    void _ZoomCamera(bool positveZoom);

    static cf::Color _AdjustColorOpenGL(const cf::Color& color);

    // you may change this parameters to the center of your object / you own distance
    glm::vec3 m_LookAt = glm::vec3(0.f, 0.f, 0.f);
    float m_LookAtDistance = 10.f;

    glm::vec3 m_CameraPositionCorrection = glm::vec3(0.f, 0.f, 0.f);

    float m_RotationAngle_Y = 0.f;
    float m_RotationAngle_X = 0.f;

    CameraType m_CameraType = Window3D::CameraType::ROTATION;

    /// CameraType::FREE_MOVEMENT specific member variables
    glm::vec3 m_FreeCamera_position = glm::vec3(0.f, 0.f, 0.f);
    glm::vec3 m_FreeCamera_UpVector = glm::vec3(0.f, 1.f, 0.f);
    glm::vec3 m_FreeCamera_LookDirection = glm::vec3(0.f, 0.f, 1.f);

  private:
    static void _KeyboardCallbackFunction(unsigned char key, int x, int y);
    static void _DrawingFunction();
    static void _MouseCtlClickCallbackFunction(int button, int press, int y, int x);
    static void _MouseCtlMotionCallbackFunction(int y, int x);
    static void _WindowResizeEvent(int w, int h);

    float m_MaxFPS = 0.f;

    std::function<void(unsigned char key, int x, int y)> m_AdditionalKeyboardCallback;
    std::function<bool(MouseButton btn, int x, int y)> m_AdditionalMouseMotionCallback;
    std::function<void(MouseButton button, MouseButtonEvent event, int x, int y)> m_AdditionalMouseClickCallback;
    int m_Width;
    int m_Height;

    int m_WindowID = -1;
};
}

#endif
