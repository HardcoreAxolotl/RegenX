//
// Created by liad on 1/14/26.
//

#pragma once
#include <glad/glad.h>

class Shader {
public:
    GLuint id;
    Shader(const char* vertex_file, const char* fragment_file);

    void activate();

    void destroy();
};
