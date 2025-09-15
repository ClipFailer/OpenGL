#pragma once

#include <string>
#include <memory>
#include <map>
#include <vector>

using ShaderPtr = std::shared_ptr<Renderer::ShaderProgram>;
using TexturePtr = std::shared_ptr<Renderer::Texture2D>;
using SpritePtr = std::shared_ptr<Renderer::Sprite>;
using AnimatedSpritePtr = std::shared_ptr<Renderer::AnimatedSprite>;

using ShaderProgramsMap = std::map<std::string, std::shared_ptr<Renderer::ShaderProgram>>;
using TexturesMap = std::map<std::string, std::shared_ptr<Renderer::Texture2D>>;
using SpritesMap = std::map<std::string, std::shared_ptr<Renderer::Sprite>>;
using AnimatedSpritesMap = std::map<std::string, std::shared_ptr<Renderer::AnimatedSprite>>;

namespace Renderer {
	class ShaderProgram;
	class Texture2D;
	class Sprite;
	class AnimatedSprite;
}

class ResourceManager {
public:
	static void SetPath(const std::string& resourcePath);
	static void UnloadAllResources();

	ResourceManager() = delete;
	~ResourceManager() = delete;

	ResourceManager(const ResourceManager& ) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager(ResourceManager&&) = delete;

	static ShaderPtr LoadShaders(const std::string& 	shaderName, 
								 const std::string& 	vertexPath, 
								 const std::string& 	fragmentPath); 
	static ShaderPtr GetShaderProgram(const std::string& shaderName);

	static TexturePtr LoadTexture(const std::string &textureName, 
								  const std::string &texturePath);
	static TexturePtr GetTexture(const std::string& textureName);

	static SpritePtr LoadSprite(
		const std::string& spriteName,
		const std::string& textureName,
		const std::string& shaderName,
		const unsigned int spriteWidth,
		const unsigned int spriteHeight,
		const std::string& initialSubTexureName = "defaultSubTexture"
	);
	static SpritePtr GetSprite(const std::string &spriteName);

	static AnimatedSpritePtr LoadAnimatedSprite(
		const std::string& spriteName,
		const std::string& textureName,
		const std::string& shaderName,
		const unsigned int spriteWidth,
		const unsigned int spriteHeight,
		const std::string& initialSubTexureName = "defaultSubTexture"
	);
	static AnimatedSpritePtr GetAnimatedSprite(const std::string &spriteName);

	static TexturePtr LoadTextureAtlas(
		const std::string 					&textureName,
		const std::string 					&texturePath,
		const std::vector<std::string> 		subTextures,
		const unsigned int 					subTextureWidth,
		const unsigned int 					subTextureHeight
	);
		
private:
	static std::string GetFileString(const std::string& filePath);

	static ShaderProgramsMap 	m_shaderPrograms;
	static TexturesMap 			m_textures;
	static SpritesMap 			m_sprites;
	static AnimatedSpritesMap 	m_animatedSprites;
	static std::string 			m_path;
};