#pragma once

#include <glad/gl.h>
#include <string>

namespace Renderer {
	class Texture2D {
	public:
		Texture2D(const std::string& name, const GLuint width, const GLuint height, 
				  unsigned char* data, const unsigned int channels = 4, 
				  const GLenum filter = GL_LINEAR, const GLenum wrapMode = GL_CLAMP_TO_EDGE);
		~Texture2D();
		Texture2D() = delete;
		Texture2D(const Texture2D& other) = delete;
		Texture2D& operator=(const Texture2D& other) = delete;
		Texture2D(Texture2D&& other);
		Texture2D& operator=(Texture2D&& other);

		void Bind() const;
		std::string GetName() const;

	private:
		GLuint m_ID;
		GLenum m_channelMode;
		unsigned int m_width;
		unsigned int m_height;
		const std::string m_name;
	};
}