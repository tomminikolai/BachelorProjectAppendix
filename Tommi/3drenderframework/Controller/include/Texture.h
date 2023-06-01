#pragma once

#include "Renderer.h"

class Texture
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;
	bool freed;
public:
	Texture(){}
	Texture(const std::string& path, unsigned int slot = 0);
	Texture(unsigned long long width, unsigned long long height, unsigned char* color);
	~Texture();

	void emplace(unsigned int x, unsigned int y, unsigned char* color, bool update = false);
	void updatebuffer();
	void draw_square(unsigned int llcx, unsigned int llcy, unsigned int rucx, unsigned int rucy, unsigned char* color, bool update = false);
	void draw_line(unsigned int startx, unsigned int starty, unsigned int stopx, unsigned int stopy, unsigned int thickness, unsigned char* color, bool update = false);
	void draw_circle(int x, int y, float radius, unsigned char* color, bool update = false);
	void clear(unsigned char* color, bool update = false);

	void Bind(unsigned int slot = 0) const;
	void UnBind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
};