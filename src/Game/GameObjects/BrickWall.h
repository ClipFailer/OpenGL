#pragma once

#include <memory>

#include <glm/vec2.hpp>

#include "IGameObject.h"

namespace Renderer {
	class Sprite;
}

class BrickWall : public IGameObject {
public:	
	BrickWall(
		const std::shared_ptr<Renderer::Sprite> 	pSprite,
		const glm::vec2& 							position,
		const glm::vec2& 							size,
		const float 								rotation
	);
	
	virtual void render() const override;
	virtual void update(float deltaTime) override;

private:
	std::shared_ptr<Renderer::Sprite> m_pSprite;

};