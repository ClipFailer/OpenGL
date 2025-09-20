#pragma once

#include "Sprite.h"

#include <map>
#include <vector>

using AnimationStates = std::map<std::string, std::vector<std::pair<std::string, float>>>;
using AnimationStatesIterator = AnimationStates::const_iterator;

using SubTexturesDuration = std::vector<std::pair<std::string, float>>;

namespace Renderer {

	class AnimatedSprite : public Sprite {
	public:
		AnimatedSprite(
			const std::string&						name,
			const std::shared_ptr<Texture2D> 		pTexture,
			const std::string&						initialSubTexture,
			const std::shared_ptr<ShaderProgram> 	pShaderProgram
		);

		void render(
			const glm::vec2& 	position,
			const glm::vec2& 	size,
			const float 		rotation
		) const override;
		
		void InsertState(
			const std::string& 		stateName,
			SubTexturesDuration 	stateSubTextures
		);

		void SetState(const std::string& stateName);
		void Update(const float deltaTime);

	private:
		AnimationStates				m_states;
		size_t 						m_currentFrame 				= 0;
		float 						m_currantAnimationTime 		= 0.f;
		AnimationStatesIterator		m_pCurrentAnimationState;
		mutable bool 				m_redrawSubTexture 			= false;
	};
}