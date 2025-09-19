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
		if (m_keys[GLFW_KEY_W] || m_keys[GLFW_KEY_UP]){
			m_pTank->SetOrientation(Tank::EOrientation::Top);
            m_pTank->Move(true);
		} else if (m_keys[GLFW_KEY_S]|| m_keys[GLFW_KEY_DOWN]) {
            m_pTank->SetOrientation(Tank::EOrientation::Bottom);
            m_pTank->Move(true);
		} else if (m_keys[GLFW_KEY_A] || m_keys[GLFW_KEY_LEFT]) {
            m_pTank->SetOrientation(Tank::EOrientation::Left);
            m_pTank->Move(true);
		} else if (m_keys[GLFW_KEY_D] || m_keys[GLFW_KEY_RIGHT]) {
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
	ResourceManager::loadJsonResources("res/resources.json");

	auto pSpriteShaderProgram = ResourceManager::GetShaderProgram("spriteShader");
	if (!pSpriteShaderProgram)
		throw std::runtime_error("Failed to load shader program");

	auto pMapTextureAtlas = ResourceManager::GetTexture("mapTextureAtlas");
	if (!pMapTextureAtlas)
		throw std::runtime_error("Failed to load map texture atlas");

	auto pTanksTextureAtlas = ResourceManager::GetTexture("tanksTextureAtlas");
	if (!pTanksTextureAtlas)
		throw std::runtime_error("Failed to load tanks texture atlas");

	auto pTanksAnimatedSprite = ResourceManager::GetAnimatedSprite("tanksAnimatedSprite");

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

	pSpriteShaderProgram->Use();
	pSpriteShaderProgram->SetInt("tex", 0);
	pSpriteShaderProgram->SetMat4("projectionMat", projectionMatrix);

	return true;
}