#pragma once

#include "GL/glew.h"
#include "glm/glm.hpp"

#include "../game_objects/GameObject.h"
#include "../renderer/Shader.h"
#include "../renderer/Texture.h"

#include <vector>

struct Particle
{
	glm::vec2 Position;
	glm::vec2 Velocity;
	glm::vec4 Color;

	float LifeTime;

	Particle()
		: Position(0.0f), Velocity(0.0f), Color(1.0f), LifeTime(0.0f)
	{
	}
};

class ParticleGenerator
{
public:
	ParticleGenerator(Shader shader, Texture2D texture, unsigned int amount);

	void Update(float deltaTime, GameObject &object, unsigned int count, const glm::vec2 offset = glm::vec2(0.0f, 0.0f));
	void Draw();

private:
	std::vector<Particle> Particles;
	unsigned int Amount;

	Shader ParticleShader;
	Texture2D Texture;

	unsigned int VAO;

private:
	void Init();
	unsigned int FindOldParticle();
	void Respawn(Particle &particle, GameObject &object, const glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};

