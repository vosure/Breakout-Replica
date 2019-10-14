#pragma once

#include "../renderer/SpriteRenderer.h"

#include <iostream>

class PostProcessor
{
public:
	Shader PostProcessorShader;
	Texture2D Texture;

	unsigned int Width;
	unsigned int Height;

	bool Confuse;
	bool Chaos;
	bool Shake;

public:
	PostProcessor(Shader shader, unsigned int width, unsigned int height);

	void BeginRender();
	void EndRender();
	void Render(float deltaTime);
	void Print();

private:
	unsigned int MSFBO;
	unsigned int FBO;
	unsigned int RBO;
	unsigned int VAO;

private:
	void Init();
};