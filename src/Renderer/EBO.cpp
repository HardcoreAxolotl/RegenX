//
// Created by liad on 1/14/26.
//

#include <RegenX/Renderer/EBO.hpp>
#include "glad/glad.h"

EBO::EBO(GLuint *indices, GLsizeiptr size) {
    glGenBuffers(1, &id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}

void EBO::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void EBO::unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::destroy() const
{
    glDeleteBuffers(1, &id);
}
