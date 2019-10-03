#pragma once 

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Texture.h"
#include "Shader.h"

class SpriteRenderer
{
public:
	SpriteRenderer(Shader &shader);
	~SpriteRenderer();

	void DrawSprite(Texture2D &texture, const glm::vec2 &position, 
					const glm::vec2 &size = glm::vec2(10, 10), float rotationAngle = 0.0f, 
					const glm::vec3 &color = glm::vec3(1.0f));

private:
	Shader shader;
	unsigned int quadVAO;

private:
	void InitRenderData();
};