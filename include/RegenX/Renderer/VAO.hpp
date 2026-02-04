//
// Created by liad on 1/14/26.
//
#pragma once

#include <glad/glad.h>
#include "VBO.hpp"

class VAO {
    public:
    GLuint id;
    VAO();

    void link_attributes(::VBO &vbo, GLuint layout, GLuint num_components, GLenum type, GLsizeiptr stride, void *offset);
    void bind();
    void unbind();
    void destroy();
};
