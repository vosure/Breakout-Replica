#pragma once

#include "GL/glew.h"
#include "glm/glm.hpp"

#include "GameObject.h"

const glm::vec2 SIZE(60.0f, 20.0f);
const glm::vec2 VELOCITY(0.0f, 150.0f);

class PowerUp : public GameObject
{
public:
	std::string Type;
	float Duration;
	bool IsActivated;

public:
	PowerUp(std::string type, const glm::vec3 &color, float duration, const glm::vec2 &position, Texture2D texture)
		: GameObject(position, SIZE, texture, color, VELOCITY), Type(type), Duration(duration), IsActivated(false)
	{
	}
};