#include "Game.h"

SpriteRenderer *Renderer;
GameObject *Paddle;


Game::Game(unsigned int width, unsigned int height)
	: State(GameState::GAME_ACTIVE), Keys(), Width(width), Height(height)
{
}

Game::~Game()
{
	delete Renderer;
	delete Paddle;
}

void Game::Init()
{
	ResourceManager::LoadShader("resources/shaders/spriteShader.vert", "resources/shaders/spriteShader.frag", nullptr, "sprite");

	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(Width), static_cast<float>(Height), 0.0f, -1.0f, 1.0f);

	ResourceManager::GetShader("sprite").Use().SetInt("image", 0);
	ResourceManager::GetShader("sprite").SetMat4("projection", projection);

	ResourceManager::LoadTexture("resources/textures/background2.jpg", GL_FALSE, "background");
	ResourceManager::LoadTexture("resources/textures/roflan.png", GL_TRUE, "face");
	ResourceManager::LoadTexture("resources/textures/block.png", GL_FALSE, "block");
	ResourceManager::LoadTexture("resources/textures/block_solid.png", GL_FALSE, "block_solid");
	ResourceManager::LoadTexture("resources/textures/paddle.png", true, "paddle");

	Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));

	GameLevel one; 
	GameLevel two; 
	GameLevel three; 
	GameLevel four; 
	
	one.Load("resources/levels/one.lvl", Width, Height * 0.5);
	two.Load("resources/levels/two.lvl", Width, Height * 0.5);
	three.Load("resources/levels/three.lvl", Width, Height * 0.5);
	four.Load("resources/levels/four.lvl", Width, Height * 0.5);

	Levels.push_back(one);
	Levels.push_back(two);
	Levels.push_back(three);
	Levels.push_back(four);

	CurrentLevel = 0;

	glm::vec2 paddlePosition = glm::vec2(Width / 2 - PADDLE_SIZE.x / 2, Height - PADDLE_SIZE.y);
	Paddle = new GameObject(paddlePosition, PADDLE_SIZE, ResourceManager::GetTexture("paddle"));
}

void Game::ProcessInput(float deltaTime)
{
	if (State == GameState::GAME_ACTIVE)
	{
		GLfloat velocity = PADDLE_VELOCITY * deltaTime;
		// Move playerboard
		if (this->Keys[GLFW_KEY_A])
		{
			if (Paddle->Position.x >= 0)
				Paddle->Position.x -= velocity;
		}
		if (this->Keys[GLFW_KEY_D])
		{
			if (Paddle->Position.x <= Width - Paddle->Size.x)
				Paddle ->Position.x += velocity;
		}
	}
}

void Game::Update(float deltaTime)
{
}

void Game::Render()
{
	if (State == GameState::GAME_ACTIVE)
	{
		Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f);
		
		Levels[CurrentLevel].Draw(*Renderer);
		
		Paddle->Draw(*Renderer);
	}
}
