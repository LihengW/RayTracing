#pragma once
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define __STDC_LIB_EXT1__

#include <string>
#include "utility.h"
#include "vendor/glm/glm/glm.hpp"

class Picture
{
public:
	Picture(const int& width, const int& height, const int& channel);
	~Picture();

	void modify(int x, int y, glm::vec3 rgb);
	void modify(int x, int y, glm::vec4 rgba);
	void modify(int x, int y, unsigned int r, unsigned int g, unsigned int b, unsigned int alpha = 255);

	int output(const std::string& path);

private:
	unsigned int m_Width, m_Height, m_Channel;
	unsigned char* data;
	std::string path = "assets/output.png";
};