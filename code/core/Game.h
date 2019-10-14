#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "level/GameLevel.h"
#include "particles/ParticleGenerator.h"
#include "post_processing/PostProcessor.h"
#include "renderer/Ball.h"

#include <vector>
#include <tuple>

const glm::vec2 PADDLE_SIZE(100, 20);
const float PADDLE_VELOCITY(500.0f);

const glm::vec2 BALL_VELOCITY(100.0f, -350.0f);
const float BALL_RADIUS(12.5f);

enum class GameState
{
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

enum class MoveDirection
{
	UP, RIGHT, DOWN, LEFT
};

typedef std::tuple<bool, MoveDirection, glm::vec2> Collision;

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

private:

private:
	bool CheckCollision(GameObject obj1, GameObject obj2);
	Collision CheckCollision(Ball obj1, GameObject obj2);

	void OnCollisionEnter();

	void ResetLevel();
	void ResetPaddle();

	MoveDirection GetDirection(glm::vec2 target);
};