#pragma once

#include <string>
#include <glad/gl.h>

#include <glm/mat4x4.hpp>

namespace Renderer {
	class ShaderProgram {
	public:
		ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
		~ShaderProgram();

		ShaderProgram() = delete;
		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram& operator=(const ShaderProgram) = delete;

		ShaderProgram& operator=(ShaderProgram&& other);
		ShaderProgram(ShaderProgram&& other);

		bool CreateShader(const std::string& source, const GLenum shaderType, GLuint& shaderID);
		bool IsCompiled() { return m_isCompiled; }
		void Use();
		void SetInt(const std::string& name, const GLint value);
		void SetMat4(const std::string& name, const glm::mat4 matrix);

	private:
		bool m_isCompiled = false;
		GLuint m_ID = 0;
	};
}