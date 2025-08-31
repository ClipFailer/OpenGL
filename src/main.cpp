#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "Renderer/ShaderProgram.h"
#include "Renderer/Texture2D.h"
#include "Renderer/Sprite.h"
#include "Tools/Tools.h"
#include "Resources/ResourceManager.h"

glm::ivec2 windowSize(800, 600);

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

		auto pSpriteShaderProgram = resourceManager.LoadShaders("SpriteShader", "res/shaders/vSprite.glsl", "res/shaders/fSprite.glsl");
		if (!pSpriteShaderProgram)
			ASSERT(false);

		auto texture = resourceManager.loadTexture("DefaultTexture", "res/textures/map_16x16.png");	
		if (!texture)
			ASSERT(false);

		auto sprite = resourceManager.LoadSprite("defaultSprite", texture->GetName(),
												 pSpriteShaderProgram->GetName(), 200, 100);
		if (!sprite)
			ASSERT(false);

		sprite->SetPosition(glm::vec2(300, 300));

		std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
		std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

		glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(windowSize.x), 
												0.f, static_cast<float>(windowSize.y), 
												-100.f, 100.f);
		
		pDefaultShaderProgram->SetMat4("projectionMat", projectionMatrix);

		pSpriteShaderProgram->Use();
		pSpriteShaderProgram->SetInt("tex", 0);
		pSpriteShaderProgram->SetMat4("projectionMat", projectionMatrix);

		glcall(glClearColor(0.0f, 0.1f, 0.2f, 1.0f));
		while (!glfwWindowShouldClose(window))
		{
			glcall(glClear(GL_COLOR_BUFFER_BIT));

			sprite->Render();

			glfwSwapBuffers(window);

			glfwPollEvents();
		}
	}

	glfwTerminate();
	return 0;
}