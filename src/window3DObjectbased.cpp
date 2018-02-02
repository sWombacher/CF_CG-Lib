#include "window3DObjectbased.h"
#include <atomic>

namespace cf {
std::thread Window3DObject::_RenderThread = std::thread();

Window3DObject::Window3DObject(int* argc, char** argv, int width, int height, const char* title)
    : cf::Window3D(argc, argv, width, height, title) {}

Window3DObject &Window3DObject::createWindow3DObject(int *argc, char **argv, int width, int height, const char *title) {
    static std::mutex mutex;
    std::lock_guard<std::mutex> lg(mutex);

    static std::unique_ptr<Window3DObject> window;
    if (window)
        throw std::runtime_error("Only one Instance of 'Window3D' and/or 'Window3DObject' is allowed");

    cf::SimpleSignal signal;
    Window3DObject::_RenderThread = std::thread([=, &signal]{
        // well... 'new' is bad...
        // but its a private constructor so...
        // std::make_unique is hard to use...
        // maybe add friend declaration...
        window = std::unique_ptr<Window3DObject>(new Window3DObject(argc, argv, width, height, title));

        // offer main thread some time before notifying
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        signal.fireSignal();

        window->startDrawing();
    });
    signal.waitSignal();
    window->setMaxFPS(60.f);
    return *window;
}

void Window3DObject::setDrawingFunction(std::function<void(Window3DObject&)> function) {
    this->m_DrawingFunction.set(std::move(function));
}

void Window3DObject::setKeyboardInputFunction(std::function<void(unsigned char, int, int)> function) {
    this->m_KeyboardInput.set(std::move(function));
}

void Window3DObject::setMousePressedMovementFunction(std::function<bool(MouseButton, int, int)> function) {
    this->m_MousePressMovement.set(std::move(function));
}

void Window3DObject::setMousePressEvent(
    std::function<void(Window3D::MouseButton, Window3D::MouseButtonEvent, int, int)> function) {
    this->m_MousePressEvent.set(std::move(function));
}

void Window3DObject::waitKeyPressed(size_t delay) {
    this->m_WaitMutex.try_lock();
    if (delay)
        this->m_WaitMutex.try_lock_for(std::chrono::milliseconds(delay));
    else
        this->m_WaitMutex.lock();
}

void Window3DObject::exit() {
#ifndef __APPLE__
    glutLeaveMainLoop();
    if (Window3DObject::_RenderThread.joinable())
        Window3DObject::_RenderThread.join();
#endif
    std::exit(0);
}

void Window3DObject::draw() { this->m_DrawingFunction(*this); }

void Window3DObject::handleKeyboardInput(unsigned char key, int x, int y) {
    this->m_WaitMutex.unlock();
    this->m_KeyboardInput(key, x, y);
}

bool Window3DObject::handleMousePressedMovement(Window3D::MouseButton button, int x, int y) {
    return this->m_MousePressMovement(button, x , y);
}

void Window3DObject::handleMousePressEvent(Window3D::MouseButton button, Window3D::MouseButtonEvent event, int x, int y) {
    this->m_MousePressEvent(button, event, x, y);
}

} // namespace cf
