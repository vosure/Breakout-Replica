#pragma once

#include "ResourceManager.h"

#include <stb_image.h>

#include <iostream>
#include <sstream>
#include <fstream>

std::map<std::string, Texture2D>    ResourceManager::Textures;
std::map<std::string, Shader>       ResourceManager::Shaders;

ResourceManager::ResourceManager()
{
}

Shader ResourceManager::LoadShader(const char *vertexPath, const char *fragmentPath, const char *geometryPath, const std::string &name)
{
	Shaders[name] = LoadShaderFromFile(vertexPath, fragmentPath, geometryPath);
	return Shaders[name];
}

Shader ResourceManager::GetShader(const std::string &name)
{
	return Shaders[name];
}

Texture2D ResourceManager::LoadTexture(const char *filepath, bool alpha, const std::string &name)
{
	Textures[name] = LoadTextureFromFile(filepath, alpha);
	return Textures[name];
}

Texture2D ResourceManager::GetTexture(const std::string &name)
{
	return Textures[name];
}

void ResourceManager::Clear()
{
	for (auto element : Shaders)
		glDeleteProgram(element.second.ID);

	for (auto element : Textures)
		glDeleteTextures(1, &element.second.ID);
}

Shader ResourceManager::LoadShaderFromFile(const char *vertexPath, const char *fragmentPath, const char *geometryPath)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	try
	{
		std::ifstream vertexFile(vertexPath);
		std::ifstream fragmentFile(fragmentPath);

		std::stringstream vertexStream, fragmentStream;
		
		vertexStream << vertexFile.rdbuf();
		fragmentStream << fragmentFile.rdbuf();
		
		vertexFile.close();
		fragmentFile.close();
	
		vertexCode = vertexStream.str();
		fragmentCode = fragmentStream.str();

		if (geometryPath != nullptr)
		{
			std::ifstream geometryShaderFile(geometryPath);
			std::stringstream geometryStream;
			geometryStream << geometryShaderFile.rdbuf();
			geometryShaderFile.close();
			geometryCode = geometryStream.str();
		}
	}
	catch (std::exception e)
	{
		std::cout << "FAILED TO LOAD SHADER FORM FILE" << std::endl;
	}

	Shader shader;
	shader.Compile(vertexCode.c_str(), fragmentCode.c_str(),geometryPath != nullptr ? geometryCode.c_str() : nullptr);
	return shader;
}

Texture2D ResourceManager::LoadTextureFromFile(const char *filepath, bool alpha)
{
	Texture2D texture;
	if (alpha)
	{
		texture.InternalFormat = GL_RGBA;
		texture.SourceFormat = GL_RGBA;
	}

	int width, height, channels;
	unsigned char* image = stbi_load(filepath, &width, &height, &channels, 0);
	// Now generate texture
	texture.Generate(width, height, image);
	// And finally free image data
	stbi_image_free(image);
	return texture;
}
