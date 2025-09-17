#pragma once

#include <memory>

#include <glm/vec2.hpp>

namespace Renderer {
	class AnimatedSprite;
}

class Tank {
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
		const glm::vec2& 							position
	);

	void Render() const;	
	void SetOrientation(const EOrientation orientetion);
	void Move(const bool isMoving);
	void Update(const float deltaTime);

private:
	std::shared_ptr<Renderer::AnimatedSprite> 	m_pSprite;
	EOrientation 								m_eOrientation;
	bool 										m_isMoving;
	float 										m_velocity;
	glm::vec2 									m_position;
	glm::vec2 									m_moveVector;

};