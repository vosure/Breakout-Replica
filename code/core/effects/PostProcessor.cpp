#include "PostProcessor.h"

PostProcessor::PostProcessor(Shader shader, unsigned int width, unsigned int height)
	: PostProcessorShader(shader), Texture(), Width(width), Height(height),
	Confuse(false), Chaos(false), Shake(false)
{
	glGenFramebuffers(1, &MSFBO);
	glGenFramebuffers(1, &FBO);
	glGenRenderbuffers(1, &RBO);

	glBindFramebuffer(GL_FRAMEBUFFER, MSFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_RGB, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, RBO);

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	Texture.Generate(width, height, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Texture.ID, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Init();

	PostProcessorShader.SetInt("scene", 0, true);

	float offset = 1.0f / 300.0f;
	float offsets[9][2] =
	{
		{ -offset,  offset  },  // top-left
		{  0.0f,    offset  },  // top-center
		{  offset,  offset  },  // top-right
		{ -offset,  0.0f    },  // center-left
		{  0.0f,    0.0f    },  // center-center
		{  offset,  0.0f    },  // center - right
		{ -offset, -offset  },  // bottom-left
		{  0.0f,   -offset  },  // bottom-center
		{  offset, -offset  }   // bottom-right  
	};
	glUniform2fv(glGetUniformLocation(PostProcessorShader.ID, "offsets"), 9, (float *)offsets);

	int edge_kernel[9] =
	{
		-1, -1, -1,
		-1,  8, -1,
		-1, -1, -1
	};
	glUniform1iv(glGetUniformLocation(PostProcessorShader.ID, "edge_kernel"), 9, edge_kernel);

	float blur_kernel[9] =
	{
		1.0 / 16, 2.0 / 16, 1.0 / 16,
		2.0 / 16, 4.0 / 16, 2.0 / 16,
		1.0 / 16, 2.0 / 16, 1.0 / 16
	};
	glUniform1fv(glGetUniformLocation(this->PostProcessorShader.ID, "blur_kernel"), 9, blur_kernel);
}

void PostProcessor::BeginRender()
{
	glBindFramebuffer(GL_FRAMEBUFFER, MSFBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void PostProcessor::EndRender()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, MSFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
	glBlitFramebuffer(0, 0, Width, Height, 0, 0, Width, Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessor::Render(float deltaTime)
{
	PostProcessorShader.Use();

	PostProcessorShader.SetFloat("time", deltaTime);
	PostProcessorShader.SetInt("confuse", Confuse);
	PostProcessorShader.SetInt("chaos", Chaos);
	PostProcessorShader.SetInt("shake", Shake);

	glActiveTexture(GL_TEXTURE0);

	Texture.Bind();
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void PostProcessor::Init()
{
	unsigned int VBO;

	float vertices[] =
	{
		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, 0.0f, 1.0f,

		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (void *)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
