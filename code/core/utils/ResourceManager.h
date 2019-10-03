#pragma once

#include "GL/glew.h"

#include "../renderer/Shader.h"
#include "../renderer/Texture.h"

#include <map>
#include <string>

class ResourceManager
{
public:
	static std::map<std::string, Shader> Shaders;
	static std::map<std::string, Texture2D> Textures;

public:
	static Shader LoadShader(const char *vertexPath, const char* fragmentPath, const char *geometryPath, const std::string &name);
	static Shader &GetShader(const std::string &name);

	static Texture2D LoadTexture(const char *filepath, bool alpha, const std::string &name);
	static Texture2D &GetTexture(const std::string &name);

	static void Clear();
	
private:
	ResourceManager();

	static Shader LoadShaderFromFile(const char *vertexPath, const char* fragmentPath, const char *geometryPath = nullptr);
	static Texture2D LoadTextureFromFile(const char *filepath, bool alpha);
};