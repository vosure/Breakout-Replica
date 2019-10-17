#pragma once

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "ft2build.h"
#include FT_FREETYPE_H

#include "../utils/ResourceManager.h"

#include <map>
#include <iostream>

struct Character
{
	unsigned int TextureID;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	unsigned int Advance;
};

class TextRenderer
{
public:
	std::map<char, Character> Characters;
	Shader TextShader;

public:
	TextRenderer(unsigned int width, unsigned int height);

	void Load(std::string font, unsigned int fontSize);
	void Render(std::string text, float x, float y, float scale, const glm::vec3 color = glm::vec3(1.0f));

private:
	unsigned int VAO;
	unsigned int VBO;
};
