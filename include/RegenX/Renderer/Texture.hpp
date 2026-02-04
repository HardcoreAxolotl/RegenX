#pragma once

#include <glad/glad.h>
#include <stb/stb_image.h>

#include "Shader.hpp"

class Texture {
public:
    GLuint id{};
    GLenum type;
    Texture(const char* image, GLenum texture_type, GLenum slot, GLenum format, GLenum pixel_type);
    void texture_unit(Shader& shader, const char* uniform, GLuint unit);

    void bind() const;
    void unbind() const;
    void destroy() const;
};
