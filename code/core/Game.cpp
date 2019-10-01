#include "Game.h"

Game::Game(unsigned int width, unsigned int height)
	: State(GameState::GAME_ACTIVE), Keys(), Width(width), Height(height)
{
}

Game::~Game()
{
}

void Game::Init()
{
}

void Game::ProcessInput(float deltaTime)
{
}

void Game::Update(float deltaTime)
{
}

void Game::Render()
{
}
