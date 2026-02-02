#pragma once

#ifndef REGENXWINDOW_HPP
#define REGENXWINDOW_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "RegenXLog.hpp"

/**
 * @todo write docs
 */

class Window {
public:
    Window(const int WINDOW_WIDTH, const int WINDOW_HEIGHT);

    // Do functions
    void swap_buffers() const;

    void destroy() const;

    // Get functions
    bool should_close() const;

private:
    GLFWwindow* window;
};

#endif // REGENXWINDOW_H
