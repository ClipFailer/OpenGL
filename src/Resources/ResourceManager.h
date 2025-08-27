#pragma once

#include <string>
#include <memory>
#include <map>

namespace Renderer {
	class ShaderProgram;
	class Texture2D;
}

class ResourceManager {
public:
	ResourceManager(const std::string& resourcePath);
	~ResourceManager() = default;

	ResourceManager(const ResourceManager& ) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager(ResourceManager&&) = delete;

	std::shared_ptr<Renderer::ShaderProgram> LoadShaders(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath); 
	std::shared_ptr<Renderer::ShaderProgram> GetShaderProgram(const std::string& shaderName) const;

	std::shared_ptr<Renderer::Texture2D> loadTexture(const std::string &textureName, const std::string &texturePath);
	std::shared_ptr<Renderer::Texture2D> getTexture(const std::string& textureName) const;

private:
	std::string GetFileString(const std::string& filePath) const;

	typedef std::map<std::string, std::shared_ptr<Renderer::ShaderProgram>> ShaderProgramsMap;
	ShaderProgramsMap m_shaderPrograms;

	typedef std::map<std::string, std::shared_ptr<Renderer::Texture2D>> TexturesMap;
	TexturesMap m_textures;

	std::string m_path;
};