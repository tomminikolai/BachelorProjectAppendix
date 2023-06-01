#pragma once

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

struct shaderprogsource
{
	std::string vertsource, fragsource;
};

class Shader
{
private:
	std::string m_FilePath;
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;
public:
	Shader(){}
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void UnBind() const;
	void SetUniform1i(const std::string& name, int val);
	void SetUniformVec3f(const std::string& name, const glm::vec3& vec);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
private:
	int GetUniformLocation(const std::string& name);
	shaderprogsource parseshader(const std::string& filepath);
	unsigned int compileshader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
};

