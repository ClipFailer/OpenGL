#pragma once

#include <memory>

#include <glm/vec2.hpp>

#include "IGameObject.h"
#include "Renderer/SpriteAnimator.h"

namespace Renderer {
	class Sprite;
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
		std::shared_ptr<Renderer::Sprite>&& 	pSpriteTop,
		std::shared_ptr<Renderer::Sprite>&& 	pSpriteBottom,
		std::shared_ptr<Renderer::Sprite>&& 	pSpriteLeft,
		std::shared_ptr<Renderer::Sprite>&& 	pSpriteRight,
		const float 							velocity,
		const glm::vec2& 						position,
		const glm::vec2& 						size,
		const float								rotation 		= 0
	);

	void render() 											const override;	
	void SetOrientation(const EOrientation orientetion);
	void Move(const bool isMoving);
	void update(const float deltaTime) 						override;

private:
	std::shared_ptr<Renderer::Sprite> 	m_pSpriteTop;
	std::shared_ptr<Renderer::Sprite> 	m_pSpriteBottom;
	std::shared_ptr<Renderer::Sprite> 	m_pSpriteLeft;
	std::shared_ptr<Renderer::Sprite> 	m_pSpriteRight;
	Renderer::SpriteAnimator			m_spriteAnimatorTop;
	Renderer::SpriteAnimator			m_spriteAnimatorBottom;
	Renderer::SpriteAnimator			m_spriteAnimatorLeft;
	Renderer::SpriteAnimator			m_spriteAnimatorRight;

	EOrientation 						m_eOrientation;
	bool 								m_isMoving;
	float 								m_velocity;
	glm::vec2 							m_moveVector;

};