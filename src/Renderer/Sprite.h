#pragma once

#include <glad/gl.h>

#include <memory>
#include <glm/vec2.hpp>
#include <string>

namespace Renderer {

	class Texture2D;
	class ShaderProgram;

	class Sprite {
	public:
		Sprite(const std::string &name,
			   const std::shared_ptr<Texture2D> pTexture, 
			   const std::shared_ptr<ShaderProgram> pShaderProgram,
			   const glm::vec2& position = glm::vec2(0.f),
			   const glm::vec2& size = glm::vec2(1.f),
			   const float rotation = 0.f);
		
		~Sprite();

		Sprite(const Sprite&) = delete;
		Sprite& operator=(const Sprite&) = delete;

		void Render() const;
		void SetPosition(const glm::vec2& newPosition);
		void SetSize(const glm::vec2& newSize);
		void SetRotation(const float newRotation);

	private:
		std::shared_ptr<Texture2D> m_pTexture;
		std::shared_ptr<ShaderProgram> m_pShaderProgram;
		glm::vec2 m_position;
		glm::vec2 m_size;
		float m_rotation;
		GLuint m_VAO = 0;
		GLuint m_VBO = 0;
		GLuint m_IBO = 0;
		std::string m_name;
	};
}