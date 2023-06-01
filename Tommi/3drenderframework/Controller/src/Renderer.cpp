#include "Renderer.h"
#include <iostream>

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR)
	{

	}
}

bool GLLogCall(const char* function, const char* file, int line)
{
	bool check = true;
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ":" << line << std::endl;
		check = false;
	}
	return check;
}

void Renderer::clear() const
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.54, 0.97, 1, 1);
}

void Renderer::draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
	shader.Bind();
	va.Bind();
	ib.Bind();
	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::draw(const VertexArray& va, const Shader& shader, unsigned int count) const
{
	shader.Bind();
	va.Bind();
	//GLCall(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr));
	GLCall(glDrawArrays(GL_TRIANGLES, 0, count));
}
