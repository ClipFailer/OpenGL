#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "Renderer/ShaderProgram.h"
#include "Renderer/Texture2D.h"
#include "Renderer/Sprite.h"
#include "Renderer/AnimatedSprite.h"

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
		auto pDefaultShaderProgram = resourceManager.LoadShaders("DefaulShader", 
																 "res/shaders/vertex.glsl", 
																 "res/shaders/fragment.glsl");
		if (!pDefaultShaderProgram)
			ASSERT(false);

		auto pSpriteShaderProgram = resourceManager.LoadShaders("SpriteShader", 
																"res/shaders/vSprite.glsl", 
																"res/shaders/fSprite.glsl");
		if (!pSpriteShaderProgram)
			ASSERT(false);

		auto pTexture = resourceManager.LoadTexture("DefaultTexture", "res/textures/map_16x16.png");	
		if (!pTexture)
			ASSERT(false);

		std::vector<std::string> subTexturesNames = {	"block", "topBlock", 
														"bottomBlock", "leftBlock", 
														"rightBlock", "topLeftBlock", 
														"topRightBlock", "bottomRightBlock", 
														"bottomLeftBlock",	

														"beton", "topBeton",
														"bottomBeton","leftBeton", "rightBeton",
														"topLeftBeton", "topRightBeton",
														"bottomLeftBeton", "bottomRightBeton",

														"water1", "water2", "water3",
														"trees", "ice", "wall",
														"eagle", "deadEagle",
														"nothing",
														"respawn1", "respawn2", "respawn3", "respawn4" };

		auto pTextureAtlas = resourceManager.LoadTextureAtlas("defaultTextureAtlas", 
															  "res/textures/map_16x16.png",
															  std::move(subTexturesNames), 	
															  16, 16);

		auto pSprite = resourceManager.LoadSprite("defaultSprite", pTextureAtlas->GetName(),
												 pSpriteShaderProgram->GetName(), 100, 100, "block");
		if (!pSprite)
			ASSERT(false);

		pSprite->SetPosition(glm::vec2(200, 200));

		auto pAnimatedSprite = resourceManager.LoadAnimatedSprite("defaultAnimatedSprite", pTextureAtlas->GetName(),
																  pSpriteShaderProgram->GetName(), 100, 100, "water1");
		if (!pAnimatedSprite)
		ASSERT(false);

		pAnimatedSprite->SetPosition(glm::vec2(400, 200));
		std::vector<std::pair<std::string, float>> waterState;
		waterState.emplace_back("water2", 0.5f);
		waterState.emplace_back("water3", 0.5f);

		pAnimatedSprite->InsertState("waterState", std::move(waterState));
		pAnimatedSprite->SetState("waterState");

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

		double lastLite = glfwGetTime();
		while (!glfwWindowShouldClose(window))
		{
			double currentTime = glfwGetTime();
			float deltaTime = static_cast<float>(currentTime - lastLite);
			lastLite = currentTime;

			pAnimatedSprite->Update(deltaTime);

			glcall(glClear(GL_COLOR_BUFFER_BIT));

			pSprite->Render();
			pAnimatedSprite->Render();

			glfwSwapBuffers(window);

			glfwPollEvents();
		}
	}

	glfwTerminate();
	return 0;
}