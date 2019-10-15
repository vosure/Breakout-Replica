#pragma once

#include "GameObject.h"

GameObject::GameObject()
	: Position(0.0f, 0.0f), Size(1.0, 1.0f), Velocity(0.0f, 0.0f), Color(1.0f), RotationAngle(0.0f),
	Sprite(), IsSolid(false), IsDestroyed(false)
{
}

GameObject::GameObject(glm::vec2 position, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity)
	: Position(position), Size(size), Velocity(velocity), Color(color), RotationAngle(0.0f),
	Sprite(sprite), IsSolid(false), IsDestroyed(false)
{
}

void GameObject::Draw(SpriteRenderer &renderer)
{
	renderer.DrawSprite(Sprite, Position, Size, RotationAngle, Color);
}
