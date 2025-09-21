#pragma once

#include <string>
#include <memory>
#include <map>
#include <vector>

namespace Renderer {
	class ShaderProgram;
	class Texture2D;
	class Sprite;
	class AnimatedSprite;
}

using ShaderPtr = std::shared_ptr<Renderer::ShaderProgram>;
using TexturePtr = std::shared_ptr<Renderer::Texture2D>;
using SpritePtr = std::shared_ptr<Renderer::Sprite>;
using AnimatedSpritePtr = std::shared_ptr<Renderer::AnimatedSprite>;

using ShaderProgramsMap = std::map<std::string, std::shared_ptr<Renderer::ShaderProgram>>;
using TexturesMap = std::map<std::string, std::shared_ptr<Renderer::Texture2D>>;
using SpritesMap = std::map<std::string, std::shared_ptr<Renderer::Sprite>>;
using AnimatedSpritesMap = std::map<std::string, std::shared_ptr<Renderer::AnimatedSprite>>;
using LevelVector = std::vector<std::vector<std::string>>;

class ResourceManager {
public:
	static void setPath(const std::string& resourcePath);
	static void UnloadAllResources();

	ResourceManager() 									= delete;
	~ResourceManager() 									= delete;

	ResourceManager(const ResourceManager& ) 			= delete;
	ResourceManager& operator=(const ResourceManager&) 	= delete;
	ResourceManager(ResourceManager&&) 					= delete;

	static ShaderPtr loadShaders(
		const std::string& 	shaderName, 
		const std::string& 	vertexPath, 
		const std::string& 	fragmentPath
	); 
	static ShaderPtr getShaderProgram(const std::string& shaderName);

	static TexturePtr loadTexture(
		const std::string& textureName, 
		const std::string& texturePath
	);
	static TexturePtr getTexture(const std::string& textureName);

	static SpritePtr loadSprite(
		const std::string& spriteName,
		const std::string& textureName,
		const std::string& shaderName,
		const std::string& initialSubTexureName = "defaultSubTexture"
	);
	static SpritePtr getSprite(const std::string &spriteName);

	static TexturePtr loadTextureAtlas(
		const std::string 					&textureName,
		const std::string 					&texturePath,
		const std::vector<std::string> 		subTextures,
		const unsigned int 					subTextureWidth,
		const unsigned int 					subTextureHeight
	);

	static const LevelVector& getLevels() noexcept { return m_levels; };

	static bool loadJsonResources(const std::string& filePath);
		
private:
	static std::string getFileString(const std::string& filePath);

	static std::string 			m_path;

	static LevelVector 			m_levels;
	static ShaderProgramsMap 	m_shaderPrograms;
	static TexturesMap 			m_textures;
	static SpritesMap 			m_sprites;
};