#include "ResourceManager.h"

#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

#include "Renderer/ShaderProgram.h"
#include "Renderer/Texture2D.h"
#include "Renderer/Sprite.h"

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
std::string ResourceManager::m_path;
LevelVector ResourceManager::m_levels;

void ResourceManager::setPath(const std::string &resourcePath) {
	size_t found = resourcePath.find_last_of("/\\");
	m_path = resourcePath.substr(0, found);
}

void ResourceManager::UnloadAllResources() {
	m_shaderPrograms.clear();
	m_sprites.clear();
	m_textures.clear();
}

ShaderPtr ResourceManager::loadShaders(
	const std::string& shaderName, 
	const std::string& vertexPath, 
	const std::string& fragmentPath
) {
    std::string vertexString = getFileString(vertexPath);	
	if (vertexString.empty())
	{
		std::cerr << "Failed to load vertex shader!\n";
		return nullptr;
	}

    std::string fragmentString = getFileString(fragmentPath);
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

ShaderPtr ResourceManager::getShaderProgram(const std::string &shaderName) {
    ShaderProgramsMap::const_iterator it = m_shaderPrograms.find(shaderName);

	if (it != m_shaderPrograms.end())
	{
		return it->second;
	}
	
	std::cerr << "Can't find the shader program: " << shaderName << std::endl;
	return nullptr;
}

TexturePtr ResourceManager::loadTexture(
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
			textureName, 
			width, 
			height, 					  
			pixels, 
			channels, 
			GL_NEAREST, 
			GL_CLAMP_TO_EDGE
		)
	).first->second;

	stbi_image_free(pixels);

	return newTexture;
}

TexturePtr ResourceManager::getTexture(const std::string &textureName) {
    TexturesMap::const_iterator it = m_textures.find(textureName);
	if (it != m_textures.end())
	{
		return it->second;
	}

	std::cerr << "Can't find texture: " << textureName << std::endl;
	return nullptr;
}

SpritePtr ResourceManager::loadSprite(
	const std::string& spriteName, 
	const std::string& textureName, 
	const std::string& shaderName, 
	const std::string& initialSubTexureName
) {
    auto pTexture = getTexture(textureName);
	if (!pTexture)
		return nullptr;

	auto pShader = getShaderProgram(shaderName);
	if (!pShader)
		return nullptr;

	std::shared_ptr<Renderer::Sprite> newSprite = m_sprites.emplace(
		spriteName,
		std::make_shared<Renderer::Sprite>(
			spriteName,
			pTexture, 
			initialSubTexureName,
			pShader
		)
	).first->second;

	return newSprite;
}

SpritePtr ResourceManager::getSprite(const std::string &spriteName) {
    auto it = m_sprites.find(spriteName);
	if (it != m_sprites.end())
		return it->second;

	std::cerr << "Can't find sprite: " << spriteName << std::endl;
	return nullptr;
}

TexturePtr ResourceManager::loadTextureAtlas(
	const std::string& 				textureName, 
	const std::string& 				texturePath, 
	const std::vector<std::string> 	subTextures,
	const unsigned int 				subTextureWidth,
	const unsigned int 				subTextureHeight
) {
    auto pTexture = loadTexture(textureName, texturePath);

	if (pTexture) {
		const unsigned int textureWidth = pTexture->GetWidth();
		const unsigned int textureHeight= pTexture->GetHeight();
		unsigned int currentTextureOffsetX = 0;
		unsigned int currentTextureOffsetY = textureHeight;

		for (const auto &currentSubTextureName : subTextures) {
			glm::vec2 leftBottonUV(
				static_cast<float>(currentTextureOffsetX + 0.01f) / textureWidth,
				static_cast<float>(currentTextureOffsetY - subTextureHeight + 0.01f) / textureHeight
			);
			glm::vec2 rightTopUV(
				static_cast<float>(currentTextureOffsetX + subTextureWidth - 0.01f) / textureWidth,
				static_cast<float>(currentTextureOffsetY - 0.01f) / textureHeight
			);

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

bool ResourceManager::loadJsonResources(const std::string &filePath) {
    const std::string jsonString = getFileString(filePath);

	if (jsonString.empty()) {
		std::cerr << "Can't find json file" << std::endl;
		return false;
	}

	rapidjson::Document document;
	rapidjson::ParseResult parseResult = document.Parse(jsonString.c_str());	
	if (!parseResult) {
		std::cerr << "JSON parse error: " 
			<< rapidjson::GetParseError_En(parseResult.Code())
			<< " (" << parseResult.Offset() << ") in file: " 
			<< filePath << std::endl;
	}

	auto shadersIt = document.FindMember("shaders");
	if (shadersIt != document.MemberEnd()) {
		for (const auto& currentShader : shadersIt->value.GetArray()) {
			const std::string name = currentShader["name"].GetString();
			const std::string filePathV = currentShader["filePath_v"].GetString();
			const std::string filePathF = currentShader["filePath_f"].GetString();
			loadShaders(name, filePathV, filePathF);
		}
	}

	auto textureAtlasIt = document.FindMember("textureAtlases");
	if (textureAtlasIt != document.MemberEnd()) {
		for (const auto& currentTextureAtlas : textureAtlasIt->value.GetArray()) {
			const std::string name = currentTextureAtlas["name"].GetString();
			const std::string filePath = currentTextureAtlas["filePath"].GetString();
			const unsigned int subTextureWidth = currentTextureAtlas["subTextureWidth"].GetUint();
			const unsigned int subTextureHeight = currentTextureAtlas["subTextureHeight"].GetUint();

			const auto subTexturesArray = currentTextureAtlas["subTextures"].GetArray();
			std::vector<std::string> subTextures;
			subTextures.reserve(subTexturesArray.Size());
			for (const auto& currentSubTexture : subTexturesArray) {
				subTextures.emplace_back(currentSubTexture.GetString());
			}

			loadTextureAtlas(
				name,
				filePath,
				std::move(subTextures),
				subTextureWidth,
				subTextureHeight
			);
		}
	}

	auto spritesIt = document.FindMember("sprites");
	if (spritesIt != document.MemberEnd()) {
		for (const auto& currentSprite : spritesIt->value.GetArray()) {
			const std::string name = currentSprite["name"].GetString();
			const std::string textureAtlas = currentSprite["textureAtlas"].GetString();
			const std::string shader = currentSprite["shader"].GetString();
			const std::string subTexture = currentSprite["initialTexture"].GetString();

			auto pSprite = loadSprite(
				name,
				textureAtlas,
				shader,
				subTexture
			);
			if (!pSprite) {
				std::cerr << "Failed to load sprite: " << name << std::endl;
				continue;
			}

			auto framesIt = currentSprite.FindMember("frames");
			if (framesIt != currentSprite.MemberEnd()) {
				auto framesArray = framesIt->value.GetArray();
				std::vector<Renderer::FrameDescription> framesDescriptions;
				framesDescriptions.reserve(framesArray.Size());
				for (const auto& currentFrame : framesArray) {
					const std::string subTexture = currentFrame["subTexture"].GetString();
					const auto pTextureAtlas = getTexture(textureAtlas);

					const auto pSubTexture = pTextureAtlas->GetSubTexture(subTexture);
					const float duration = currentFrame["duration"].GetFloat();

					framesDescriptions.emplace_back(
						pSubTexture.leftBottomUV, 
						pSubTexture.rightTopUV,
						duration
					);
				} 

				pSprite->insertFrames(std::move(framesDescriptions));
			}
		}
	}

	auto levelsIt = document.FindMember("levels");
	if (levelsIt != document.MemberEnd()) {
		for (const auto& currentLevel : levelsIt->value.GetArray()) {
			const auto description = currentLevel["description"].GetArray();
			std::vector<std::string> levelRows;
			size_t maxRowLength = 0;
			levelRows.reserve(description.Size());
			for (const auto& currentRow : description) {
				levelRows.emplace_back(currentRow.GetString());
				if (maxRowLength < levelRows.back().length())
					maxRowLength = levelRows.back().length();
			}

			for (auto& currentRow : levelRows) {
				while (currentRow.length() < maxRowLength) {
					currentRow.append("D");
				}
			}

			m_levels.emplace_back(std::move(levelRows));
		}
	}

	return true;
}

std::string ResourceManager::getFileString(const std::string &relativeFilePath){
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
