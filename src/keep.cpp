#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <RegenX/Window/RegenXWindow.hpp>
#include <RegenX/Renderer/Shader.hpp>
#include <RegenX/Renderer/VAO.hpp>
#include <RegenX/Renderer/VBO.hpp>
#include <RegenX/Renderer/EBO.hpp>
#include <RegenX/Renderer/Texture.hpp>

class rectangle
{
	Shader shader_program;

	VAO vao;
	VBO vbo;
	EBO ebo;
	Texture texture;

	GLuint scale_id;

	std::vector<GLfloat> vertices = {
		//  COORDINATES  //     // COLORS	   //		// TEXTURE COORDINATES
		-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,		0.0f, 1.0f,// Bottom left  → Red
		 0.5f, -0.5f, 0.0f,		1.0f, 1.0f, 1.0f,		1.0f, 1.0f,// Bottom right → White
		 0.5f,  0.5f, 0.0f,		0.0f, 0.0f, 1.0f,		1.0f, 0.0f,// Top right    → Blue
		-0.5f,  0.5f, 0.0f,		0.0f, 1.0f, 0.0f, 		0.0f, 0.0f// Top left     → Green
	};

	std::vector<GLuint> indices = {
		0, 1, 2, // Bottom-left triangle
		2, 3, 0  // Top-right triangle
	};

	public:
	rectangle(const Shader &sp) : shader_program(sp), vbo(vertices, sizeof(vertices)), ebo(indices, sizeof(indices)),
	                              texture("resource/textures/khat.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE)
	{
		// Generates Vertex Array Object and binds it
		vao.bind();

		// Generates Vertex Buffer Object and links it to vertices
		vbo = VBO(vertices, sizeof(vertices));
		// Generates Element Buffer Object and links it to indices
		ebo = EBO(indices, sizeof(indices));

		// Links VBO attributes such as coordinates and colors to VAO
		vao.link_attributes(vbo, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
		vao.link_attributes(vbo, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		vao.link_attributes(vbo, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		// Unbind all to prevent accidentally modifying them
		vao.unbind();
		vbo.unbind();
		ebo.unbind();

		// Gets ID of uniform called "scale"
		scale_id = glGetUniformLocation(shader_program.id, "scale");

		// Texture
		texture = Texture("resource/textures/khat.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);

		GLuint text0_uni = glGetUniformLocation(shader_program.id, "tex0");
		shader_program.activate();
		glUniform1i(text0_uni, 0);
	}

	void draw()
	{
		// Assigns a value to the uniform; NOTE: Must always be done after activating the Shader Program
		glUniform1f(scale_id, 0.5f);
		glBindTexture(GL_TEXTURE_2D, texture.id);

		vao.bind();

		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

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
	Window window(800, 800);

	// Generates Shader object using shaders defualt.vert and default.frag
	const Shader shader_program("resource/shaders/default.vert", "resource/shaders/default.frag");

	rectangle rect_1(shader_program);

	// Main while loop
	while (!window.should_close())
	{
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		shader_program.activate();

		// Bind the VAO so OpenGL knows to use it
		rect_1.draw();

		// Swap the back buffer with the front buffer
		window.swap_buffers();
		// Take care of all GLFW events
		glfwPollEvents();
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