#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "level/GameLevel.h"

#include <vector>

enum class GameState
{
	GAME_ACTIVE, 
	GAME_MENU,
	GAME_WIN
};

const glm::vec2 PADDLE_SIZE(100, 20);
const float PADDLE_VELOCITY(500.0f);

class Game
{
public:
	GameState State;
	bool Keys[1024];
	unsigned int Width;
	unsigned int Height;

	std::vector<GameLevel> Levels;
	unsigned int CurrentLevel;

public:
	Game(unsigned int width, unsigned int height);
	~Game();

	void Init();
	void ProcessInput(float deltaTime);
	void Update(float deltaTime);
	void Render();
};