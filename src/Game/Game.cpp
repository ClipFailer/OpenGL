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

#include <GLFW/glfw3.h>

#include "Game/Tank.h"


Game::Game(const glm::ivec2& windowSize) 
	: m_eGameState(Game::EGameState::Active)
	, m_windowSize(windowSize)
{
	m_keys.fill(false);
}

Game::~Game()
{

}

void Game::Render(){
	// ResourceManager::GetAnimatedSprite("defaultAnimatedSprite")->Render();
	if (m_pTank) {
		m_pTank->Render();
	}
}

void Game::Update(const float deltaTime){
	// ResourceManager::GetAnimatedSprite("defaultAnimatedSprite")
	// 	->Update(deltaTime);

	if (m_pTank) {
		m_pTank->Update(deltaTime);			
		if (m_keys[GLFW_KEY_W]){
			m_pTank->SetOrientation(Tank::EOrientation::Top);
            m_pTank->Move(true);
		} else if (m_keys[GLFW_KEY_S]) {
            m_pTank->SetOrientation(Tank::EOrientation::Bottom);
            m_pTank->Move(true);
		} else if (m_keys[GLFW_KEY_A]) {
            m_pTank->SetOrientation(Tank::EOrientation::Left);
            m_pTank->Move(true);
		} else if (m_keys[GLFW_KEY_D]) {
            m_pTank->SetOrientation(Tank::EOrientation::Right);
            m_pTank->Move(true);
		} else {
            m_pTank->Move(false);
        }
	}
}

void Game::SetKey(const int key, const int action){
	m_keys[key] = action;
}

bool Game::Init(){
	auto pDefaultShaderProgram = ResourceManager::LoadShaders(
		"DefaulShader", 
		"res/shaders/vertex.glsl", 
		"res/shaders/fragment.glsl"
	);
	if (!pDefaultShaderProgram)
		throw std::runtime_error("Failed to load shader program");

	auto pSpriteShaderProgram = ResourceManager::LoadShaders(
		"SpriteShader", 
		"res/shaders/vSprite.glsl", 
		"res/shaders/fSprite.glsl"
	);
	if (!pSpriteShaderProgram)
		throw std::runtime_error("Failed to load shader program");

	auto pTexture = ResourceManager::LoadTexture(
		"DefaultTexture", 
		"res/textures/map_16x16.png"
	);	
	if (!pTexture)
		throw std::runtime_error("Failed to load texture");

	std::vector<std::string> subTexturesNames {	
		"block", "topBlock", 
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

	auto pTextureAtlas = ResourceManager::LoadTextureAtlas(
		"defaultTextureAtlas", 
		"res/textures/map_16x16.png",
		std::move(subTexturesNames),
		16, 
		16
	);

	auto pSprite = ResourceManager::LoadSprite(
		"defaultSprite", 
		pTextureAtlas->GetName(),
		pSpriteShaderProgram->GetName(), 
		100, 
		100, 
		"block"
	);
	if (!pSprite)
		throw std::runtime_error("Failed to load sprite");

	pSprite->SetPosition(glm::vec2(200, 200));

	auto pAnimatedSprite = ResourceManager::LoadAnimatedSprite (
		"defaultAnimatedSprite", 
		pTextureAtlas->GetName(),
		pSpriteShaderProgram->GetName(), 
		100, 
		100, 
	 	"water1"
	);
	if (!pAnimatedSprite)
		throw std::runtime_error("Failed to load animated sprite");

	pAnimatedSprite->SetPosition(glm::vec2(400, 200));

	std::vector<std::pair<std::string, float>> waterState;
	waterState.emplace_back("water2", 0.5f);
	waterState.emplace_back("water3", 0.5f);

	pAnimatedSprite->InsertState("waterState", std::move(waterState));
	pAnimatedSprite->SetState("waterState");

	std::vector<std::string> tanksSubTexturesNames {	
		"tankTop1", "tankTop2",
		"tankLeft1", "tankLeft2",
		"tankBottom1", "tankBottom2",
		"tankRight1", "tankRight2"
	};

	auto pTankTextureAtlas = ResourceManager::LoadTextureAtlas(	
		"TanksTextureAtlas",
		"res/textures/tanks.png",
		tanksSubTexturesNames,
		16,
		16
	);

	auto pTanksAnimatedSprite = ResourceManager::LoadAnimatedSprite(
		"tanks",
		pTankTextureAtlas->GetName(),
		pSpriteShaderProgram->GetName(),
		100,
		100,
		"tankTop1"
	);


	std::vector<std::pair<std::string, float>> tankTopState;
	tankTopState.emplace_back("tankTop1", 0.5f);
	tankTopState.emplace_back("tankTop2", 0.5f);

	std::vector<std::pair<std::string, float>> tankBottomState;
	tankBottomState.emplace_back("tankBottom1", 0.5f);
	tankBottomState.emplace_back("tankBottom2", 0.5f);

	std::vector<std::pair<std::string, float>> tankLeftState;
	tankLeftState.emplace_back("tankLeft1", 0.5f);
	tankLeftState.emplace_back("tankLeft2", 0.5f);

	std::vector<std::pair<std::string, float>> tankRightState;
	tankRightState.emplace_back("tankRight1", 0.5f);
	tankRightState.emplace_back("tankRight2", 0.5f);

	pTanksAnimatedSprite->InsertState(
		"tankTopState", std::move(tankTopState)
	);
	pTanksAnimatedSprite->InsertState(
		"tankBottomState", std::move(tankBottomState)
	);
	pTanksAnimatedSprite->InsertState(
		"tankLeftState", std::move(tankLeftState)
	);
	pTanksAnimatedSprite->InsertState(
		"tankRightState", std::move(tankRightState)
	);

	m_pTank = std::make_unique<Tank>(
		pTanksAnimatedSprite, 100.f, glm::vec2(300.f, 150.f)
	);

	glm::mat4 projectionMatrix = glm::ortho(
		0.f, 
		static_cast<float>(m_windowSize.x), 
		0.f, 
		static_cast<float>(m_windowSize.y), 
		-100.f, 
		100.f
	);
		
	pDefaultShaderProgram->SetMat4("projectionMat", projectionMatrix);

	pSpriteShaderProgram->Use();
	pSpriteShaderProgram->SetInt("tex", 0);
	pSpriteShaderProgram->SetMat4("projectionMat", projectionMatrix);

	return true;
}