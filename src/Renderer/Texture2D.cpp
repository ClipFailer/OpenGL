#include "Texture2D.h"

namespace Renderer {
	Texture2D::Texture2D(const GLuint width, const GLuint height, 
						 unsigned char *data, const unsigned int channels, 
						 const GLenum filter, const GLenum wrapMode)
						: m_width(width), m_height(height)
	{
		switch (channels)
		{
		case 4:
			m_channelMode = GL_RGBA;
			break;
		case 3:
			m_channelMode = GL_RGB;
			break;
		default:
			m_channelMode = GL_RGBA;
			break;
		}

		glGenTextures(1, &m_ID);
		glBindTexture(GL_TEXTURE_2D, m_ID);

		glTexImage2D(GL_TEXTURE_2D, 0, m_channelMode, 
					 m_width, m_height, 
					 0, m_channelMode, 
					 GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	Texture2D::~Texture2D() 
	{
		glDeleteTextures(1, &m_ID);
	}

	Texture2D::Texture2D(Texture2D &&other) 
	{
		m_ID = other.m_ID;
		other.m_ID = 0;

		m_channelMode = other.m_channelMode;
		m_width = other.m_width;
		m_height = other.m_height;
	}

	Texture2D &Renderer::Texture2D::operator=(Texture2D &&other) 
	{
		glDeleteTextures(1, &m_ID);
		m_ID = other.m_ID;
		other.m_ID = 0;

		m_channelMode = other.m_channelMode;
		m_width = other.m_width;
		m_height = other.m_height;

		return *this;
	}

	void Texture2D::Bind() const 
	{
		glBindTexture(GL_TEXTURE_2D, m_ID);
	}
}
