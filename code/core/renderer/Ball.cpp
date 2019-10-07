#include "Ball.h"

Ball::Ball()
	: GameObject(), Radius(12.5f), IsStuck(true)
{
}

Ball::Ball(const glm::vec2 &position, float radius, const glm::vec2 &velocity, Texture2D sprite)
	: GameObject(position, glm::vec2(radius * 2, radius * 2), sprite, glm::vec3(1.0f), velocity), Radius(radius), IsStuck(true)
{
}

const glm::vec2 & Ball::Move(float deltaTime, unsigned int windowWidth)
{
	if (!IsStuck)
	{
		Position += Velocity * deltaTime;

		if (Position.x <= 0.0f)
		{
			Velocity.x = -Velocity.x;
			Position.x = 0.0f;
		}
		else if (Position.x + Size.x >= windowWidth)
		{
			Velocity.x = -Velocity.x;
			Position.x = windowWidth - Size.x;
		}

		if (Position.y <= 0.0f)
		{
			Velocity.y = -Velocity.y;
			Position.y = 0.0f;
		}
	}
	return Position;
}

void Ball::Reset(const glm::vec2 &position, const glm::vec2 &velocity)
{
	Position = position;
	Velocity = velocity;
	IsStuck = true;
}
