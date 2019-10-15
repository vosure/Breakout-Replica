#include "Game.h"

PostProcessor *Effects;
SpriteRenderer *Renderer;
GameObject *Paddle;
Ball *BallObject;
ParticleGenerator *ParticleGen;
float ShakeTime = 0.0f;

Game::Game(unsigned int width, unsigned int height)
	: State(GameState::GAME_ACTIVE), Keys(), Width(width), Height(height)
{
}

Game::~Game()
{
	delete Renderer;
	delete Paddle;
	delete BallObject;
	delete ParticleGen;
	delete Effects;
}

void Game::Init()
{
	ResourceManager::LoadShader("resources/shaders/spriteShader.vert", "resources/shaders/spriteShader.frag", nullptr, "sprite");

	ResourceManager::LoadShader("resources/shaders/particleShader.vert", "resources/shaders/particleShader.frag", nullptr, "particle");

	ResourceManager::LoadShader("resources/shaders/postProcessingShader.vert", "resources/shaders/postProcessingShader.frag", nullptr, "postProcessing");

	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(Width), static_cast<float>(Height), 0.0f, -1.0f, 1.0f);

	ResourceManager::GetShader("sprite").Use().SetInt("image", 0);
	ResourceManager::GetShader("sprite").SetMat4("projection", projection);

	ResourceManager::GetShader("particle").Use().SetInt("image", 0);
	ResourceManager::GetShader("particle").SetMat4("projection", projection);

	ResourceManager::LoadTexture("resources/textures/background2.jpg", false, "background");
	ResourceManager::LoadTexture("resources/textures/roflan.png", true, "ball");
	ResourceManager::LoadTexture("resources/textures/block.png", false, "block");
	ResourceManager::LoadTexture("resources/textures/block_solid.png", false, "block_solid");
	ResourceManager::LoadTexture("resources/textures/paddle.png", true, "paddle");
	ResourceManager::LoadTexture("resources/textures/particle2.png", true, "particle");

	ResourceManager::LoadTexture("resources/textures/powerup_speed.png", true, "powerup_speed");
	ResourceManager::LoadTexture("resources/textures/powerup_sticky.png", true, "powerup_sticky");
	ResourceManager::LoadTexture("resources/textures/powerup_increase.png", true, "powerup_increase");
	ResourceManager::LoadTexture("resources/textures/powerup_confuse.png", true, "powerup_confuse");
	ResourceManager::LoadTexture("resources/textures/powerup_chaos.png", true, "powerup_chaos");
	ResourceManager::LoadTexture("resources/textures/powerup_passthrough.png", true, "powerup_passthrough");

	Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
	ParticleGen = new ParticleGenerator(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle"), 1000);
	Effects = new PostProcessor(ResourceManager::GetShader("postProcessing"), Width, Height);
	Effects->Print();

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

	CurrentLevel = 2;

	glm::vec2 paddlePosition = glm::vec2(Width / 2 - PADDLE_SIZE.x / 2, Height - PADDLE_SIZE.y);
	Paddle = new GameObject(paddlePosition, PADDLE_SIZE, ResourceManager::GetTexture("paddle"));

	glm::vec2 ballPosition = paddlePosition + glm::vec2(PADDLE_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
	BallObject = new Ball(ballPosition, BALL_RADIUS, BALL_VELOCITY, ResourceManager::GetTexture("ball"));
}

void Game::ProcessInput(float deltaTime)
{
	if (State == GameState::GAME_ACTIVE)
	{
		GLfloat velocity = PADDLE_VELOCITY * deltaTime;

		if (Keys[GLFW_KEY_A])
		{
			if (Paddle->Position.x >= 0)
			{
				Paddle->Position.x -= velocity;
				if (BallObject->IsStuck)
					BallObject->Position.x -= velocity;
			}
		}
		if (Keys[GLFW_KEY_D])
		{
			if (Paddle->Position.x <= Width - Paddle->Size.x)
			{
				Paddle->Position.x += velocity;
				if (BallObject->IsStuck)
					BallObject->Position.x += velocity;
			}
		}
		if (Keys[GLFW_KEY_SPACE])
		{
			BallObject->IsStuck = false;
		}
	}
}

void Game::Update(float deltaTime)
{
	BallObject->Move(deltaTime, Width);

	OnCollisionEnter();

	ParticleGen->Update(deltaTime, *BallObject, 2, glm::vec2(BallObject->Radius / 2));

	UpdatePowerUp(deltaTime);

	if (ShakeTime > 0.0f)
	{
		ShakeTime -= deltaTime;
		if (ShakeTime <= 0.0f)
			Effects->Shake = false;
	}

	if (BallObject->Position.y >= Height)
	{
		this->ResetLevel();
		this->ResetPaddle();
	}
}

void Game::Render()
{
	if (State == GameState::GAME_ACTIVE)
	{
		Effects->BeginRender();

		Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f);
		Levels[CurrentLevel].Draw(*Renderer);
		Paddle->Draw(*Renderer);

		for (PowerUp &powerUp : PowerUps)
			if (!powerUp.IsDestroyed)
				powerUp.Draw(*Renderer);

		ParticleGen->Draw();
		BallObject->Draw(*Renderer);

		Effects->EndRender();

		Effects->Render(glfwGetTime());
	}
}

void Game::SpawnPowerUp(GameObject &brick)
{
	if (PowerUpShouldSpawn(75))
		this->PowerUps.push_back(PowerUp("speed", glm::vec3(0.5f, 0.5f, 1.0f), 0.0f, brick.Position, ResourceManager::GetTexture("powerup_speed")));
	if (PowerUpShouldSpawn(75))
		this->PowerUps.push_back(PowerUp("sticky", glm::vec3(1.0f, 0.5f, 1.0f), 20.0f, brick.Position, ResourceManager::GetTexture("powerup_sticky")));
	if (PowerUpShouldSpawn(75))
		this->PowerUps.push_back(PowerUp("pass-through", glm::vec3(0.5f, 1.0f, 0.5f), 10.0f, brick.Position, ResourceManager::GetTexture("powerup_passthrough")));
	if (PowerUpShouldSpawn(75))
		this->PowerUps.push_back(PowerUp("pad-size-increase", glm::vec3(1.0f, 0.6f, 0.4), 0.0f, brick.Position, ResourceManager::GetTexture("powerup_increase")));
	if (PowerUpShouldSpawn(15))
		this->PowerUps.push_back(PowerUp("confuse", glm::vec3(1.0f, 0.3f, 0.3f), 15.0f, brick.Position, ResourceManager::GetTexture("powerup_confuse")));
	if (PowerUpShouldSpawn(15))
		this->PowerUps.push_back(PowerUp("chaos", glm::vec3(0.9f, 0.25f, 0.25f), 15.0f, brick.Position, ResourceManager::GetTexture("powerup_chaos")));
}

void Game::UpdatePowerUp(float deltaTime)
{
	for (PowerUp &powerUp : PowerUps)
	{
		powerUp.Position += powerUp.Velocity * deltaTime;
		if (powerUp.IsActivated)
		{
			powerUp.Duration -= deltaTime;

			if (powerUp.Duration <= 0.0f)
			{
				powerUp.IsActivated = false;
				if (powerUp.Type == "sticky")
				{
					if (!IsPowerUpActive(PowerUps, "sticky"))
						BallObject->Sticky = false;
					Paddle->Color = glm::vec3(1.0f);
				}
			}
			else if (powerUp.Type == "pass-through")
			{
				if (!IsPowerUpActive(PowerUps, "pass-through"))
				{
					BallObject->PassThrough = false;
					BallObject->Color = glm::vec3(1.0f);
				}
			}
			else if (powerUp.Type == "confuse")
			{
				if (!IsPowerUpActive(PowerUps, "confuse"))
				{
					Effects->Confuse = false;
				}
			}
			else if (powerUp.Type == "chaos")
			{
				if (!IsPowerUpActive(PowerUps, "chaos"))
				{
					Effects->Chaos = false;
				}
			}

		}
		PowerUps.erase(std::remove_if(this->PowerUps.begin(), this->PowerUps.end(),
			[](const PowerUp &powerUp)
		{
			return powerUp.IsDestroyed && !powerUp.IsActivated;
		}
		), this->PowerUps.end());
	}
}

bool Game::CheckCollision(GameObject obj1, GameObject obj2)
{
	bool collisionX = obj1.Position.x + obj1.Size.x >= obj2.Position.x &&
		obj2.Position.x + obj2.Size.x >= obj1.Position.x;

	bool collisionY = obj1.Position.y + obj1.Size.y >= obj2.Position.y &&
		obj2.Position.y + obj2.Size.y >= obj1.Position.y;

	return collisionX && collisionY;
}

Collision Game::CheckCollision(Ball obj1, GameObject obj2)
{
	glm::vec2 centerPoint(obj1.Position + obj1.Radius);

	glm::vec2 AABBHalfExtents(obj2.Size.x / 2, obj2.Size.y / 2);
	glm::vec2 AABBCenter(obj2.Position.x + AABBHalfExtents.x,
		obj2.Position.y + AABBHalfExtents.y);

	glm::vec2 difference = centerPoint - AABBCenter;
	glm::vec2 clamped = glm::clamp(difference, -AABBHalfExtents, AABBHalfExtents);
	glm::vec2 closest = AABBCenter + clamped;

	difference = closest - centerPoint;

	if (glm::length(difference) < obj1.Radius)
		return std::make_tuple(true, GetDirection(difference), difference);
	else
		return std::make_tuple(false, MoveDirection::UP, glm::vec2(0, 0));
}

void Game::OnCollisionEnter()
{
	for (GameObject &brick : Levels[CurrentLevel].Bricks)
	{
		if (!brick.IsDestroyed)
		{
			Collision collision = CheckCollision(*BallObject, brick);
			if (std::get<0>(collision))
			{
				if (!brick.IsSolid)
				{
					brick.IsDestroyed = true;
					SpawnPowerUp(brick);
				}
				else
				{
					ShakeTime = 0.05f;
					Effects->Shake = true;
				}

				MoveDirection direction = std::get<1>(collision);
				glm::vec2 diff_vector = std::get<2>(collision);
				if (!(BallObject->PassThrough && !brick.IsSolid))
				{
					if (direction == MoveDirection::LEFT || direction == MoveDirection::RIGHT)
					{
						BallObject->Velocity.x = -BallObject->Velocity.x;
						float penetration = BallObject->Radius - std::abs(diff_vector.x);
						if (direction == MoveDirection::LEFT)
							BallObject->Position.x += penetration;
						else
							BallObject->Position.x -= penetration;
					}
					else
					{
						BallObject->Velocity.y = -BallObject->Velocity.y;
						float penetration = BallObject->Radius - std::abs(diff_vector.y);

						if (direction == MoveDirection::UP)
							BallObject->Position.y -= penetration;
						else
							BallObject->Position.y += penetration;
					}
				}
			}

			for (PowerUp &powerUp : PowerUps)
			{
				if (!powerUp.IsDestroyed)
				{
					if (powerUp.Position.y >= Height)
						powerUp.IsDestroyed = true;

					if (CheckCollision(*Paddle, powerUp))
					{
						ActivatePowerUp(powerUp);
						powerUp.IsDestroyed = true;
						powerUp.IsActivated = true;
					}
				}
			}

			Collision result = CheckCollision(*BallObject, *Paddle);
			if (!BallObject->IsStuck && std::get<0>(result))
			{
				float centerBoard = Paddle->Position.x + Paddle->Size.x / 2;
				float distance = (BallObject->Position.x + BallObject->Radius) - centerBoard;
				float percentage = distance / (Paddle->Size.x / 2);

				float strength = 2.0f;
				glm::vec2 oldVelocity = BallObject->Velocity;
				BallObject->Velocity.x = BALL_VELOCITY.x * percentage * strength;
				BallObject->Velocity = glm::normalize(BallObject->Velocity) * glm::length(oldVelocity);

				BallObject->Velocity.y = -1 * abs(BallObject->Velocity.y);

				BallObject->IsStuck = BallObject->Sticky;
			}
		}
	}
}

void Game::ResetLevel()
{
	if (CurrentLevel == 0)
		Levels[0].Load("resources/levels/one.lvl", Width, Height * 0.5f);
	else if (CurrentLevel == 1)
		Levels[1].Load("resources/levels/two.lvl", Width, Height * 0.5f);
	else if (CurrentLevel == 2)
		Levels[2].Load("resources/levels/three.lvl", Width, Height * 0.5f);
	else if (CurrentLevel == 3)
		Levels[3].Load("resources/levels/four.lvl", Width, Height * 0.5f);
}

void Game::ResetPaddle()
{
	Paddle->Size = PADDLE_SIZE;
	Paddle->Position = glm::vec2(Width / 2 - PADDLE_SIZE.x / 2, Height - PADDLE_SIZE.y);
	Paddle->Color = glm::vec3(1.0f);

	BallObject->Reset(Paddle->Position + glm::vec2(PADDLE_SIZE.x / 2 - BALL_RADIUS, -(BALL_RADIUS * 2)), BALL_VELOCITY);
	BallObject->PassThrough = BallObject->Sticky = false;
	BallObject->Color = glm::vec3(1.0f);

	Effects->Chaos = Effects->Confuse = false;

}

MoveDirection Game::GetDirection(glm::vec2 target)
{
	glm::vec2 directions[] = {
	  glm::vec2(0.0f, 1.0f),
	  glm::vec2(1.0f, 0.0f),
	  glm::vec2(0.0f, -1.0f),
	  glm::vec2(-1.0f, 0.0f)
	};

	float max = 0.0f;
	unsigned int bestMatch = -1;

	for (unsigned int i = 0; i < 4; i++)
	{
		float dotProduct = glm::dot(glm::normalize(target), directions[i]);
		if (dotProduct > max)
		{
			max = dotProduct;
			bestMatch = i;
		}
	}

	return (MoveDirection)bestMatch;
}

bool Game::PowerUpShouldSpawn(unsigned int chance)
{
	unsigned int result = rand() % chance;
	return result == 0;
}

void Game::ActivatePowerUp(PowerUp &powerUp)
{
	if (powerUp.Type == "speed")
	{
		BallObject->Velocity *= 1.2;
	}
	else if (powerUp.Type == "sticky")
	{
		BallObject->Sticky = true;
		Paddle->Color = glm::vec3(1.0f, 0.5f, 1.0f);
	}
	else if (powerUp.Type == "pass-through")
	{
		BallObject->PassThrough = true;
		BallObject->Color = glm::vec3(1.0f, 0.5f, 0.5f);
	}
	else if (powerUp.Type == "pad-size-increase")
	{
		Paddle->Size.x += 50;
	}
	else if (powerUp.Type == "confuse")
	{
		if (!Effects->Chaos)
			Effects->Confuse = true;
	}
	else if (powerUp.Type == "chaos")
	{
		if (!Effects->Confuse)
			Effects->Chaos = true;
	}
}

bool Game::IsPowerUpActive(std::vector<PowerUp> &powerUps, const std::string &type)
{
	for (const PowerUp &powerUp : powerUps)
	{
		if (powerUp.IsActivated)
			if (powerUp.Type == type)
				return true;
	}
	return false;
}
