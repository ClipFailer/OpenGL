#pragma once

#include <string>
#include <glad/gl.h>

#include <glm/mat4x4.hpp>

namespace Renderer {
	class ShaderProgram {
	public:
		ShaderProgram(const std::string& name, 
					  const std::string& vertexShader, 
					  const std::string& fragmentShader);
		~ShaderProgram();

		ShaderProgram() 								= delete;
		ShaderProgram(const ShaderProgram&) 			= delete;
		ShaderProgram& operator=(const ShaderProgram&) 	= delete;

		ShaderProgram& operator=(ShaderProgram&& other)	noexcept;
		ShaderProgram(ShaderProgram&& other)			noexcept;

		bool CreateShader(
			const std::string& 	source, 
			const GLenum 		shaderType, 
			GLuint& 			shaderID
		);
		bool IsCompiled() noexcept { return m_isCompiled; }
		void Use() const noexcept;
		void SetInt(
			const std::string& 	name, 
			const GLint 		value
		);
		void SetMat4(
			const std::string& 	name, 
			const glm::mat4 	&matrix
		);
		std::string GetName() const noexcept;

	private:
		bool 			m_isCompiled 	= false;
		GLuint 			m_ID 			= 0;
		std::string 	m_name;
	};
}