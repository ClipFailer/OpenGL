#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

#include <iostream>

#include "Renderer/ShaderProgram.h"
#include "Renderer/Texture2D.h"
#include "Tools/Tools.h"
#include "Resources/ResourceManager.h"

glm::ivec2 windowSize(800, 600);

GLfloat points[]
{
	// Вершины				// Цвета 				// Текстура
	-0.5f, 0.0f, 0.0f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f, 
	-0.5f, 0.5f, 0.0f,		0.0f, 1.0f, 0.0f,		0.0f, 1.0f,
	0.5f, 0.5f, 0.0f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,
	0.5f, 0.0f, 0.0f,       0.0f, 0.0f, 1.0f,		1.0f, 0.0f,
};

GLuint indeces[]
{
	0, 1, 2,
	2, 3, 0,
};

void changeWindowSizeCallback(GLFWwindow *window, int width, int height)
{
	windowSize.x = width;
	windowSize.y = height;
	glViewport(0, 0, windowSize.x, windowSize.y);
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

		auto texture = resourceManager.loadTexture("DefaultTexture", "res/textures/map_16x16.png");	
		if (!texture)
			ASSERT(false);

		std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
		std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

		GLuint vao = 0;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		GLuint vbo = 0;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

		GLuint ibo = 0;
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces), indeces, GL_STATIC_DRAW);

		// Вершины
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, 
							  GL_FLOAT, GL_FALSE, 
							  8 * sizeof(GL_FLOAT), (void*)0);

		// Цвета
		glEnableVertexAttribArray(1);
		glcall(glVertexAttribPointer(1, 3, 
									 GL_FLOAT, GL_FALSE, 
									 8 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT))));

		// Текстуры
		glEnableVertexAttribArray(2);
		glcall(glVertexAttribPointer(2, 2, 
									 GL_FLOAT, GL_FALSE, 
									 8 * sizeof(GL_FLOAT), (void*)(6 * sizeof(GL_FLOAT))));

		glBindVertexArray(0);

		pDefaultShaderProgram->Use();
		pDefaultShaderProgram->SetInt("tex", 0);
		
		glcall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		while (!glfwWindowShouldClose(window))
		{
			glcall(glClear(GL_COLOR_BUFFER_BIT));

			pDefaultShaderProgram->Use();
			glcall(glBindVertexArray(vao));
			texture->Bind();

			glcall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

			glcall(glBindVertexArray(0));

			glfwSwapBuffers(window);

			glfwPollEvents();
		}
	}

	glfwTerminate();
	return 0;
}