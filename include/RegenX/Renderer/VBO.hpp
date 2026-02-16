//
// Created by liad on 1/14/26.
//
#pragma once

#include <glad/glad.h>

#include "RegenX/Core/RegenXType.hpp"

class VBO {
    public:
    GLuint id;
    VBO(GLfloat *vertices, GLsizeiptr size);

    VBO(regenx::type::vertex* vertices, GLsizeiptr size);
    void bind();
    void unbind();
    void destroy();
};
