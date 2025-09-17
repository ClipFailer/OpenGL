#include "ResourceManager.h"

#include "Renderer/ShaderProgram.h"
#include "Renderer/Texture2D.h"
#include "Renderer/Sprite.h"
#include "Renderer/AnimatedSprite.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"

#include <sstream>
#include <fstream>
#include <iostream>

#include <string>

ShaderProgramsMap ResourceManager::m_shaderPrograms;
TexturesMap ResourceManager::m_textures;
SpritesMap ResourceManager::m_sprites;
AnimatedSpritesMap ResourceManager::m_animatedSprites;
std::string ResourceManager::m_path;

void ResourceManager::SetPath(const std::string &resourcePath) {
	size_t found = resourcePath.find_last_of("/\\");
	m_path = resourcePath.substr(0, found);
}

void ResourceManager::UnloadAllResources() {
	m_animatedSprites.clear();
	m_shaderPrograms.clear();
	m_sprites.clear();
	m_textures.clear();
}

ShaderPtr ResourceManager::LoadShaders(
	const std::string& shaderName, 
	const std::string& vertexPath, 
	const std::string& fragmentPath
) {
    std::string vertexString = GetFileString(vertexPath);	
	if (vertexString.empty())
	{
		std::cerr << "Failed to load vertex shader!\n";
		return nullptr;
	}

    std::string fragmentString = GetFileString(fragmentPath);
	if (fragmentString.empty())
	{
		std::cerr << "Failed to load fragment shader!\n";
		return nullptr;
	}

	auto newShader = m_shaderPrograms.emplace(
		shaderName, 
		std::make_shared<Renderer::ShaderProgram>(
			shaderName, 
			vertexString, 
			fragmentString
		)
	).first->second;
	if (newShader->IsCompiled())
	{
		return newShader;
	}

	std::cerr << "Can't load shader: \n" 
		<< "Vertex: " << vertexPath << "\n"
		<< "Fragment: " << fragmentPath << std::endl;

	return nullptr;
}

ShaderPtr ResourceManager::GetShaderProgram(const std::string &shaderName) {
    ShaderProgramsMap::const_iterator it = m_shaderPrograms.find(shaderName);

	if (it != m_shaderPrograms.end())
	{
		return it->second;
	}
	
	std::cerr << "Can't find the shader program: " << shaderName << std::endl;
	return nullptr;
}

TexturePtr ResourceManager::LoadTexture(
	const std::string& textureName,
	const std::string& texturePath
) {
	int channels 	= 0;
	int width 		= 0;
	int height 		= 0;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* pixels = stbi_load(
		std::string(m_path + "/" + texturePath).c_str(), 
		&width, 
		&height, 
		&channels, 
		0
	);
	
	if(!pixels) {
		std::cerr << "Can't load texture: " << textureName << std::endl;
		return nullptr;
	}

	std::shared_ptr<Renderer::Texture2D> newTexture = m_textures.emplace(
		textureName, 
		std::make_shared<Renderer::Texture2D>(
			textureName, width, height, 					  
			pixels, channels, 
			GL_NEAREST, GL_CLAMP_TO_EDGE
		)
	).first->second;

	stbi_image_free(pixels);

	return newTexture;
}

TexturePtr ResourceManager::GetTexture(const std::string &textureName) {
    TexturesMap::const_iterator it = m_textures.find(textureName);
	if (it != m_textures.end())
	{
		return it->second;
	}

	std::cerr << "Can't find texture: " << textureName << std::endl;
	return nullptr;
}

SpritePtr ResourceManager::LoadSprite(
	const std::string &spriteName, 
	const std::string &textureName, 
	const std::string &shaderName, 
	const unsigned int spriteWidth, 
	const unsigned int spriteHeight,
	const std::string& initialSubTexureName
) {
    auto pTexture = GetTexture(textureName);
	if (!pTexture)
		return nullptr;

	auto pShader = GetShaderProgram(shaderName);
	if (!pShader)
		return nullptr;

	std::shared_ptr<Renderer::Sprite> newSprite = m_sprites.emplace(
		spriteName,
		std::make_shared<Renderer::Sprite>(
			spriteName,
			pTexture, 
			initialSubTexureName,
			pShader, 
			glm::vec2(0.f, 0.f),
			glm::vec2(spriteWidth, spriteHeight)
		)
	).first->second;

	return newSprite;
}

SpritePtr ResourceManager::GetSprite(const std::string &spriteName) {
    auto it = m_sprites.find(spriteName);
	if (it != m_sprites.end())
		return it->second;

	std::cerr << "Can't find sprite: " << spriteName << std::endl;
	return nullptr;
}

AnimatedSpritePtr ResourceManager::LoadAnimatedSprite(
	const std::string&	spriteName, 
	const std::string&	textureName, 
	const std::string&	shaderName, 
	const unsigned int 	spriteWidth, 
	const unsigned int 	spriteHeight,
	const std::string&	initialSubTexureName
) {
    auto texture = GetTexture(textureName);
	if (!texture) {
		std::cerr << "Can't find texture: " << textureName << std::endl;
		return nullptr;
	}

	auto shader = GetShaderProgram(shaderName);
	if (!shader) {
		std::cerr << "Can't find shader: " << shaderName << std::endl;
		return nullptr;
	}

	auto newAnimatedSprite = m_animatedSprites.emplace(
		spriteName, 
		std::make_shared<Renderer::AnimatedSprite>(
			spriteName,
			texture,
			initialSubTexureName,
			shader,
			glm::vec2(0.f, 0.f),
			glm::vec2(spriteWidth, spriteHeight)
		)
	).first->second;

	return newAnimatedSprite;
}

AnimatedSpritePtr ResourceManager::GetAnimatedSprite(
	const std::string &spriteName
) {
    auto it = m_animatedSprites.find(spriteName);

	if (it != m_animatedSprites.end())
		return it->second;

	std::cerr << "Can't find sprite: " << spriteName << std::endl;
	return nullptr;
}
TexturePtr ResourceManager::LoadTextureAtlas(
	const std::string& 				textureName, 
	const std::string& 				texturePath, 
	const std::vector<std::string> 	subTextures,
	const unsigned int 				subTextureWidth,
	const unsigned int 				subTextureHeight
) {
    auto pTexture = LoadTexture(textureName, texturePath);

	if (pTexture) {
		const unsigned int textureWidth = pTexture->GetWidth();
		const unsigned int textureHeight= pTexture->GetHeight();
		unsigned int currentTextureOffsetX = 0;
		unsigned int currentTextureOffsetY = textureHeight;

		for (const auto &currentSubTextureName : subTextures) {
			glm::vec2 leftBottonUV(static_cast<float>(currentTextureOffsetX) / textureWidth,
								   static_cast<float>(currentTextureOffsetY - subTextureHeight) / textureHeight);
			glm::vec2 rightTopUV(static_cast<float>(currentTextureOffsetX + subTextureWidth) / textureWidth,
								 static_cast<float>(currentTextureOffsetY) / textureHeight);

			pTexture->AddSubTexture(std::move(currentSubTextureName), leftBottonUV, rightTopUV);

			currentTextureOffsetX += subTextureWidth;
			if (currentTextureOffsetX >= textureWidth) {
				currentTextureOffsetX = 0;
				currentTextureOffsetY -= subTextureHeight;
			}
		}
	}

	return pTexture;
}

std::string ResourceManager::GetFileString(const std::string &relativeFilePath){
	std::fstream file;
	file.open(m_path + "/" + relativeFilePath, std::ios::in | std::ios::binary);

	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << relativeFilePath << std::endl;
		return std::string{};
	}

	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}
