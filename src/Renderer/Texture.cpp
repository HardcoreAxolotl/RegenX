//
// Created by liad on 23/01/2026.
//
#include <RegenX/Renderer/Texture.hpp>

Texture::Texture(const char* image, const GLenum texture_type, const GLenum slot, const GLenum format, const GLenum pixel_type)
{
	// Assigns the type of the texture ot the texture object
	type = texture_type;

	// Stores the width, height, and the number of color channels of the image
	int widthImg, heightImg, numColCh;
	// Reads the image from a file and stores it in bytes
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);

	// Generates an OpenGL texture object
	glGenTextures(1, &id);
	// Assigns the texture to a Texture Unit
	glActiveTexture(slot);
	glBindTexture(texture_type, id);

	// Configures the type of algorithm that is used to make the image smaller or bigger
	glTexParameteri(texture_type, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(texture_type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Configures the way the texture repeats (if it does at all)
	glTexParameteri(texture_type, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(texture_type, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Extra lines in case you choose to use GL_CLAMP_TO_BORDER
	// float flatColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
	// glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor);

	// Assigns the image to the OpenGL Texture object
	glTexImage2D(texture_type, 0, GL_RGBA, widthImg, heightImg, 0, format, pixel_type, bytes);
	// Generates MipMaps
	glGenerateMipmap(texture_type);

	// Deletes the image data as it is already in the OpenGL Texture object
	stbi_image_free(bytes);

	// Unbinds the OpenGL Texture object so that it can't accidentally be modified
	glBindTexture(texture_type, 0);
}

void Texture::texture_unit(Shader& shader, const char* uniform, GLuint unit)
{
	// Gets the location of the uniform
	const GLuint tex_uni = glGetUniformLocation(shader.id, uniform);
	// Shader needs to be activated before changing the value of a uniform
	shader.activate();
	// Sets the value of the uniform
	glUniform1i(tex_uni, unit);
}

void Texture::bind() const
{
	glBindTexture(type, id);
}

void Texture::unbind() const
{
	glBindTexture(type, 0);
}

void Texture::destroy() const
{
	glDeleteTextures(1, &id);
}
