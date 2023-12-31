#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <string>

namespace shl {

class Window
{
    private:
        GLFWwindow* m_window = nullptr;
        bool        m_windowOk = true;
        const char* m_glsl_version = "#version 460";
        int         m_width;
        int         m_height;
        const char* m_tile;

        // FPS counter
        double      m_prevTime = 0.0;
        double      m_crntTime = 0.0;
        double      m_timeDiff = 0.0;
        uint32_t    m_counter  = 0;


        static void glfw_error_callback(int error, const char* description)
        {
            fprintf(stderr, "GLFW Error %d: %s\n", error, description);
        }

    public:

    Window(int width, int heigth, const char* title): m_width(width), m_height(heigth), m_tile(title)
    {
        glfwSetErrorCallback(this->glfw_error_callback);

        /* Initialize glfw */
        if (!glfwInit())
        {
            m_windowOk = false;
            return;
        }

        /* Create a windowed mode window and its OpenGL context */
        m_window = glfwCreateWindow(m_width, m_height, m_tile, NULL, NULL);

        if (!m_window)
        {
            m_windowOk = false;
            glfwTerminate();
            return;
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(m_window);
        glfwSwapInterval(1); // Enable vsync

        // TODO: Enable graphics api abstraction
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
        {
            puts("Failed to initialize OpenGL context\n");
            m_windowOk = false;
            return;
        }

        // m_glsl_version = "#version 460";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    }

    ~Window()
    {
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    void pollEvents() 
    { 
        /* Poll for and process events */
        glfwPollEvents();
    }

    int windowShouldClose() { return glfwWindowShouldClose(m_window); }

    void swapBuffers()
    {
        m_crntTime = glfwGetTime();
        m_timeDiff = m_crntTime - m_prevTime;
        m_counter++;
        if(m_timeDiff >= 1.0 / 30.0)
        {
            std::string FPS = std::to_string((1.0 / m_timeDiff) * m_counter);
            std::string ms  = std::to_string((m_timeDiff / m_counter) * 1000);
            std::string title = FPS + "fps / " + ms + "ms";
            glfwSetWindowTitle(m_window, title.c_str());
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(m_window);
    }

    bool ok() { return m_windowOk; }

    GLFWwindow* get_window() { return m_window; }

    const char* getGlslVersion() { return m_glsl_version; }
};

}
