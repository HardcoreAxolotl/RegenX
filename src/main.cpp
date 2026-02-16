#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <iostream>
#include <thread>

#include <RegenX/Window/RegenXWindow.hpp>
#include <RegenX/Renderer/Shader.hpp>
#include <RegenX/Renderer/VAO.hpp>
#include <RegenX/Renderer/VBO.hpp>
#include <RegenX/Renderer/EBO.hpp>
#include <RegenX/Renderer/Texture.hpp>

#include "RegenX/Core/RegenXMath.hpp"

using namespace regenx;
constexpr double target_fps = 60.0;
constexpr double frame_duration = 1.0 / target_fps; // in seconds

class rectangle
{
private:
    Shader shader_program;

    VAO vao;
    VBO vbo;
    EBO ebo;
    Texture texture;

    GLuint scale_id;

    // Vertex data
    std::vector<type::vertex> vertices;
    std::vector<GLuint> indices;

    std::vector<type::vertex> original_vertices; // Keep original positions for reset
	type::vector3<float> position = {0.5f, 0.5f, 0.0f}; // Current position
    float angle = 0.0f;                          // Current rotation angle

public:
    // Constructor: initialize all GPU resources and store original vertices
	rectangle(const Shader &sp, std::vector<type::vertex> &vertices, std::vector<GLuint> &indices)
	: shader_program(sp),
	  vertices(vertices), indices(indices),
	  vbo(vertices.data(), static_cast<GLsizeiptr>(vertices.size() * sizeof(type::vertex))),
	  ebo(indices.data(), static_cast<GLsizeiptr>(indices.size() * sizeof(GLuint))),
	  texture("resource/textures/khat.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE)
	{
		vao.bind();

		// Reassigns vbo/ebo here
		vbo = VBO(vertices.data(), static_cast<GLsizeiptr>(vertices.size() * sizeof(type::vertex)));
		ebo = EBO(indices.data(), static_cast<GLsizeiptr>(indices.size() * sizeof(GLuint)));

		vao.link_attributes(vbo, 0, 3, GL_FLOAT, sizeof(type::vertex), (void*)offsetof(type::vertex, position));
		vao.link_attributes(vbo, 1, 3, GL_FLOAT, sizeof(type::vertex), (void*)offsetof(type::vertex, color));
		vao.link_attributes(vbo, 2, 2, GL_FLOAT, sizeof(type::vertex), (void*)offsetof(type::vertex, uv));

		vao.unbind();
		vbo.unbind();
		ebo.unbind();

		original_vertices = vertices;

		scale_id = glGetUniformLocation(shader_program.id, "scale");
	}

    // Translate rectangle
	void rotate_by(float _angle)
	{
		angle += _angle; // accumulate rotation
		apply_transform();
	}

	void set_position(float x, float y)
	{
		position = {x, y, 0.0f};
		apply_transform();
	}

	void move_by(float dx, float dy)
	{
		position += {dx, dy, 0.0f};
		apply_transform();
	}

	void debug_transform()
	{
		std::cout << "Rectangle transform debug:\n";
		std::cout << "  Position: (" << position.x << ", " << position.y << ")\n";
		std::cout << "  Angle: " << angle << "\n";

		for (size_t i = 0; i < vertices.size(); ++i)
		{
			auto &orig = original_vertices[i].position;
			auto &trans = vertices[i].position;
			std::cout << "  Vertex " << i
					  << ": before = (" << orig.x << ", " << orig.y << ", " << orig.z << ")"
					  << ", after = (" << trans.x << ", " << trans.y << ", " << trans.z << ")\n";
		}
	}

	void apply_transform()
	{
		// Build transform matrix: scale -> rotate -> translate
		math::matrix3<float> transform_matrix =
			math::matrix3<float>::translation({position.x, position.y}) *
			math::matrix3<float>::rotation(angle) *
			math::matrix3<float>::scaling({1.0f, 1.0f});

		// Apply transform to each vertex
		for (size_t i = 0; i < vertices.size(); ++i)
		{
			auto before = vertices[i].position;
			vertices[i].position = math::vector::transform(original_vertices[i].position, transform_matrix);

			// Debug output
			std::cout << transform_matrix.m[0][0] << " " << transform_matrix.m[0][1] << " " << transform_matrix.m[0][2] << "\n";
			std::cout << transform_matrix.m[1][0] << " " << transform_matrix.m[1][1] << " " << transform_matrix.m[1][2] << "\n";
			std::cout << transform_matrix.m[2][0] << " " << transform_matrix.m[2][1] << " " << transform_matrix.m[2][2] << "\n";
		}

		debug_transform();

		// VBO upload
		vbo.bind();
		glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<GLsizeiptr>(vertices.size() * sizeof(type::vertex)), vertices.data());
		vbo.unbind();
	}

    // Draw rectangle
    void draw()
    {
        shader_program.activate();
        glUniform1f(static_cast<GLint>(scale_id), 0.1f);
        glBindTexture(GL_TEXTURE_2D, texture.id);
        vao.bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }

    // Free GPU resources
    void destroy()
    {
        vao.destroy();
        vbo.destroy();
        ebo.destroy();
        texture.destroy();
    }
};

int main()
{
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a Window object of 800 by 800 pixels, naming it "YoutubeOpenGL"
	Window window(800, 800, "RegenX");

	// Generates Shader object using shaders defualt.vert and default.frag
	const Shader shader_program("resource/shaders/default.vert", "resource/shaders/default.frag");

	std::vector<type::vertex> vertices1 = {
		//  COORDINATES          COLORS            UVs
		{{-0.5f, -0.5f, 0.1f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}}, // Bottom left
		{{ 0.5f, -0.5f, 0.1f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}, // Bottom right
		{{ 0.5f,  0.5f, 0.1f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}}, // Top right
		{{-0.5f,  0.5f, 0.1f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}}  // Top left
	};

	std::vector<GLuint> indices1 = {
		0, 1, 2, // Bottom-left triangle
		2, 3, 0  // Top-right triangle
	};
	rectangle rect_1(shader_program, vertices1, indices1);

	std::vector<type::vertex> vertices2 = {
		//  COORDINATES          COLORS            UVs
		{{-0.25f, -0.25f, 0.25f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}}, // Bottom left
		{{ 0.25f, -0.25f, 0.25f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}, // Bottom right
		{{ 0.25f,  0.25f, 0.25f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}}, // Top right
		{{-0.25f,  0.25f, 0.25f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}}  // Top left
	};

	std::vector<GLuint> indices2 = {
		0, 1, 2, // Bottom-left triangle
		2, 3, 0  // Top-right triangle
	};
	rectangle rect_2(shader_program, vertices2, indices2);

	while (!window.should_close())
	{
		auto frame_start = std::chrono::high_resolution_clock::now();
		// Clear the screen
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Use the shader
		shader_program.activate();

		// Draw rectangle
		rect_1.draw();
		rect_2.draw();

		// --- Input handling ---
		if (glfwGetKey(window.get(), GLFW_KEY_W) == GLFW_PRESS) rect_1.move_by(0.0f, 0.1f);   // up
		if (glfwGetKey(window.get(), GLFW_KEY_S) == GLFW_PRESS) rect_1.move_by(0.0f, -0.1f);  // down
		if (glfwGetKey(window.get(), GLFW_KEY_A) == GLFW_PRESS) rect_1.move_by(-0.1f, 0.0f);  // left
		if (glfwGetKey(window.get(), GLFW_KEY_D) == GLFW_PRESS) rect_1.move_by(0.1f, 0.0f);   // right

		if (glfwGetKey(window.get(), GLFW_KEY_Q) == GLFW_PRESS) rect_1.rotate_by(1.f);  // left
		if (glfwGetKey(window.get(), GLFW_KEY_E) == GLFW_PRESS) rect_1.rotate_by(-1.f); // right

		if (glfwGetKey(window.get(), GLFW_KEY_W) == GLFW_PRESS) rect_2.move_by(0.0f, 0.1f);   // up
		if (glfwGetKey(window.get(), GLFW_KEY_S) == GLFW_PRESS) rect_2.move_by(0.0f, -0.1f);  // down
		if (glfwGetKey(window.get(), GLFW_KEY_A) == GLFW_PRESS) rect_2.move_by(-0.1f, 0.0f);  // left
		if (glfwGetKey(window.get(), GLFW_KEY_D) == GLFW_PRESS) rect_2.move_by(0.1f, 0.0f);   // right

		if (glfwGetKey(window.get(), GLFW_KEY_Q) == GLFW_PRESS) rect_2.rotate_by(1.f);  // left
		if (glfwGetKey(window.get(), GLFW_KEY_E) == GLFW_PRESS) rect_2.rotate_by(-1.f); // right

		// Swap buffers and poll events
		window.swap_buffers();
		glfwPollEvents();

		auto frame_end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = frame_end - frame_start;
		double sleep_time = frame_duration - elapsed.count();
		if (sleep_time > 0)
			std::this_thread::sleep_for(std::chrono::duration<double>(sleep_time));
	}


	// Delete all the objects we've created
	rect_1.destroy();

	shader_program.destroy();
	// Delete window before ending the program
	window.destroy();
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}