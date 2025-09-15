#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "Game/Game.h"
#include "Tools/Tools.h"
#include "Resources/ResourceManager.h"

glm::ivec2 g_windowSize(800, 600);

Game g_game(g_windowSize);

void changeWindowSizeCallback(
	GLFWwindow 	*window, 
	int 		width, 
	int 		height
) {
	g_windowSize.x = width;
	g_windowSize.y = height;
	glViewport(0, 0, g_windowSize.x, g_windowSize.y);
}

void keyHandlerCallback(
	GLFWwindow 	*window, 
	int 		key, 
	int 		scancode, 
	int 		action, 
	int 		mode
) {
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
		std::cerr << "Failed to initialized GLFW\n";
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL Tutorial", nullptr, nullptr);
	if (!window)
	{
		std::cerr << "Failed to create window\n";
		glfwTerminate();
		return -1;
	}

	glfwSetWindowSizeCallback(window, changeWindowSizeCallback);
	glfwSetKeyCallback(window, keyHandlerCallback);

	glfwMakeContextCurrent(window);

	if (!gladLoadGL(glfwGetProcAddress))
	{
		std::cerr << "Failed to inisialized GLAD\n";
		glfwTerminate();
		return -1;
	}

	{
		ResourceManager::SetPath(argv[0]);

		if (!g_game.Init())
			throw std::runtime_error("Failed to inizialize game");

		glcall(glClearColor(0.0f, 0.1f, 0.2f, 1.0f));

		double lastLite = glfwGetTime();
		while (!glfwWindowShouldClose(window))
		{
			double currentTime = glfwGetTime();
			float deltaTime = static_cast<float>(currentTime - lastLite);
			lastLite = currentTime;

			g_game.Update(deltaTime);

			glcall(glClear(GL_COLOR_BUFFER_BIT));

			g_game.Render();

			glfwSwapBuffers(window);

			glfwPollEvents();
		}
	}

	glfwTerminate();
	return 0;
}