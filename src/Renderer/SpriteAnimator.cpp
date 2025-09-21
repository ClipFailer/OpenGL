#include "SpriteAnimator.h"

#include "Sprite.h"

namespace Renderer {
SpriteAnimator::SpriteAnimator(std::shared_ptr<Sprite> pSprite)
	: m_pSprite(std::move(pSprite)) 
	, m_currentFrame(0)
	, m_currentFrameTime(m_pSprite->getFrameTime(0))
	, m_currentAnimationTime(0)
{
	
}

void SpriteAnimator::update(const float deltaTime) {
	m_currentAnimationTime += deltaTime;

	while (m_currentAnimationTime >= m_currentFrameTime) {
		m_currentAnimationTime -= m_currentFrameTime;
		++m_currentFrame;

		if (m_currentFrame == m_pSprite->getFramesCount())
			m_currentFrame = 0;
		
		m_currentFrameTime = m_pSprite->getFrameTime(m_currentFrame);
	}
}

} // namespace Renderer
