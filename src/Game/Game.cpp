#include "Game.h"

#include <iostream>
#include <stdexcept>

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Resources/ResourceManager.h"
#include "Renderer/ShaderProgram.h"
#include "Renderer/Texture2D.h"
#include "Renderer/Sprite.h"
#include "Renderer/AnimatedSprite.h"

#include "Game/Tank.h"


Game::Game(const glm::ivec2& windowSize) 
	: m_eGameSate(Game::EGameState::Active),
	  m_windowSize(windowSize)
{
	m_keys.fill(false);
}

Game::~Game()
{

}

void Game::Render()
{
	ResourceManager::GetAnimatedSprite("defaultAnimatedSprite")->Render();
}

void Game::Update(const float deltaTime)
{
	ResourceManager::GetAnimatedSprite("defaultAnimatedSprite")->Update(deltaTime);
}

void Game::SetKey(const int key, const int action)
{
	m_keys[key] = action;
}

bool Game::Init()
{
	auto pDefaultShaderProgram = ResourceManager::LoadShaders("DefaulShader", 
															"res/shaders/vertex.glsl", 
															"res/shaders/fragment.glsl");
	if (!pDefaultShaderProgram)
		throw std::runtime_error("Failed to load shader program");

	auto pSpriteShaderProgram = ResourceManager::LoadShaders("SpriteShader", 
															"res/shaders/vSprite.glsl", 
															"res/shaders/fSprite.glsl");
	if (!pSpriteShaderProgram)
		throw std::runtime_error("Failed to load shader program");

	auto pTexture = ResourceManager::LoadTexture("DefaultTexture", "res/textures/map_16x16.png");	
	if (!pTexture)
		throw std::runtime_error("Failed to load texture");

	std::vector<std::string> subTexturesNames 
	{	"block", "topBlock", 
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
		"respawn1", "respawn2", "respawn3", "respawn4" 
	};

	auto pTextureAtlas = ResourceManager::LoadTextureAtlas("defaultTextureAtlas", 
															"res/textures/map_16x16.png",
															std::move(subTexturesNames), 	
															16, 
															16);

	auto pSprite = ResourceManager::LoadSprite("defaultSprite", 
												pTextureAtlas->GetName(),
												pSpriteShaderProgram->GetName(), 
												100, 
												100, 
												"block");
	if (!pSprite)
		throw std::runtime_error("Failed to load sprite");

	pSprite->SetPosition(glm::vec2(200, 200));

	auto pAnimatedSprite = ResourceManager::LoadAnimatedSprite("defaultAnimatedSprite", 
																pTextureAtlas->GetName(),
																pSpriteShaderProgram->GetName(), 
																100, 
																100, 
																"water1");
	if (!pAnimatedSprite)
		throw std::runtime_error("Failed to load animated sprite");

	pAnimatedSprite->SetPosition(glm::vec2(400, 200));
	std::vector<std::pair<std::string, float>> waterState;
	waterState.emplace_back("water2", 0.5f);
	waterState.emplace_back("water3", 0.5f);

	pAnimatedSprite->InsertState("waterState", std::move(waterState));
	pAnimatedSprite->SetState("waterState");

	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

	glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(m_windowSize.x), 
											0.f, static_cast<float>(m_windowSize.y), 
											-100.f, 100.f);
		
	pDefaultShaderProgram->SetMat4("projectionMat", projectionMatrix);

	pSpriteShaderProgram->Use();
	pSpriteShaderProgram->SetInt("tex", 0);
	pSpriteShaderProgram->SetMat4("projectionMat", projectionMatrix);


	std::vector<std::string> tanksSubTexturesNames 
	{	
		"tankTop1", "tankTop2",
		"tankLeft1", "tankLeft2",
		"tankBottom1", "tankBottom2",
		"tankRight1", "tankRight2"
	};

	auto pTankTextureAtlas = ResourceManager::LoadTextureAtlas(	"TanksTextureAtlas",
															   	"res/textures/tanks.png",
															   	tanksSubTexturesNames,
																16,
																16);

	return true;
}