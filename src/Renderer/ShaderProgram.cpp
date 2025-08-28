#include "ShaderProgram.h"

#include <iostream>
#include "Tools/Tools.h"

Renderer::ShaderProgram::ShaderProgram(const std::string &vertexShader, 
									  const std::string &fragmentShader) 
{
	GLuint vertexShaderID = 0;
	if(!CreateShader(vertexShader, GL_VERTEX_SHADER, vertexShaderID))
	{
		std::cerr << "SHADER ERROR. Vetex shader compile error: ";
		ASSERT(false);
		return;
	}

	GLuint fragmentShaderID = 0;
	if(!CreateShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentShaderID))
	{
		std::cerr << "SHADER ERROR. Fragment shader compile error: ";
		glDeleteShader(vertexShaderID);
		ASSERT(false);
		return;
	}
	
	m_ID = glCreateProgram();
	glAttachShader(m_ID, vertexShaderID);
	glAttachShader(m_ID, fragmentShaderID);
	glLinkProgram(m_ID);

	GLint success;
	glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
	if(!success)
	{
		GLchar infoLog[1024];
		glGetProgramInfoLog(m_ID, 1024, nullptr, infoLog);
		std::cerr << "SHADER ERROR. link-time error: " << infoLog << std::endl;
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
		ASSERT(false);
		return;
	}

	m_isCompiled = true;

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}

Renderer::ShaderProgram::~ShaderProgram() {
	glDeleteProgram(m_ID);
}

Renderer::ShaderProgram &Renderer::ShaderProgram::operator=(ShaderProgram&& other) {
    glDeleteProgram(m_ID);
	m_ID = other.m_ID;
	m_isCompiled = other.m_isCompiled;

	other.m_ID = 0;
	other.m_isCompiled = false;

	return *this;
}

Renderer::ShaderProgram::ShaderProgram(ShaderProgram &&other) {
	m_ID = other.m_ID;
	m_isCompiled = other.m_isCompiled;

	other.m_ID = 0;
	other.m_isCompiled = false;
}

bool Renderer::ShaderProgram::CreateShader(const std::string &source, const GLenum shaderType, GLuint &shaderID) {
    shaderID = glCreateShader(shaderType);
	const char* code = source.c_str();
	glShaderSource(shaderID, 1, &code, nullptr);
	glCompileShader(shaderID);

	GLint success;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[1024];
		glGetShaderInfoLog(shaderID, 1024, nullptr, infoLog);
		std::cerr << infoLog << std::endl;
		
		return false;
	}

	return true;
}

void Renderer::ShaderProgram::Use() {
	glUseProgram(m_ID);
}

void Renderer::ShaderProgram::SetInt(const std::string &name, const GLint value) 
{
	glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}
