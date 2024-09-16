#include "WindowsWindow.h"
#include "Deer/Core/Log.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Deer/Core/Events/ApplicationEvent.h"
#include "Deer/Core/Events/KeyEvent.h"
#include "Deer/Core/Events/MouseEvent.h"

#include "Plattform/OpenGL/OpenGLContext.h"
#include "Plattform/OpenGL/imgui_impl_glfw.h"

#include <string>
#include <sstream>

namespace Deer {
    Window* Window::create(const WindowProps& props) {
        return new WindowsWindow(props);
    }

    WindowsWindow::WindowsWindow(const WindowProps& props)
    {
        m_data.height = props.height;
        m_data.width = props.width;

        if (!glfwInit())
        {
            DEER_CORE_ERROR("Failed to initialize GLFW");
            return;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        m_window = glfwCreateWindow(props.width, props.height, props.title.c_str(), NULL, NULL);
        if (!m_window) {
            DEER_CORE_ERROR("Window or OpenGL context creation failed (GLFW)");
            return;
        }

        m_renderContext = new OpenGLContext(m_window);
        m_renderContext->init();

        setVSync(true);
        glfwSetWindowUserPointer(m_window, &m_data);

        //Set GLFW callback
        glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
            WindowsWindowData& data = *(WindowsWindowData*)glfwGetWindowUserPointer(window);
            data.width = width;
            data.height = height;

            WindowResizeEvent event(width, height);
            data.callback(event);
        });

        glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
            WindowsWindowData& data = *(WindowsWindowData*)glfwGetWindowUserPointer(window);

            WindowCloseEvent event;
            data.callback(event);
        });

        glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scanCode, int action, int mods) {

            WindowsWindowData& data = *(WindowsWindowData*)glfwGetWindowUserPointer(window);

            switch (action)
            {
            case GLFW_PRESS: {
                KeyPressedEvent event(key, 0);
                data.callback(event);
                break;
            }
            case GLFW_RELEASE: {
                KeyReleasedEvent event(key);
                data.callback(event);
                break;
            }
            case GLFW_REPEAT: {
                KeyPressedEvent event(key, 1);
                data.callback(event);
                break;
            }
            }
        });

        glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
            WindowsWindowData& data = *(WindowsWindowData*)glfwGetWindowUserPointer(window);

            switch (action)
            {
            case GLFW_PRESS: {
                MouseButtonPressedEvent event(button);
                data.callback(event);
                break;
            }
            case GLFW_RELEASE: {
                MouseButtonReleasedEvent event(button);
                data.callback(event);
                break;
            }
            }
        });

        glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset) {
            WindowsWindowData& data = *(WindowsWindowData*)glfwGetWindowUserPointer(window);

            MouseScrolledEvent event(xOffset, yOffset);
            data.callback(event);
        });

        glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset) {
            WindowsWindowData& data = *(WindowsWindowData*)glfwGetWindowUserPointer(window);

            MouseMovedEvent event(xOffset, yOffset);
            data.callback(event);
        });

        glfwSetErrorCallback([](int errorCode, const char* description) {
            std::stringstream error;
            error << "GLFW error : " << errorCode << " " << description;
            DEER_CORE_ERROR(error.str().c_str());
        });

        glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int key) {
            WindowsWindowData& data = *(WindowsWindowData*)glfwGetWindowUserPointer(window);

            KeyTypedEvent event(key);
            data.callback(event);
        });

    }

    WindowsWindow::~WindowsWindow()
    {
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    void WindowsWindow::onUpdate()
    {
        glfwPollEvents();
        m_renderContext->swapBuffers();
    }

    void WindowsWindow::clear()
    {
        glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void WindowsWindow::setVSync(bool enabled)
    {
        if (enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);

        m_data.vSync = enabled;
    }

    bool WindowsWindow::getKeyPressed(unsigned int key) {
        auto state = glfwGetKey(m_window, key);
        return (state == GLFW_PRESS || state == GLFW_REPEAT);
    }

    bool WindowsWindow::getMouseButton(int button) {
        auto state = glfwGetMouseButton(m_window, button);
        return state == GLFW_PRESS;
    }

    void WindowsWindow::getMousePos(float& x, float& y) {
        
        double _x, _y;
        glfwGetCursorPos(m_window, &_x, &_y);

        x = _x;
        y = _y;
    }

    void WindowsWindow::initImGUI() {
        ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    }

}