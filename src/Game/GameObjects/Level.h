#pragma once

#include <string>
#include <memory>
#include <vector>

class IGameObject;

class Level {
public:
	Level(
		const std::vector<std::string>& 	levelDescription,
		const unsigned int 					blockSize
	);

	void render() const noexcept;
	void update(const float deltaTime);

private:
	unsigned int m_width;
	unsigned int m_height;
	unsigned int m_blockSize;

	std::vector<std::shared_ptr<IGameObject>> m_mapObjects;
};