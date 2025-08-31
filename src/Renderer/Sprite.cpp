#include "Sprite.h"

#include "Renderer/ShaderProgram.h"
#include "Renderer/Texture2D.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Renderer {
	Sprite::Sprite(const std::string &name,
			   	   const std::shared_ptr<Texture2D> pTexture, 
			   	   const std::shared_ptr<ShaderProgram> pShaderProgram,
			       const glm::vec2& position,
			       const glm::vec2& size,
			       const float rotation) 
				: m_pTexture(std::move(pTexture)), 
				  m_pShaderProgram(std::move(pShaderProgram)),
				  m_position(position),
     			  m_size(size),
      			  m_rotation(rotation),
				  m_name(name)
	{
		const GLfloat verteces[]
		{
			//Verteces	Texture coords
			//X   Y		//U	  V
			0.f, 0.f,	0.f, 0.f,
			0.f, 1.f,	0.f, 1.f,
			1.f, 1.f, 	1.f, 1.f,
			1.f, 0.f,	1.f, 0.f,
		};

		const GLuint indeces[]
		{
			0, 1, 2,
			2, 3, 0,
		};

		// VAO
		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		// VBO
		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(verteces), verteces, GL_STATIC_DRAW);
		
		// VBO - Verteces coords
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, 
							  GL_FLOAT, GL_FALSE, 
							  4 * sizeof(GL_FLOAT), (void*)0);

		// VBO - Texture coords
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, 
							  GL_FLOAT, GL_FALSE, 
							  4 * sizeof(GL_FLOAT), (void*)(2 * sizeof(GL_FLOAT)));
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// IBO
		glGenBuffers(1, &m_IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces), indeces, GL_STATIC_DRAW);

		glBindVertexArray(0);
	}

    Sprite::~Sprite() 
	{
		glDeleteBuffers(1, &m_VBO);
		glDeleteBuffers(1, &m_IBO);
		glDeleteVertexArrays(1, &m_VAO);
    }

    void Sprite::Render() const
	{
		m_pShaderProgram->Use();	
		
		glm::mat4 model(1.f);

		model = glm::translate(model, glm::vec3(m_position, 0.f));
		model = glm::translate(model, glm::vec3(0.5f * m_size.x, 0.5f * m_size.y, 0.f));
		model = glm::rotate(model, glm::radians(m_rotation), glm::vec3(0.f, 0.f, 1.f));
		model = glm::translate(model, glm::vec3(-0.5f * m_size.x, -0.5f * m_size.y, 0.f));
		model = glm::scale(model, glm::vec3(m_size, 1.f));

		glBindVertexArray(m_VAO);
		m_pShaderProgram->SetMat4("modelMat", model);

		glActiveTexture(GL_TEXTURE0);
		m_pTexture->Bind();

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    void Sprite::SetPosition(const glm::vec2 &newPosition) 
	{
		m_position = newPosition;
    }

    void Sprite::SetSize(const glm::vec2 &newSize) 
	{
		m_size = newSize;
    }

    void Sprite::SetRotation(const float newRotation) 
	{
		m_rotation = newRotation;
    }
} // namespace Renderer