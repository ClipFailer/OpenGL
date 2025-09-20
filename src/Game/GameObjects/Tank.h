#pragma once

#include <memory>

#include <glm/vec2.hpp>

#include "IGameObject.h"

namespace Renderer {
	class AnimatedSprite;
}

class Tank : IGameObject {
public:
	enum EOrientation
	{
		Top,
		Bottom,
		Right,
		Left
	};

	Tank(
		std::shared_ptr<Renderer::AnimatedSprite> 	pSprite,
		const float 								velocity,
		const glm::vec2& 							position,
		const glm::vec2& 							size,
		const float									rotation = 0
	);

	void render() const override;	
	void SetOrientation(const EOrientation orientetion);
	void Move(const bool isMoving);
	void update(const float deltaTime) override;

private:
	std::shared_ptr<Renderer::AnimatedSprite> 	m_pSprite;
	EOrientation 								m_eOrientation;
	bool 										m_isMoving;
	float 										m_velocity;
	glm::vec2 									m_moveVector;

};