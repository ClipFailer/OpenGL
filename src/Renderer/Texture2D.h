#pragma once

#include <glad/gl.h>
#include <string>
#include <map>

#include <glm/vec2.hpp>

namespace Renderer {
	class Texture2D {
	public:
		struct SubTexture2D {
			glm::vec2 leftBottomUV;
			glm::vec2 rightTopUV;

			SubTexture2D(
				const glm::vec2 &_leftBottonUV, 
				const glm::vec2 &_rightTopUV
			) 
				: leftBottomUV(_leftBottonUV)
				, rightTopUV(_rightTopUV) {};
				
			SubTexture2D() 
			: leftBottomUV(0.f)
			, rightTopUV(1.f) {};
		};

		Texture2D(
			const std::string&		name, 
			const GLuint 			width, 
			const GLuint 			height, 
			unsigned char* 			data, 
			const unsigned int 		channels 	= 4, 
			const GLenum 			filter 		= GL_LINEAR, 
			const GLenum			wrapMode 	= GL_CLAMP_TO_EDGE
		);
		Texture2D() 									= delete;
		~Texture2D();

		Texture2D(const Texture2D& other) 				= delete;
		Texture2D& operator=(const Texture2D& other) 	= delete;

		Texture2D(Texture2D&& other);
		Texture2D& operator=(Texture2D&& other);

		void Bind() const;
		std::string GetName() const;

		void AddSubTexture(
			std::string 		name, 
			const glm::vec2&	rightBottomUV,
			const glm::vec2&	leftTopUV
		);
		
		const SubTexture2D& GetSubTexture(const std::string& name) const;

		unsigned int GetWidth() const;
		unsigned int GetHeight() const;

	private:
		GLuint 									m_ID;
		GLenum 									m_channelMode;
		unsigned int 							m_width;
		unsigned int 							m_height;
		const std::string 						m_name;
		std::map<std::string, SubTexture2D> 	m_subtextures;
	};
}