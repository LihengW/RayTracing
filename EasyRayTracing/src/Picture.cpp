#include "Picture.h"
#include "vendor/stb_image_write.h"

Picture::Picture(const int& width, const int& height, const int& channel)
	:m_Width(width), m_Height(height), m_Channel(channel)
{
	data = new unsigned char[width * height * channel];
}
Picture::~Picture()
{
	delete data;
}

void Picture::modify(int x, int y, glm::vec3 rgb)
{
	modify(x, y, { rgb.r, rgb.g, rgb.b, 1.0f });
}

void Picture::modify(int x, int y, glm::vec4 rgba)
{
	modify(x, y, Utility::LineartoGamma(rgba.r)*255.0f, Utility::LineartoGamma(rgba.g) * 255.0f, Utility::LineartoGamma(rgba.b) * 255.0f, rgba.a*255);
}

void Picture::modify(int x, int y, unsigned int r, unsigned int g, unsigned int b, unsigned int alpha)
{
	data[m_Channel * (m_Width * (m_Height - y - 1) + x)] = (unsigned char)r;
	data[m_Channel * (m_Width * (m_Height - y - 1) + x) + 1] = (unsigned char)g;
	data[m_Channel * (m_Width * (m_Height - y - 1) + x) + 2] = (unsigned char)b;
	if (m_Channel == 4) data[m_Channel * (m_Width * (m_Height - y - 1) + x) + 3] = (unsigned char)alpha;
}

int Picture::output(const std::string& path)
{
	return stbi_write_png(path.c_str(), m_Width, m_Height, m_Channel, data, 0);
}

std::pair<unsigned int, unsigned int> Picture::size()
{
	return std::pair<unsigned int, unsigned int>(m_Width, m_Height);
}

