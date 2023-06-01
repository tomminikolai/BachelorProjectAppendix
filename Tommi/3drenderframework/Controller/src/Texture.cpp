#include <memory>
#include "Texture.h"
#include "stb_image/stb_image.h"

Texture::Texture(const std::string& path, unsigned int slot)
	:m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0), freed(false)
{
	stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);
	glGenTextures(1, &m_RendererID);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
	glBindTexture(GL_TEXTURE_2D, slot);
	if (m_LocalBuffer)
	{
		stbi_image_free(m_LocalBuffer);
		freed = true;
	}
}

Texture::Texture(unsigned long long width, unsigned long long height, unsigned char* color)
	:m_RendererID(), m_LocalBuffer(nullptr), m_Width(width), m_Height(height), m_BPP(4), freed(false)
{
	m_LocalBuffer = (unsigned char*)malloc(m_Width * m_Height * m_BPP * sizeof(unsigned char));
	for (unsigned long long i = 0; i < m_Width * m_Height; i++)
	{
		for (unsigned long long j = 0; j < m_BPP; j++)
		{
			m_LocalBuffer[i * m_BPP + j] = color[j];
		}

	}
	glGenTextures(1, &m_RendererID);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_RendererID);
	if (!freed)
	{
		free(m_LocalBuffer);
	}
}

void Texture::emplace(unsigned int x, unsigned int y, unsigned char* color, bool update)
{
	unsigned long long pos = (y * (unsigned int)m_Width + x) * m_BPP;
	for (unsigned long long j = 0; j < m_BPP; j++)
	{
		m_LocalBuffer[pos + j] = color[j];
	}
	if (update)
	{
		updatebuffer();
	}
}

void Texture::updatebuffer()
{
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::draw_square(unsigned int llcx, unsigned int llcy, unsigned int rucx, unsigned int rucy, unsigned char* color, bool update)
{
	unsigned int width = rucx - llcx;
	unsigned int height = rucy - llcy;
	unsigned long long startpos = (llcy * (unsigned int)m_Width + llcx) * m_BPP;
	for (unsigned int i = 0; i < height; i++)
	{
		for (unsigned int j = 0; j < width; j++)
		{
			for (unsigned int k = 0; k < m_BPP; k++)
			{
				unsigned long long pos = startpos + i * m_Width * m_BPP + m_BPP * j + k;
				if (pos < m_Width * m_Height * m_BPP)
				{
					m_LocalBuffer[pos] = color[k];
				}
				else
				{
					if (update)
					{
						updatebuffer();
						return;
					}
				}
			}
		}
	}
	if (update)
	{
		updatebuffer();
	}
}

void Texture::draw_line(unsigned int startx, unsigned int starty, unsigned int stopx, unsigned int stopy, unsigned int thickness, unsigned char* color, bool update)
{
	float dx = (int)stopx - (int)startx;
	float dy = (int)stopy - (int)starty;
	float a = (float)(dy) / (float)(dx);
	int b = starty - a * startx;
	unsigned int points = sqrtf((dx * dx) + (dy * dy));
	float dx2 = dx / (float)points;
	draw_circle(startx, starty, thickness , color);
	draw_circle(stopx, stopy, thickness, color);
	for (unsigned int j = 0; j < thickness; j++)
	{
		for (unsigned int i = 0; i < points; i++)
		{
			float x = (float)startx + dx2 * float(i);
			emplace(x, (a * x + b) + j - (thickness / 2), color, false);
		}
	}
	a = (float)(dx) / (float)(dy);
	b = startx - a * starty;
	dx2 = dy / (float)points;
	for (unsigned int j = 0; j < thickness; j++)
	{
		for (unsigned int i = 0; i < points; i++)
		{
			float y = (float)starty + dx2 * float(i);
			emplace((a * y + b) + j - (thickness / 2), y , color, false);
		}
	}
	if (update)
	{
		updatebuffer();
	}
}

void Texture::draw_circle(int x, int y, float radius, unsigned char* color, bool update)
{
	int width = radius * 2;
	int height = radius * 2;
	int startx = x - radius;
	int starty = y - radius;
	for (int xloc = -radius; xloc < radius; xloc++)
	{
		;
		for (int yloc = -radius; yloc < radius; yloc++)
		{
			if (sqrtf((float)(xloc * xloc) + (float)(yloc * yloc)) < radius)
			{
				emplace(x + xloc, y + yloc, color, false);
			}
		}
	}
	if (update)
	{
		updatebuffer();
	}
}

void Texture::clear(unsigned char* color, bool update)
{
	for (unsigned long long i = 0; i < m_Width * m_Height; i++)
	{
		for (unsigned long long j = 0; j < m_BPP; j++)
		{
			m_LocalBuffer[i * m_BPP + j] = color[j];
		}
	}
	if (update)
	{
		updatebuffer();
	}
}

void Texture::Bind(unsigned int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Texture::UnBind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
