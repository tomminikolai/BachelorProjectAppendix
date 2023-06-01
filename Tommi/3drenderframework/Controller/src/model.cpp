#include "..\include\model.h"

model::model(const char* filepath, const glm::vec3& startpos, const std::string& shaderpath, const glm::vec3& offset, const glm::vec3& startrot, const glm::vec3& offsetrot)
	:m_pos(startpos), m_shader(shaderpath), m_offset(offset), m_posrot(startrot), m_offsetrot(offsetrot)
{
	std::string folderpath("");
	if (std::string(filepath).find('/') != std::string::npos)
	{
		folderpath = std::string(filepath).substr(0, std::string(filepath).find('/') + 1);
	}
	float* xd;
	std::ifstream ifs;
	ifs.open(filepath, std::ios_base::in);
	unsigned long long countv = 0;
	unsigned long long countvn = 0;
	unsigned long long countvt = 0;
	unsigned long long countf = 0;
	char msg[3];
	for (unsigned int i = 0; i < 3; i++)
	{
		msg[i] = ifs.get();
	}
	while (ifs.peek() != EOF)
	{
		if (msg[0] == 'v' && msg[1] == ' ' && msg[2] == ' ')
		{
			countv++;
		}
		if (msg[0] == 'v' && msg[1] == 'n' && msg[2] == ' ')
		{
			countvn++;
		}
		if (msg[0] == 'v' && msg[1] == 't' && msg[2] == ' ')
		{
			countvt++;
		}
		if (msg[0] == 'f' && msg[1] == ' ')
		{
			countf++;
		}
		msg[0] = msg[1];
		msg[1] = msg[2];
		msg[2] = ifs.get();
	}
	m_count = countf * 3;
	ifs.close();
	ifs.open(filepath, std::ios_base::in);
	float* vertices = nullptr;
	if (countv)
	{
		m_vbl.Push<float>(3);
		vertices = (float*)malloc(countv * 3 * sizeof(float));
	}
	float* vertex_normals = nullptr;
	if (countvn)
	{
		m_vbl.Push<float>(3);
		vertex_normals = (float*)malloc(countvn * 3 * sizeof(float));
	}
	float* vertex_tex_coords = nullptr;
	if (countvt)
	{
		m_vbl.Push<float>(3);
		vertex_tex_coords = (float*)malloc(countvt * 3 * sizeof(float));
	}
	unsigned int facet_size = (countv > 0) + (countvn > 0) + (countvt > 0);
	std::string msgs;
	std::string filename;
	m_vbl.Push<float>(3);
	while (msgs != std::string("mtllib"))
	{
		ifs >> msgs;
	}
	ifs >> filename;
	filename = folderpath + filename;
	unsigned long long vcount = 0;
	unsigned long long vncount = 0;
	unsigned long long vtcount = 0;
	unsigned long long vbscount = (unsigned long long)3 * ((unsigned long long)((countv != 0) + (countvt != 0) + (countvn != 0)) * (unsigned long long)3 + 3);
	float* vertex_buffer = (float*)malloc(countf * vbscount * sizeof(float));
	unsigned long long vbcount = 0;

	float color[3] = { 0, 0, 0 };

	while (ifs.peek() != EOF)
	{
		ifs >> msgs;
		if (msgs == "v")
		{
			for (unsigned int i = 0; i < 3; i++)
			{
				ifs >> vertices[vcount + i];
			}
			vcount += 3;
		}
		else if (msgs == "vn")
		{
			for (unsigned int i = 0; i < 3; i++)
			{
				ifs >> vertex_normals[vncount + i];
			}
			vncount += 3;
		}
		else if (msgs == "vt")
		{
			for (unsigned int i = 0; i < 3; i++)
			{
				ifs >> vertex_tex_coords[vtcount + i];
			}
			vtcount += 3;
		}
		else if (msgs == "usemtl")
		{
			ifs >> msgs;
			std::ifstream matstream;
			matstream.open(filename);
			std::string test;
			while (test != msgs)
			{
				matstream >> test;
			}
			while (test != "Kd")
			{
				matstream >> test;
			}
			for (unsigned int i = 0; i < 3; i++)
			{
				matstream >> test;
				color[i] = std::strtof(test.c_str(), NULL);
			}
			matstream.close();
		}
		else if (msgs == "f")
		{
			msgs = "";
			float vert_temp[3];
			float vertn_temp[3];
			float vertt_temp[3];
			std::getline(ifs, msgs);
			msgs = msgs.substr(msgs.find(' ') + 1, msgs.length());
			for (unsigned int i = 0; i < 3; i++)
			{
				if (countv)
				{
					unsigned long long index;
					if (msgs.find('/' != std::string::npos))
					{
						index = std::strtoull((msgs.substr(0, msgs.find('/'))).c_str(), NULL, 10);
						msgs = msgs.substr(msgs.find('/') + 1, msgs.length());
					}
					else if (msgs.find(' ' != std::string::npos))
					{
						index = std::strtoull((msgs.substr(0, msgs.find(' '))).c_str(), NULL, 10);
						msgs = msgs.substr(msgs.find(' ') + 1, msgs.length());
					}
					else
					{
						index = std::strtoull(msgs.c_str(), NULL, 10);
					}
					for (unsigned int i = 0; i < 3; i++)
					{
						vertex_buffer[vbcount] = vertices[(index - 1) * 3 + i];
						vbcount++;
					}
				}

				if (countvt)
				{
					unsigned long long index;
					if (msgs.find('/' != std::string::npos))
					{
						index = std::strtoull((msgs.substr(0, msgs.find('/'))).c_str(), NULL, 10);
						msgs = msgs.substr(msgs.find('/') + 1, msgs.length());
					}
					else if (msgs.find(' ' != std::string::npos))
					{
						index = std::strtoull((msgs.substr(0, msgs.find(' '))).c_str(), NULL, 10);
						msgs = msgs.substr(msgs.find(' ') + 1, msgs.length());
					}
					else
					{
						index = std::strtoull(msgs.c_str(), NULL, 10);
					}
					for (unsigned int i = 0; i < 3; i++)
					{
						vertex_buffer[vbcount] = vertex_tex_coords[(index - 1) * 3 + i];
						vbcount++;
					}
				}

				if (countvn)
				{
					unsigned long long index;
					if (msgs.find('/' != std::string::npos))
					{
						index = std::strtoull((msgs.substr(0, msgs.find('/'))).c_str(), NULL, 10);
						msgs = msgs.substr(msgs.find('/') + 1, msgs.length());
					}
					else if (msgs.find(' ' != std::string::npos))
					{
						index = std::strtoull((msgs.substr(0, msgs.find(' '))).c_str(), NULL, 10);
						msgs = msgs.substr(msgs.find(' ') + 1, msgs.length());
					}
					else
					{
						index = std::strtoull(msgs.c_str(), NULL, 10);
					}
					for (unsigned int i = 0; i < 3; i++)
					{
						vertex_buffer[vbcount] = vertex_normals[(index - 1) * 3 + i];
						vbcount++;
					}
				}
				for (unsigned int i = 0; i < 3; i++)
				{
					vertex_buffer[vbcount] = color[i];
					vbcount++;
				}
			}
		}
	}
	m_vb = new VertexBuffer((const void*)vertex_buffer, countf * vbscount * sizeof(float));
	m_va.AddBuffer(*m_vb, m_vbl);
	for (unsigned int i = 0; i < 12; i++)
	{
		std::cout << vertex_buffer[i] << ", ";
	}
	ifs.close();
	m_shader.Bind();

	m_model = glm::translate(glm::mat4(1.0f), m_pos);
	m_model = glm::rotate(m_model, m_posrot.y, glm::vec3(0, 1, 0));
	m_model = glm::rotate(m_model, m_posrot.x, glm::vec3(1, 0, 0));
	m_model = glm::rotate(m_model, m_posrot.z, glm::vec3(0, 0, 1));
	m_model = glm::rotate(m_model, m_offsetrot.y, glm::vec3(0, 1, 0));
	m_model = glm::rotate(m_model, m_offsetrot.x, glm::vec3(1, 0, 0));
	m_model = glm::rotate(m_model, m_offsetrot.z, glm::vec3(0, 0, 1));
	m_model = glm::translate(m_model, m_offset);
}

model::~model()
{
	delete(m_vb);
}

void model::move_pos(const glm::vec3& newpos, const glm::vec3& newrot)
{
	m_pos = newpos;
	m_posrot = newrot;
	m_model = glm::translate(glm::mat4(1.0f), m_pos);
	m_model = glm::rotate(m_model, m_posrot.y, glm::vec3(0, 1, 0));
	m_model = glm::rotate(m_model, m_posrot.x, glm::vec3(1, 0, 0));
	m_model = glm::rotate(m_model, m_posrot.z, glm::vec3(0, 0, 1));
	m_model = glm::rotate(m_model, m_offsetrot.y, glm::vec3(0, 1, 0));
	m_model = glm::rotate(m_model, m_offsetrot.x, glm::vec3(1, 0, 0));
	m_model = glm::rotate(m_model, m_offsetrot.z, glm::vec3(0, 0, 1));
	m_model = glm::translate(m_model, m_offset);
}

void model::draw(const glm::mat4& projection, const glm::mat4& view, const glm::vec3& lightpos)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	m_shader.Bind();
	glm::mat4 mvp = projection * view * m_model;
	m_shader.SetUniform1i("u_Texture", 0);
	m_shader.SetUniformMat4f("u_MVP", mvp);
	m_shader.SetUniformMat4f("u_M", m_model);
	m_shader.SetUniformVec3f("u_lightpos", lightpos);
	m_renderer.draw(m_va, m_shader, m_count);
}
