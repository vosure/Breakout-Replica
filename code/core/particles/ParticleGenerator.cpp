#pragma once

#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(Shader shader, Texture2D texture, unsigned int amount)
	: ParticleShader(shader), Texture(texture), Amount(amount)
{
	Init();
}

void ParticleGenerator::Update(float deltaTime, GameObject & object, unsigned int count, const glm::vec2 offset)
{
	for (unsigned int i = 0; i < count; i++)
	{
		int  oldParticle = FindOldParticle();
		Respawn(Particles[oldParticle], object, offset);
	}

	for (unsigned int i = 0; i < Amount; i++)
	{
		Particle &particle = Particles[i];
		particle.LifeTime -= deltaTime;
		if (particle.LifeTime > 0.0f)
		{
			particle.Position -= particle.Velocity * deltaTime;
			particle.Color.a -= deltaTime * 2.5f;
		}
	}
}

void ParticleGenerator::Draw()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	ParticleShader.Use();

	for (Particle particle : Particles)
	{
		if (particle.LifeTime > 0.0f)
		{
			ParticleShader.SetVec2("offset", particle.Position);
			ParticleShader.SetVec4("color", particle.Color);
			Texture.Bind();

			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			glBindVertexArray(0);
		}
	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::Init()
{
	unsigned int VBO;
	float vertices[] =
	{
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	for (unsigned int i = 0; i < Amount; i++)
	{
		Particles.push_back(Particle());
	}
}

unsigned int LastUsedParticle = 0;
unsigned int ParticleGenerator::FindOldParticle()
{
	LastUsedParticle = 0;
	for (unsigned int i = 0; i < Amount; i++)
	{
		if (Particles[i].LifeTime <= 0.0f)
		{
			LastUsedParticle = i;
			return i;
		}
	}

	for (unsigned int i = 0; i < LastUsedParticle; i++)
	{
		if (Particles[i].LifeTime <= 0.0f)
		{
			LastUsedParticle = i;
			return i;
		}
	}

	LastUsedParticle = 0;

	return 0;
}

void ParticleGenerator::Respawn(Particle &particle, GameObject &object, const glm::vec2 offset)
{
	float random = ((rand() & 100) - 50) / 10.0f;
	float color = 0.5f + ((rand() % 100) / 100.0f);

	particle.Position = object.Position + random + offset;
	particle.Color = glm::vec4(color, color, color, 1.0f);
	particle.LifeTime = 1.0f;
	particle.Velocity = object.Velocity * 0.1f;
}
