#include "Game.h"

SpriteRenderer *Renderer;
GameObject *Paddle;
Ball *BallObject;

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
	ResourceManager::LoadTexture("resources/textures/roflan.png", GL_TRUE, "ball");
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

	glm::vec2 ballPosition = paddlePosition + glm::vec2(PADDLE_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
	BallObject = new Ball(ballPosition, BALL_RADIUS, BALL_VELOCITY, ResourceManager::GetTexture("ball"));
}

void Game::ProcessInput(float deltaTime)
{
	if (State == GameState::GAME_ACTIVE)
	{
		GLfloat velocity = PADDLE_VELOCITY * deltaTime;
		// Move playerboard
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
				Paddle ->Position.x += velocity;
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

	if (BallObject->Position.y >= Height) // Did ball reach bottom edge?
	{
		this->ResetLevel();
		this->ResetPaddle();
	}
}

void Game::Render()
{
	if (State == GameState::GAME_ACTIVE)
	{
		Renderer->DrawSprite(ResourceManager::GetTexture("background"), glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f);
		
		Levels[CurrentLevel].Draw(*Renderer);
		
		Paddle->Draw(*Renderer);

		BallObject->Draw(*Renderer);
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
		return std::make_tuple(GL_TRUE, GetDirection(difference), difference);
	else
		return std::make_tuple(GL_FALSE, MoveDirection::UP, glm::vec2(0, 0));
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
				// Destroy block if not solid
				if (!brick.IsSolid)
					brick.IsDestroyed = GL_TRUE;
				// Collision resolution
				MoveDirection dir = std::get<1>(collision);
				glm::vec2 diff_vector = std::get<2>(collision);
				if (dir == MoveDirection::LEFT || dir == MoveDirection::RIGHT) // Horizontal collision
				{
					BallObject->Velocity.x = -BallObject->Velocity.x; // Reverse horizontal velocity
					// Relocate
					GLfloat penetration = BallObject->Radius - std::abs(diff_vector.x);
					if (dir == MoveDirection::LEFT)
						BallObject->Position.x += penetration; // Move ball to right
					else
						BallObject->Position.x -= penetration; // Move ball to left;
				}
				else // Vertical collision
				{
					BallObject->Velocity.y = -BallObject->Velocity.y; // Reverse vertical velocity
					// Relocate
					GLfloat penetration = BallObject->Radius - std::abs(diff_vector.y);
					if (dir == MoveDirection::UP)
						BallObject->Position.y -= penetration; // Move ball bback up
					else
						BallObject->Position.y += penetration; // Move ball back down
				}
			}

			Collision result = CheckCollision(*BallObject, *Paddle);
			if (!BallObject->IsStuck && std::get<0>(result))
			{
				// Check where it hit the board, and change velocity based on where it hit the board
				GLfloat centerBoard = Paddle->Position.x + Paddle->Size.x / 2;
				GLfloat distance = (BallObject->Position.x + BallObject->Radius) - centerBoard;
				GLfloat percentage = distance / (Paddle->Size.x / 2);
				// Then move accordingly
				GLfloat strength = 2.0f;
				glm::vec2 oldVelocity = BallObject->Velocity;
				BallObject->Velocity.x = BALL_VELOCITY.x * percentage * strength;
				//Ball->Velocity.y = -Ball->Velocity.y;
				BallObject->Velocity = glm::normalize(BallObject->Velocity) * glm::length(oldVelocity); // Keep speed consistent over both axes (multiply by length of old velocity, so total strength is not changed)
				// Fix sticky paddle
				BallObject->Velocity.y = -1 * abs(BallObject->Velocity.y);
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
	Paddle->Position = glm::vec2(this->Width / 2 - PADDLE_SIZE.x / 2, Height - PADDLE_SIZE.y);
	BallObject->Reset(Paddle->Position + glm::vec2(PADDLE_SIZE.x / 2 - BALL_RADIUS, -(BALL_RADIUS * 2)), BALL_VELOCITY);
}

MoveDirection Game::GetDirection(glm::vec2 target)
{
	glm::vec2 directions[] = {
	  glm::vec2(0.0f, 1.0f),
	  glm::vec2(1.0f, 0.0f),
	  glm::vec2(0.0f, -1.0f),
	  glm::vec2(-1.0f, 0.0f)
	};

	GLfloat max = 0.0f;
	GLuint bestMatch = -1;

	for (GLuint i = 0; i < 4; i++)
	{
		GLfloat dot_product = glm::dot(glm::normalize(target), directions[i]);
		if (dot_product > max)
		{
			max = dot_product;
			bestMatch = i;
		}
	}

	return (MoveDirection)bestMatch;
}
