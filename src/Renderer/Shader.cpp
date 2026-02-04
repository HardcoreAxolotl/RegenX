//
// Created by liad on 1/14/26.
//

#include <RegenX/Renderer/Shader.hpp>

#include <RegenX/Core/RegenXIO.hpp>
#include <RegenX/Core/RegenXLog.hpp>
#include <glad/glad.h>

Shader::Shader(const char* vertex_file, const char* fragment_file) {
    std::string vertexSource   = regenx::io::file_read(vertex_file);
    std::string fragmentSource = regenx::io::file_read(fragment_file);

    const char* vertex_source   = vertexSource.c_str();
    const char* fragment_source = fragmentSource.c_str();

    // Load Vertex Shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertex_source, NULL);
    glCompileShader(vertexShader);

    // Load Fragment Shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment_source, NULL);
    glCompileShader(fragmentShader);

    // Attach Shaders to program
    id = glCreateProgram();
    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);
    glLinkProgram(id);

    // Clean up as they are useless now
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::activate() const {
    glUseProgram(id);
}

void Shader::destroy() const
{
    glDeleteProgram(id);
}

