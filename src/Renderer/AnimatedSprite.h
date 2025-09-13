#pragma once


#include "Sprite.h"

#include <map>
#include <vector>

namespace Renderer {

	class AnimatedSprite : public Sprite {
	public:
		AnimatedSprite(const std::string &name,
					   const std::shared_ptr<Texture2D> pTexture,
					   const std::string &initialSubTexture,
					   const std::shared_ptr<ShaderProgram> pShaderProgram,
					   const glm::vec2& position = glm::vec2(0.f),
					   const glm::vec2& size = glm::vec2(1.f),
					   const float rotation = 0.f);

		void Render() const override;
		
		void InsertState(const std::string& stateName,
						 std::vector<std::pair<std::string, float>> stateSubTextures);

		void SetState(const std::string& stateName);
		void Update(const float deltaTime);

	private:
		std::map<std::string, std::vector<std::pair<std::string, float>>> m_states;
		size_t m_currentFrame = 0;
		float m_currantAnimationTime = 0.f;
		std::map<std::string, std::vector<std::pair<std::string, float>>>::const_iterator m_pCurrentAnimationState;
		mutable bool m_redrawSubTexture = false;
	};
}