#pragma once

#include <string>
#include <memory>
#include <map>
#include <vector>

namespace Renderer {
	class ShaderProgram;
	class Texture2D;
	class Sprite;
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

	std::shared_ptr<Renderer::Sprite> LoadSprite(const std::string& spriteName,
												 const std::string& textureName,
												 const std::string& shaderName,
												 const unsigned int spriteWidth,
												 const unsigned int spriteHeight,
												const std::string& initialSubTexureName = "defaultSubTexture");
	std::shared_ptr<Renderer::Sprite> GetSprite(const std::string &spriteName);

	std::shared_ptr<Renderer::Texture2D> LoadTextureAtlas(const std::string &textureName,
														  const std::string &texturePath,
														  const std::vector<std::string> subTextures,
														  const unsigned int subTextureWidth,
														  const unsigned int subTextureHeight);

private:
	std::string GetFileString(const std::string& filePath) const;

	typedef std::map<std::string, std::shared_ptr<Renderer::ShaderProgram>> ShaderProgramsMap;
	ShaderProgramsMap m_shaderPrograms;

	typedef std::map<std::string, std::shared_ptr<Renderer::Texture2D>> TexturesMap;
	TexturesMap m_textures;

	typedef std::map<std::string, std::shared_ptr<Renderer::Sprite>> SpritesMap;
	SpritesMap m_sprites;

	std::string m_path;
};