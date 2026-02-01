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
    Window(const int WINDOW_WIDTH, const int WINDOW_HEIGHT) {
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
    void swap_buffers() {
        glfwSwapBuffers(window);
    }

    void destroy() {
        glfwDestroyWindow(window);
    }

    void set_fullscreen() {
    }

    // Get functions
    bool should_close() {
        return glfwWindowShouldClose(window);
    }

private:
    GLFWwindow* window;
};

#endif // REGENXWINDOW_H
