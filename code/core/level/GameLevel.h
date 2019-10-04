#pragma once

#include "GL/glew.h"
#include "glm/glm.hpp"

#include "../renderer/GameObject.h"
#include "../utils/ResourceManager.h"

#include <vector>

class GameLevel
{
public:
	std::vector<GameObject> Bricks;

public:
	GameLevel();

	void Load(const char *filepath, unsigned int width, unsigned int height);
	void Draw(SpriteRenderer &renderer);
	bool IsCompleted();

private:
	void Init(std::vector<std::vector<unsigned int>> tileData, unsigned int width, unsigned int height);
};