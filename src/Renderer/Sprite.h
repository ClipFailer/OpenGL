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

	struct FrameDescription {
		FrameDescription(
			const glm::vec2 	_leftBottomUV,
			const glm::vec2 	_rightTopUV,
			const float  		_duration
		)
			: leftBottomUV(_leftBottomUV)
			, rightTopUV(_rightTopUV)
			, duration(_duration) {};

		glm::vec2 	leftBottomUV;
		glm::vec2 	rightTopUV;
		float 		duration;
	};

	class Sprite {
	public:
		Sprite(
			const std::string&						name,
			const std::shared_ptr<Texture2D> 		pTexture,
			const std::string&						initialSubTexture,
			const std::shared_ptr<ShaderProgram> 	pShaderProgram
		);
		
		~Sprite();

		Sprite(const Sprite&) 				= delete;
		Sprite& operator=(const Sprite&) 	= delete;

		void render(
			const glm::vec2& 	position,
			const glm::vec2& 	size,
			const float 		rotation,
			const size_t		frameId		= 0) const;
		
		void insertFrames(std::vector<FrameDescription>&& framesDescriptions) noexcept;
		float getFrameTime(const size_t frameId) const noexcept;
		size_t getFramesCount() const noexcept;

	protected:
		std::shared_ptr<Texture2D> 		m_pTexture;
		std::shared_ptr<ShaderProgram> 	m_pShaderProgram;
		std::string						m_name;
		std::vector<FrameDescription> 	m_frameDescriptions;
		mutable size_t					m_lastFrameId 			= 0;

		VertexArray						m_VAO;
		VertexBuffer 					m_vertexVBO;
		VertexBuffer 					m_textureVBO;
		IndexBuffer 					m_IBO;
	};
}