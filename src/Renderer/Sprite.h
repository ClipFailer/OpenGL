#pragma once

#include <glad/gl.h>

#include <memory>
#include <glm/vec2.hpp>
#include <string>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

namespace Renderer {

	class Texture2D;
	class ShaderProgram;

	class Sprite {
	public:
		Sprite(const std::string&						name,
			   const std::shared_ptr<Texture2D> 		pTexture,
			   const std::string&						initialSubTexture,
			   const std::shared_ptr<ShaderProgram> 	pShaderProgram
		);
		
		~Sprite();

		Sprite(const Sprite&) 				= delete;
		Sprite& operator=(const Sprite&) 	= delete;

		virtual void render(
			const glm::vec2& 	position,
			const glm::vec2& 	size,
			const float 		rotation) const;

	protected:
		std::shared_ptr<Texture2D> 		m_pTexture;
		std::shared_ptr<ShaderProgram> 	m_pShaderProgram;
		std::string						m_name;

		VertexArray						m_VAO;
		VertexBuffer 					m_vertexVBO;
		VertexBuffer 					m_textureVBO;
		IndexBuffer 					m_IBO;
	};
}