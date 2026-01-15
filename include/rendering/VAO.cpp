//
// Created by liad on 1/14/26.
//

#include "VAO.h"

VAO::VAO() {
    glGenVertexArrays(1, &id);
}

void VAO::link_attributes(
    VBO& vbo,
    GLuint layout,
    GLuint num_components,
    GLenum type,
    GLsizeiptr stride,
    void* offset
) {
    vbo.bind();
    glVertexAttribPointer(
        layout,
        num_components,
        type,
        GL_FALSE,
        stride,
        offset
    );
    glEnableVertexAttribArray(layout);
}


void VAO::bind() {
    glBindVertexArray(id);
}

void VAO::unbind() {
    glBindVertexArray(0);
}

void VAO::destroy() {
    glDeleteVertexArrays(1, &id);
}
