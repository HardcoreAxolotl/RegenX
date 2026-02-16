#pragma once

#ifndef REGENXWINDOW_HPP
#define REGENXWINDOW_HPP

// Forward declarations to avoid pulling in the full GLFW header (and the
// OpenGL headers it may include). The implementation file includes glad and
// GLFW in the correct order.
struct GLFWwindow;
struct GLFWmonitor;

/**
 * Simple RAII-style wrapper around a GLFWwindow.
 *
 * Responsibilities:
 * - Create a GLFW window and make its OpenGL context current.
 * - Provide helper methods for common window operations (swap buffers,
 *   query close status, destroy).
 *
 * Note: This class intentionally exposes the raw GLFWwindow* via `get()`
 * for advanced usage. The wrapper does not attempt to manage GLFW initialization
 * (glfwInit) or termination (glfwTerminate) — that is expected to happen
 * elsewhere in the application.
 */
class Window {
public:
    /**
     * Construct a Window of the given width and height.
     * The constructor will create the GLFWwindow and initialize GLAD for OpenGL
     * function loading. If creation or initialization fails, a fatal log entry
     * will be produced and GLFW may be terminated.
     *
     * @param window_width  Desired window width in pixels.
     * @param window_height Desired window height in pixels.
     * @param title         Optional window title (default: "RegenX").
     * @param monitor       Optional monitor for fullscreen mode (default: nullptr).
     * @param share         Optional window whose context to share (default: nullptr).
     */
    Window(int window_width, int window_height, const char* title = "RegenX", GLFWmonitor* monitor = nullptr, GLFWwindow* share = nullptr);

    // Swap the front and back buffers. Should be called once per rendered
    // frame to present the rendered image.
    void swap_buffers() const;

    // Destroy the underlying GLFWwindow. After calling this, the Window no
    // longer refers to a valid window.
    void destroy() const;

    // Query whether the user has requested that the window be closed.
    [[nodiscard]] bool should_close() const;

    // Return the raw GLFWwindow pointer. Use carefully; the caller remains
    // responsible for not using the pointer after `destroy()` is called.
    [[nodiscard]] GLFWwindow* get() const;

private:
    GLFWwindow* window;
};

#endif // REGENXWINDOW_HPP
