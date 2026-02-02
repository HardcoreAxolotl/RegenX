#include "RegenXWindow.hpp"

Window::Window(const int WINDOW_WIDTH, const int WINDOW_HEIGHT)
{
    // Create a GLFWwindow object of WINDOW_WIDTH by WINDOW_HEIGHT pixels, naming it "YoutubeOpenGL"
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "YoutubeOpenGL", NULL, NULL);
    // Error check if the window fails to create
    if (window == NULL)
    {
        regenx::log::log_fatal("Failed to create GLFW window", false);
        glfwTerminate();
    }
    // Introduce the window into the current context
    glfwMakeContextCurrent(window);

    //Load GLAD so it configures OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        regenx::log::log_fatal("Failed to initialize GLAD");
    }

    // Specify the viewport of OpenGL in the Window
    // In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
}

// Do functions
void Window::swap_buffers() const
{
    glfwSwapBuffers(window);
}

void Window::destroy() const
{
    glfwDestroyWindow(window);
}

// Get functions
bool Window::should_close() const
{
    return glfwWindowShouldClose(window);
}
