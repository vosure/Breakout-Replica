#include "Shader.h"

Shader::Shader()
{
}

Shader &Shader::Use()
{
	glUseProgram(ID);
	return *this;
}

void Shader::Compile(const char *vertexSource, const char *fragmentSource, const char *geometrySource)
{
	unsigned int vertex, fragment, geometry;

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexSource, NULL);
	glCompileShader(vertex);
	CheckCompileErrors(vertex, "VERTEX");

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentSource, NULL);
	glCompileShader(fragment);
	CheckCompileErrors(fragment, "FRAGMENT");

	if (geometrySource)
	{
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &geometrySource, NULL);
		glCompileShader(geometry);
		CheckCompileErrors(geometry, "GEOMETRY");
	}

	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	if (geometrySource)
	{
		glAttachShader(ID, geometry);
	}
	glLinkProgram(ID);
	CheckCompileErrors(ID, "PROGRAM");
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (geometrySource)
	{
		glDeleteShader(geometry);
	}
}

void Shader::SetFloat(const char *name, float value, bool useShader)
{
	if (useShader)
	{
		this->Use();
	}
	glUniform1f(glGetUniformLocation(ID, name), value);
}

void Shader::SetInt(const char *name, int value, bool useShader)
{
	if (useShader)
	{
		this->Use();
	}
	glUniform1i(glGetUniformLocation(ID, name), value);
}

void Shader::SetVec2(const char *name, const glm::vec2 &vector, bool useShader)
{
	if (useShader)
	{
		this->Use();
	}
	glUniform2f(glGetUniformLocation(ID, name), vector.x, vector.y);
}

void Shader::SetVec2(const char *name, float x, float y, bool useShader)
{
	if (useShader)
	{
		this->Use();
	}
	glUniform2f(glGetUniformLocation(ID, name), x, y);
}

void Shader::SetVec3(const char *name, const glm::vec3 &vector, bool useShader)
{
	if (useShader)
	{
		this->Use();
	}
	glUniform3f(glGetUniformLocation(ID, name), vector.x, vector.y, vector.z);
}

void Shader::SetVec3(const char *name, float x, float y, float z, bool useShader)
{
	if (useShader)
	{
		this->Use();
	}
	glUniform3f(glGetUniformLocation(ID, name), x, y, z);
}

void Shader::SetVec4(const char *name, const glm::vec4 &vector, bool useShader)
{
	if (useShader)
	{
		this->Use();
	}
	glUniform4f(glGetUniformLocation(ID, name), vector.x, vector.y, vector.z, vector.w);
}

void Shader::SetVec4(const char *name, float x, float y, float z, float w, bool useShader)
{
	if (useShader)
	{
		this->Use();
	}
	glUniform4f(glGetUniformLocation(ID, name), x, y, z, w);
}

void Shader::SetMat4(const char *name, const glm::mat4 &matrix, bool useShader)
{
	if (useShader)
	{
		this->Use();
	}
	glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::CheckCompileErrors(unsigned int object, const std::string &type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(object, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(object, sizeof(infoLog) / sizeof(char), NULL, infoLog);
			std::cout << "SHADER COMPILE TIME ERROR: " << type << std::endl;
		}
	}
	else
	{
		glGetProgramiv(object, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(object, sizeof(infoLog) / sizeof(char), NULL, infoLog);
			std::cout << "SHADER PROGRAM LINK TIME ERROR: " << type << std::endl;
		}
	}
}
