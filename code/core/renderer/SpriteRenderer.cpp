#pragma once

#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer(Shader &shader)
{
	this->shader = shader;
	InitRenderData();
}

SpriteRenderer::~SpriteRenderer()
{
	glDeleteVertexArrays(1, &quadVAO);
}

void SpriteRenderer::DrawSprite(Texture2D &texture, const glm::vec2 &position, const glm::vec2 &size, float rotationAngle, const glm::vec3 &color)
{
	shader.Use();

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position, 0.0f));
	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
	model = glm::rotate(model, rotationAngle, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
	model = glm::scale(model, glm::vec3(size, 1.0f));

	shader.SetMat4("model", model);
	shader.SetVec3("spriteColor", color);

	glActiveTexture(GL_TEXTURE0);
	texture.Bind();

	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void SpriteRenderer::InitRenderData()
{
	unsigned int VBO;
	float vertices[] =
	{
		0.0f, 1.0f,	 0.0f, 1.0f,
		1.0f, 0.0f,	 1.0f, 0.0f,
		0.0f, 0.0f,	 0.0f, 0.0f,
		0.0f, 1.0f,	 0.0f, 1.0f,
		1.0f, 1.0f,	 1.0f, 1.0f,
		1.0f, 0.0f,	 1.0f, 0.0f
	};

	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(quadVAO);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
