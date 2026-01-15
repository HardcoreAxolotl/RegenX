//
// Created by liad on 1/14/26.
//

#include "VBO.h"

VBO::VBO(GLfloat *vertices, GLsizeiptr size) {
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);
}

void VBO::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VBO::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::destroy() {
    glDeleteBuffers(1, &id);
}
