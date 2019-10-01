#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"

enum class GameState
{
	GAME_ACTIVE, 
	GAME_MENU,
	GAME_WIN
};

class Game
{
public:
	GameState State;
	bool Keys[1024];
	unsigned int Width;
	unsigned int Height;

public:
	Game(unsigned int width, unsigned int height);
	~Game();

	void Init();
	void ProcessInput(float deltaTime);
	void Update(float deltaTime);
	void Render();
};