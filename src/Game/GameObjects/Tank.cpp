#include "Tank.h"

#include "Renderer/Sprite.h"

Tank::Tank(
	std::shared_ptr<Renderer::Sprite>&& 		pSpriteTop,
	std::shared_ptr<Renderer::Sprite>&& 		pSpriteBottom,
	std::shared_ptr<Renderer::Sprite>&& 		pSpriteLeft,
	std::shared_ptr<Renderer::Sprite>&& 		pSpriteRight,
	const float 								velocity,
	const glm::vec2& 							position,
	const glm::vec2& 							size,
	const float									rotation
) 
	: IGameObject(position, size, rotation)
	, m_pSpriteTop(std::move(pSpriteTop))
	, m_pSpriteBottom(std::move(pSpriteBottom))
	, m_pSpriteLeft(std::move(pSpriteLeft))
	, m_pSpriteRight(std::move(pSpriteRight))
	, m_spriteAnimatorTop(m_pSpriteTop)
	, m_spriteAnimatorBottom(m_pSpriteBottom)
	, m_spriteAnimatorLeft(m_pSpriteLeft)
	, m_spriteAnimatorRight(m_pSpriteRight)
	, m_velocity(velocity)
	, m_eOrientation(EOrientation::Top)
	, m_isMoving(false)
	, m_moveVector(glm::vec2(0.f, 1.f)) 
{
}

void Tank::render() const {
	switch (m_eOrientation)
	{
	case EOrientation::Top:
		m_pSpriteTop->render(
			m_position, 
			m_size, 
			m_rotation, 
			m_spriteAnimatorTop.getCurrentFrame()
		);
		break;
	case EOrientation::Bottom:
		m_pSpriteBottom->render(
			m_position, 
			m_size, 
			m_rotation, 
			m_spriteAnimatorBottom.getCurrentFrame()
		);
		break;
	case EOrientation::Left:
		m_pSpriteLeft->render(
			m_position, 
			m_size, 
			m_rotation, 
			m_spriteAnimatorLeft.getCurrentFrame()
		);
		break;
	case EOrientation::Right:
		m_pSpriteRight->render(
			m_position, 
			m_size, 
			m_rotation, 
			m_spriteAnimatorRight.getCurrentFrame()
		);
		break;
	
	default:
		break;
	}
}

void Tank::SetOrientation(const EOrientation orientetion) {
	if (m_eOrientation == orientetion)
		return;
	
	m_eOrientation = orientetion;

	switch (m_eOrientation)
	{
	case EOrientation::Top:
		m_moveVector.x = 0.f;
		m_moveVector.y = 1.f;
		break;
	case EOrientation::Bottom:
		m_moveVector.x = 0.f;
		m_moveVector.y = -1.f;
		break;
	case EOrientation::Left:
		m_moveVector.x = -1.f;
		m_moveVector.y = 0.f;
		break;
	case EOrientation::Right:
		m_moveVector.x = 1.f;
		m_moveVector.y = 0.f;
		break;
	
	default:
		break;
	}
}

void Tank::Move(const bool isMoving) {
	m_isMoving = isMoving;
}

void Tank::update(const float deltaTime) {
	if(m_isMoving)
	{
		m_position += deltaTime * m_velocity * m_moveVector;

		switch (m_eOrientation)
		{
		case EOrientation::Top:
			m_spriteAnimatorTop.update(deltaTime);
			break;
		case EOrientation::Bottom:
			m_spriteAnimatorBottom.update(deltaTime);
			break;
		case EOrientation::Left:
			m_spriteAnimatorLeft.update(deltaTime);
			break;
		case EOrientation::Right:
			m_spriteAnimatorRight.update(deltaTime);
			break;
		
		default:
			break;
		}

	}
}