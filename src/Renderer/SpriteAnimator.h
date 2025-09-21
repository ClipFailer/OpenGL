#pragma once

#include <memory>

namespace Renderer {
	class Sprite;

	class SpriteAnimator {
	public:
		SpriteAnimator(std::shared_ptr<Sprite> pSprite);

		size_t getCurrentFrame() const noexcept { return m_currentFrame; };
		void update(const float deltaTime);

	private:
		std::shared_ptr<Sprite> 	m_pSprite;
		size_t 						m_currentFrame;
		float 						m_currentFrameTime;
		float						m_currentAnimationTime;
	};
}
