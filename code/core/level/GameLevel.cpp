#pragma once

#include "GameLevel.h"

GameLevel::GameLevel()
{
}

//TODO(vosure): Level loading should be done by resource manager
void GameLevel::Load(const char * filepath, unsigned int width, unsigned int height)
{
	Bricks.clear();

	GameLevel level;
	unsigned int tileCode;
	std::string line;
	std::ifstream fstream(filepath);
	std::vector<std::vector<unsigned int>> tileData;

	if (fstream)
	{
		while (std::getline(fstream, line))
		{
			std::istringstream sstream(line);
			std::vector<unsigned int> row;
			while (sstream >> tileCode)
				row.push_back(tileCode);
			tileData.push_back(row);
		}
		if (tileData.size() > 0)
			Init(tileData, width, height);
	}
}

void GameLevel::Draw(SpriteRenderer &renderer)
{
	for (GameObject &tile : Bricks)
		if (!tile.IsDestroyed)
			tile.Draw(renderer);
}

bool GameLevel::IsCompleted()
{
	for (GameObject &tile : Bricks)
		if (!tile.IsSolid && !tile.IsDestroyed)
			return false;
	return true;
}

void GameLevel::Init(std::vector<std::vector<unsigned int>> tileData, unsigned int width, unsigned int height)
{
	float block_width = (float)width / (float)tileData[0].size();
	float block_height = (float)height / (float)tileData.size();

	for (int y = 0; y < tileData.size(); y++)
	{
		for (int x = 0; x < tileData[0].size(); x++)
		{
			glm::vec2 position(block_width * x, block_height * y);
			glm::vec2 size(block_width, block_height);
			if (tileData[y][x] == 1) //NOTE(vosure): Solid Block
			{			
				GameObject object(position, size, ResourceManager::GetTexture("block_solid"), glm::vec3(0.9f, 0.9f, 1.6f));
				object.IsSolid = true;
				Bricks.push_back(object);
			}
			else if (tileData[y][x] > 1)
			{
				glm::vec3 color = glm::vec3(1.0f);
				if (tileData[y][x] == 2)
					color = glm::vec3(1.0f, 0.3f, 0.3f);
				else if (tileData[y][x] == 3)
					color = glm::vec3(0.9f, 0.9f, 0.1f);
				else if (tileData[y][x] == 4)
					color = glm::vec3(0.2f, 1.0f, 0.4f);
				else if (tileData[y][x] == 5)
					color = glm::vec3(0.5f, 0.4f, 1.0f);

				GameObject object(position, size, ResourceManager::GetTexture("block"), color);
				object.IsSolid = false;
				Bricks.push_back(object);
			}
		}
	}
}
