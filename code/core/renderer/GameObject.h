#pragma once

#include "GL/glew.h"
#include "glm/glm.hpp"

#include "Texture.h"
#include "SpriteRenderer.h"

class GameObject
{
public:
	Texture2D Sprite;

	glm::vec2 Position;
	glm::vec2 Size;
	glm::vec2 Velocity;
	glm::vec3 Color;
	float RotationAngle;

	bool IsSolid;
	bool IsDestroyed;

public:
	GameObject();
	GameObject(glm::vec2 position, glm::vec2 size, Texture2D sprite,
			   glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));

	virtual void Draw(SpriteRenderer &renderer);
};