#include "Tank.h"

#include "Renderer/AnimatedSprite.h"

Tank::Tank(std::shared_ptr<Renderer::AnimatedSprite> 	pSprite, 
		   const float 									velocity, 
		   const glm::vec2&								position,
		   const glm::vec2&								size,
		   const float									rotation
		) 
		   	: IGameObject(position, size, rotation)
			, m_pSprite(pSprite)
			, m_velocity(velocity)
			, m_eOrientation(EOrientation::Top)
			, m_isMoving(false)
			, m_moveVector(glm::vec2(0.f, 1.f)) 
{
}

void Tank::render() const {
	m_pSprite->render(m_position, m_size, m_rotation);
}

void Tank::SetOrientation(const EOrientation orientetion) {
	if (m_eOrientation == orientetion)
		return;
	
	m_eOrientation = orientetion;

	switch (m_eOrientation)
	{
	case EOrientation::Top:
		m_pSprite->SetState("tankTopState");
		m_moveVector.x = 0.f;
		m_moveVector.y = 1.f;
		break;
	case EOrientation::Bottom:
		m_pSprite->SetState("tankBottomState");
		m_moveVector.x = 0.f;
		m_moveVector.y = -1.f;
		break;
	case EOrientation::Left:
		m_pSprite->SetState("tankLeftState");
		m_moveVector.x = -1.f;
		m_moveVector.y = 0.f;
		break;
	case EOrientation::Right:
		m_pSprite->SetState("tankRightState");
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
		m_pSprite->Update(deltaTime);
	}
}