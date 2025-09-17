#include "Texture2D.h"

#include <iostream>

namespace Renderer {
	Texture2D::Texture2D(
		const std::string& 	name, 
		const GLuint 		width, 
		const GLuint 		height, 
		unsigned char*		data, 
		const unsigned int 	channels, 
		const GLenum 		filter, 
		const GLenum 		wrapMode
	)
		: m_name(name)
		, m_width(width)
		, m_height(height) 
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
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_ID);

		glTexImage2D(
			GL_TEXTURE_2D, 
			0, 
			m_channelMode, 			 
			m_width, 
			m_height, 		
			0, 
			m_channelMode, 	 
			GL_UNSIGNED_BYTE, 
			data
		);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 		wrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 		wrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 	filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, 	filter);

		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	Texture2D::~Texture2D() {
		glDeleteTextures(1, &m_ID);
	}

	Texture2D::Texture2D(Texture2D &&other) {
		m_ID 		= other.m_ID;
		other.m_ID 	= 0;

		m_channelMode 	= other.m_channelMode;
		m_width 		= other.m_width;
		m_height 		= other.m_height;
	}

	Texture2D &Renderer::Texture2D::operator=(Texture2D &&other) {
		glDeleteTextures(1, &m_ID);
		m_ID 		= other.m_ID;
		other.m_ID 	= 0;

		m_channelMode 	= other.m_channelMode;
		m_width 		= other.m_width;
		m_height 		= other.m_height;

		return *this;
	}

	void Texture2D::Bind() const {
		glBindTexture(GL_TEXTURE_2D, m_ID);
	}

    std::string Texture2D::GetName() const {
        return m_name;
    }

    void Texture2D::AddSubTexture(
		std::string name, 
		const glm::vec2 &rightBottomUV, 
		const glm::vec2 &leftTopUV
	) {
		m_subtextures.emplace(std::move(name), 
							  SubTexture2D(rightBottomUV, leftTopUV));
    }

    const Texture2D::SubTexture2D& Texture2D::GetSubTexture(
		const std::string& name
	) const {
        auto it = m_subtextures.find(name);

		if (it != m_subtextures.end())
			return it->second;

		std::cerr << "Can't find subtexture: " << name << std::endl;
		
		const static SubTexture2D defaultSubTexture;
		return defaultSubTexture;
    }

    unsigned int Texture2D::GetWidth() const {
        return m_width;
    }

    unsigned int Texture2D::GetHeight() const {
        return m_height;
    }

} // namespace Renderer
