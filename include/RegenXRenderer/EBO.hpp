#pragma once

#include "glad/glad.h"

class EBO {
public:
    GLuint id;
    EBO(GLuint *indices, GLsizeiptr size);

    void bind() const;
    void unbind();
    void destroy() const;
};
