#pragma once

#include <array>
#include <memory>

#include <glm/vec2.hpp>

class Tank;

class Game
{
public:
	Game(const glm::ivec2& windowSize);
	~Game();

	void Render();
	void Update(const float deltaTime);
	void SetKey(const int key, const int action);
	bool Init();

private:
	std::array<bool, 349> m_keys;

	enum EGameState
	{
		Active,
		Pause,
	};

	EGameState m_eGameState;
	glm::ivec2 m_windowSize;

	std::unique_ptr<Tank> m_pTank;
};