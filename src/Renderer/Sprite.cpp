#include "Sprite.h"

#include <vector>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer.h"
#include "ShaderProgram.h"
#include "Texture2D.h"

#include "VertexArrayLayout.h"
;
namespace Renderer {
	Sprite::Sprite(
		const std::string&						name,
		const std::shared_ptr<Texture2D> 		pTexture, 
		const std::string& 						initialSubTexture,
		const std::shared_ptr<ShaderProgram> 	pShaderProgram
	) 
		: m_pTexture(std::move(pTexture))
		, m_pShaderProgram(std::move(pShaderProgram)) 
		, m_name(name)
	{
        auto subTexture = m_pTexture->GetSubTexture(initialSubTexture);

		// VBO - Verteces coords
		std::vector<GLfloat> verteces
		{
			//X   Y	
			0.f, 0.f,	
			0.f, 1.f,			
			1.f, 1.f, 	
			1.f, 0.f,
		};

		m_vertexVBO.init(
			verteces.data(), 
			verteces.size() * sizeof(GLfloat)
		);

		// VAO - VertexBuffe5r
		VertexArrayLayout vertexLayout;
		vertexLayout.addElement<float>(2, 0);
		m_VAO.addBuffer(m_vertexVBO, vertexLayout);

		// VBO - Texture coords
		std::vector<GLfloat> textureCoords {
			subTexture.leftBottomUV.x, subTexture.leftBottomUV.y,
			subTexture.leftBottomUV.x, subTexture.rightTopUV.y,
			subTexture.rightTopUV.x, subTexture.rightTopUV.y,
			subTexture.rightTopUV.x, subTexture.leftBottomUV.y,
		};

		m_textureVBO.init(
			textureCoords.data(), 
			textureCoords.size() * sizeof(GLfloat)
		);
		
		VertexArrayLayout textureLayout;
		textureLayout.addElement<float>(2, 0);
		m_VAO.addBuffer(m_textureVBO, textureLayout);
		
		// IBO
		std::vector<GLuint> indeces {
			0, 1, 2,
			2, 3, 0,
		};

		m_IBO.init(
			indeces.data(),
			indeces.size() * sizeof(GLuint)
		);

		m_VAO.unbind();
		m_IBO.unbind();
	}

    Sprite::~Sprite() 
	{
    }

    void Sprite::render(
		const glm::vec2& 	position,
		const glm::vec2& 	size,
		const float 		rotation,
		const size_t		frameId
	) const
	{
		if (frameId != m_lastFrameId) {
			const FrameDescription& currentFrameDescription = m_frameDescriptions[frameId];

			std::vector<GLfloat> textureCoords {
				currentFrameDescription.leftBottomUV.x, 	currentFrameDescription.leftBottomUV.y,
				currentFrameDescription.leftBottomUV.x, 	currentFrameDescription.rightTopUV.y,
				currentFrameDescription.rightTopUV.x,   	currentFrameDescription.rightTopUV.y,
				currentFrameDescription.rightTopUV.x,   	currentFrameDescription.leftBottomUV.y,
			};

			m_textureVBO.update(
				textureCoords.data(), 
				textureCoords.size() * sizeof(GLfloat)
			);

			m_lastFrameId = frameId;
		}

		m_pShaderProgram->Use();	
		
		glm::mat4 model(1.f);

		model = glm::translate(model, glm::vec3(position, 0.f));

		model = glm::translate(
			model, 
			glm::vec3(0.5f * size.x, 0.5f * size.y, 0.f));

		model = glm::rotate(
			model, 
			glm::radians(rotation), 
			glm::vec3(0.f, 0.f, 1.f));

		model = glm::translate(
			model, 
			glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.f)
		);
		
		model = glm::scale(
			model, 
			glm::vec3(size, 1.f)
		);

		m_pShaderProgram->SetMat4("modelMat", model);

		glActiveTexture(GL_TEXTURE0);
		m_pTexture->Bind();

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		Renderer::Draw(m_VAO, m_IBO, *m_pShaderProgram);
    }

    void Sprite::insertFrames(std::vector<FrameDescription>&& framesDescriptions) noexcept {
		m_frameDescriptions = std::move(framesDescriptions);
    }

    float Sprite::getFrameTime(const size_t frameId) const noexcept {
        return m_frameDescriptions[frameId].duration;
    }

    size_t Sprite::getFramesCount() const noexcept {
        return m_frameDescriptions.size();
    }

    
} // namespace Renderer