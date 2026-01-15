#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <iostream>
#include <cmath>

#include <rendering/Shader.hpp>
#include <rendering/VAO.h>
#include <rendering/VBO.h>
#include <rendering/EBO.h>

int main()
{
    // ---------------- SDL INIT ----------------
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("SDL Init failed: %s", SDL_GetError());
        return -1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_Window* window = SDL_CreateWindow(
        "RegenX - SDL2 OpenGL",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, 800,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );

    SDL_GLContext context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, context);

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        SDL_Log("Failed to initialize GLAD");
        return -1;
    }

    glViewport(0, 0, 800, 800);

    // ---------------- GEOMETRY ----------------
    GLfloat vertices[] =
    {
        // positions            // colors
        -0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f, // bottom-left   (red)
         0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f, // bottom-right  (white)
         0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f, // top-right     (blue)
        -0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f  // top-left      (green)
    };
    GLuint indices[] =
    {
        0, 1, 2,  // first triangle
        2, 3, 0   // second triangle
    };

    // ---------------- SHADER ----------------
    Shader shader("resource/shaders/shader.vert",
                  "resource/shaders/shader.frag");

    // ---------------- BUFFERS ----------------
    VAO vao;
    vao.bind();

    VBO vbo(vertices, sizeof(vertices));
    EBO ebo(indices, sizeof(indices));

    vao.link_attributes(vbo, 0, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void*)0);
    vao.link_attributes(vbo, 1, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    vao.unbind();
    vbo.unbind();
    ebo.unbind();

    GLuint scaleLoc = glGetUniformLocation(shader.id, "scale");

    // ---------------- LOOP ----------------
    bool running = true;
    SDL_Event e;

    while (running)
    {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = false;
        }

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.activate();
        glUniform1f(scaleLoc, 0.5f);

        vao.bind();
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

        SDL_GL_SwapWindow(window);
    }

    // ---------------- CLEANUP ----------------
    vao.destroy();
    vbo.destroy();
    ebo.destroy();
    shader.destroy();

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
