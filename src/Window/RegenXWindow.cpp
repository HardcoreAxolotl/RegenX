#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <RegenX/Window/RegenXWindow.hpp>
#include <RegenX/Core/RegenXLog.hpp>

// Window constructor
// Initializes a GLFW window with the requested width and height, makes its
// OpenGL context current, and loads OpenGL function pointers via GLAD.
// Parameters are intentionally simple integers to match usage in the app.
Window::Window(const int window_width, const int window_height, const char* title, GLFWmonitor* monitor, GLFWwindow* share)
{
    // Create a GLFW window of the requested size and set its title.
    // The last two parameters (monitor, share) may be nullptr if the caller
    // does not request fullscreen or context sharing.
    window = glfwCreateWindow(window_width, window_height, title, monitor, share);

    // If window creation failed, log a fatal error and terminate GLFW. The
    // application may exit shortly after (depending on how regenx::log::fatal
    // is implemented). We don't return here because there's no valid window.
    if (window == nullptr)
    {
        regenx::log::fatal("Failed to create GLFW window");
        glfwTerminate();
    }

    // Make the newly-created window's OpenGL context the current context for
    // the calling thread. All subsequent OpenGL calls will affect this context.
    glfwMakeContextCurrent(window);

    // Load OpenGL function pointers through GLAD. If this fails the program
    // cannot use modern OpenGL functions and should abort.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        regenx::log::fatal("Failed to initialize GLAD");
    }

    // Set the OpenGL viewport to cover the whole window.
    // The viewport maps normalized device coordinates to window coordinates.
    // Here it starts at (0,0) and spans the full width and height provided.
    glViewport(0, 0, window_width, window_height);
}

// Swap the front and back buffers for the window. Call this each frame after
// rendering to present the rendered image to the screen.
void Window::swap_buffers() const
{
    glfwSwapBuffers(window);
}

// Destroy the GLFW window and free associated resources. After calling this
// the Window object no longer refers to a valid GLFWwindow.
void Window::destroy() const
{
    glfwDestroyWindow(window);
}

// Query whether the user has requested the window to close (for example by
// clicking the close button). This is commonly used to drive the main loop.
bool Window::should_close() const
{
    return glfwWindowShouldClose(window);
}

// Return the raw GLFWwindow pointer. Exposes the underlying window handle for
// advanced or library-specific operations that aren't wrapped by this class.
GLFWwindow* Window::get() const
{
    return window;
}