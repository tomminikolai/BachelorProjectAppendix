#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Renderer.h"
#include "Texture.h"
#include "Shader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class model
{
public:
	model(const char* filepath, const glm::vec3& startpos, const std::string& shaderpath, const glm::vec3& offset = glm::vec3(0.0, 0.0, 0.0), const glm::vec3& startrot = glm::vec3(0.0, 0.0, 0.0), const glm::vec3& offsetrot = glm::vec3(0, 0, 0));
	~model();
	void move_pos(const glm::vec3& newpos, const glm::vec3& newrot);
	void draw(const glm::mat4& projection, const glm::mat4& view, const glm::vec3& lightpos);
private:
	VertexBufferLayout m_vbl;
	VertexArray m_va;
	VertexBuffer* m_vb;
	Shader m_shader;
	glm::vec3 m_pos;
	glm::vec3 m_offset;
	glm::vec3 m_posrot;
	glm::vec3 m_offsetrot;
	glm::mat4 m_model;
	Renderer m_renderer;
	unsigned int m_count;
};