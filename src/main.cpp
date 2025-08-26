#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Renderer/ShaderProgram.h"
#include "Tools/Tools.h"
#include "Resources/ResourceManager.h"

int windowWidth = 800;
int windowHeight = 600;

GLfloat points[]
{
	-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,
	0.0f, 0.5f, 0.0f,		0.0f, 1.0f, 0.0f,
	0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,
};

void changeWindowSizeCallback(GLFWwindow *window, int width, int height)
{
	windowWidth = width;
	windowHeight = height;
	glViewport(0, 0, windowWidth, windowHeight);
}

void keyHandlerCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		std::cout << "Window has closed!\n";
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

int main(int argc, char** argv)
{
	

	if (!glfwInit())
	{
		std::cout << "Failed to initialized GLFW\n";
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL Tutorial", nullptr, nullptr);
	if (!window)
	{
		std::cout << "Failed to create window\n";
		glfwTerminate();
		return -1;
	}

	glfwSetWindowSizeCallback(window, changeWindowSizeCallback);
	glfwSetKeyCallback(window, keyHandlerCallback);

	glfwMakeContextCurrent(window);

	if (!gladLoadGL(glfwGetProcAddress))
	{
		std::cout << "Failed to inisialized GLAD\n";
		glfwTerminate();
		return -1;
	}

	{
		ResourceManager resourceManager(argv[0]);
		auto pDefaultShaderProgram = resourceManager.LoadShaders("DefaulShader", "res/shaders/vertex.glsl", "res/shaders/fragment.glsl");
		if (!pDefaultShaderProgram)
			ASSERT(false);

		if (!resourceManager.loadTexture("DefaultTexture", "res/textures/map_16x16.png"))
			ASSERT(false);

		std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
		std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

		GLuint vbo = 0;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

		GLuint vao = 0;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (void*)0);

		glEnableVertexAttribArray(1);
		glcall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT))));
		
		glcall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		while (!glfwWindowShouldClose(window))
		{
			glcall(glClear(GL_COLOR_BUFFER_BIT));

			pDefaultShaderProgram->Use();
			glcall(glBindVertexArray(vao));

			glcall(glDrawArrays(GL_TRIANGLES, 0, 3));

			glcall(glBindVertexArray(0));

			glfwSwapBuffers(window);

			glfwPollEvents();
		}
	}

	glfwTerminate();
	return 0;
}