#pragma once 

#include "GameObject.h"


class Ball : public GameObject
{
public:
	float Radius;
	bool IsStuck;

	bool Sticky;
	bool PassThrough;

public:
	Ball();
	Ball(const glm::vec2 &position, float radius, const glm::vec2 &velocity, Texture2D sprite);

	const glm::vec2 &Move(float deltaTime, unsigned int windowWidth);
	void Reset(const glm::vec2 &position, const glm::vec2 &velocity);
};