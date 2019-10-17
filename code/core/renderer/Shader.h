#pragma once

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <string>
#include <iostream>

class Shader
{
public:
	unsigned int ID;

public:
	Shader();

	//NOTE(vosure): Actually, most of them sould be const!
	Shader &Use();
	void Compile(const char *vertexSource, const char *fragmentSource, const char*geometrySource = nullptr);

	void SetFloat(const char *name, float value, bool useShader = false);
	void SetInt(const char *name, int value, bool useShader = false);

	void SetVec2(const char *name, const glm::vec2 &vector, bool useShader = false);
	void SetVec2(const char *name, float x, float y, bool useShader = false);
	void SetVec3(const char *name, const glm::vec3 &vector, bool useShader = false);
	void SetVec3(const char *name, float x, float y, float z, bool useShader = false);
	void SetVec4(const char *name, const glm::vec4 &vector, bool useShader = false);
	void SetVec4(const char *name, float x, float y, float z, float w, bool useShader = false);

	void SetMat4(const char *name, const glm::mat4 &matrix, bool useShader = false);

private:
	void CheckCompileErrors(unsigned int object, const std::string &type);
	//TODO(vosure): Uniform Location Cache
};