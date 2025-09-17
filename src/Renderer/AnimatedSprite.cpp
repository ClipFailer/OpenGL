#include "AnimatedSprite.h"

#include <iostream>

#include <glad/gl.h>

#include "Texture2D.h"


namespace Renderer
{
	AnimatedSprite::AnimatedSprite(
		const std::string                    	&name, 
		const std::shared_ptr<Texture2D>     	pTexture, 
		const std::string                    	&initialSubTexture, 
		const std::shared_ptr<ShaderProgram> 	pShaderProgram, 
		const glm::vec2                      	&position, 
		const glm::vec2                      	&size, 
		float                          		 	rotation
	) 
		: Sprite(
			name, 
			std::move(pTexture), 
			initialSubTexture, 
			std::move(pShaderProgram),
			position,
			size,
			rotation
		)
	{
		m_pCurrentAnimationState = m_states.end();
	}

	void AnimatedSprite::Render() const
	{
		if (m_redrawSubTexture)
		{
			auto subTexture = m_pTexture->GetSubTexture(m_pCurrentAnimationState->second[m_currentFrame].first);

			std::vector<GLfloat> textureCoords
			{
				subTexture.leftBottomUV.x, 	subTexture.leftBottomUV.y,
				subTexture.leftBottomUV.x, 	subTexture.rightTopUV.y,
				subTexture.rightTopUV.x,   	subTexture.rightTopUV.y,
				subTexture.rightTopUV.x,   	subTexture.leftBottomUV.y,
			};

			m_textureVBO.update(
				textureCoords.data(), 
				textureCoords.size() * sizeof(GLfloat)
			);

			m_redrawSubTexture = false;
		}

		Sprite::Render();
	}
    
	void AnimatedSprite::InsertState(
		const std::string 						   &stateName, 
		std::vector<std::pair<std::string, float>> stateSubTextures
	) {
		m_states.emplace(std::move(stateName), std::move(stateSubTextures));
    }

    void AnimatedSprite::SetState(const std::string &stateName) 
	{
		auto it = m_states.find(stateName);

		if (it == m_states.end())
		{
			std::cout << "Can't find state: " << stateName << std::endl;
			return;
		}

		if (m_pCurrentAnimationState != it)
		{
			m_currantAnimationTime = 0;
			m_currentFrame = 0;
			m_pCurrentAnimationState = it;
			m_redrawSubTexture = true;
		}
    }

    void AnimatedSprite::Update(const float deltaTime) 
	{
        if (m_pCurrentAnimationState != m_states.end()) {
            m_currantAnimationTime += deltaTime;

            while (m_currantAnimationTime >= m_pCurrentAnimationState->second[m_currentFrame].second) {
                m_currantAnimationTime -= m_pCurrentAnimationState->second[m_currentFrame].second;
                ++m_currentFrame;
                m_redrawSubTexture = true;

                if (m_currentFrame == m_pCurrentAnimationState->second.size())
                    m_currentFrame = 0;
            }
        }
    }

} // namespace Renderer