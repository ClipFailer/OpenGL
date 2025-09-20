#include "Level.h"

#include <iostream>

#include "IGameObject.h"
#include "Resources/ResourceManager.h"
#include "BrickWall.h"

std::shared_ptr<IGameObject> createGameObjectFromDescriprion(
	const char 			description,
	const glm::vec2& 	position,
	const glm::vec2& 	size,
	const float 		rotation = 0.f
) {
	switch (description)
	{
	case '0':
		return std::make_shared<BrickWall>(
			ResourceManager::getSprite("brickWall_right"),
			position,
			size,
			rotation
		);
		break;
	case '1':
		return std::make_shared<BrickWall>(
			ResourceManager::getSprite("brickWall_bottom"),
			position,
			size,
			rotation
		);
		break;
	case '2':
		return std::make_shared<BrickWall>(
			ResourceManager::getSprite("brickWall_left"),
			position,
			size,
			rotation
		);
		break;
	case '3':
		return std::make_shared<BrickWall>(
			ResourceManager::getSprite("brickWall_top"),
			position,
			size,
			rotation
		);
		break;
	case '4':
		return std::make_shared<BrickWall>(
			ResourceManager::getSprite("brickWall"),
			position,
			size,
			rotation
		);
		break;
	case 'G':
		return std::make_shared<BrickWall>(
			ResourceManager::getSprite("brickWall_bottom_left"),
			position,
			size,
			rotation
		);
		break;
	case 'H':
		return std::make_shared<BrickWall>(
			ResourceManager::getSprite("brickWall_bottom_right"),
			position,
			size,
			rotation
		);
		break;
	case 'I':
		return std::make_shared<BrickWall>(
			ResourceManager::getSprite("brickWall_top_left"),
			position,
			size,
			rotation
		);
		break;
	case 'J':
		return std::make_shared<BrickWall>(
			ResourceManager::getSprite("brickWall_top_right"),
			position,
			size,
			rotation
		);
		break;
	case 'D':
		return nullptr;
		break;
	
	default:
		std::cerr << "Unknown game object description: " << description << std::endl;
		break;
	}

	return nullptr;
};

Level::Level(
	const std::vector<std::string>& 	levelDescription,
	const unsigned int 					blockSize)
	: m_blockSize(blockSize)
{
	if (levelDescription.empty()) {
		std::cerr << "Empty level description\n";
		return;	
	}

	m_width 	= levelDescription[0].length();
	m_height 	= levelDescription.size();

	m_mapObjects.reserve(m_width * m_height);

	unsigned int currentBottomOffset = blockSize * (m_height - 1);

	for (const std::string& currentRow : levelDescription) {
		unsigned int currentLeftOffset = 0;

		for (const char currentElement : currentRow) {
			m_mapObjects.emplace_back(
				createGameObjectFromDescriprion(
					currentElement,
					glm::vec2(currentLeftOffset, currentBottomOffset),
					glm::vec2(blockSize, blockSize
				)
			));

			currentLeftOffset += blockSize;
		}

		currentBottomOffset -= blockSize;
	}
};

void Level::render() const noexcept {
	for (const auto& currentGameObject : m_mapObjects) {
		if (currentGameObject) 
			currentGameObject->render();
	}
};

void Level::update(const float deltaTime) {
	for (const auto& currentGameObject : m_mapObjects) {
		if (currentGameObject) 
			currentGameObject->update(deltaTime);
	}
};
