#include <Image/Image32.hpp>
#include <stb_image.h>

#include <fstream>

Image32::Image32(const char* path)
{
	int channelCount;
	int width, height;
	m_data = reinterpret_cast<uint32_t*>(stbi_load(path, &width, &height, &channelCount, STBI_rgb_alpha));
	m_width = width;
	m_height = height;
}

Image32::Image32(size_t width, size_t height)
	: m_width(width)
	, m_height(height)
	, m_data(reinterpret_cast<uint32_t*>(::operator new(width * height * sizeof(uint32_t))))
{}

void Image32::set(size_t x, size_t y, Color32 color)
{
	*reinterpret_cast<Color32*>(&m_data[y * m_width + x]) = color;
}

Color32 Image32::get(size_t x, size_t y) const
{
	return *reinterpret_cast<Color32*>(&m_data[y * m_width + x]);
}

size_t Image32::width() const
{
	return m_width;
}

size_t Image32::height() const
{
	return m_height;
}

uint32_t* Image32::data()
{
	return m_data;
}

const uint32_t* Image32::data() const
{
	return m_data;
}

void Image32::saveAsPpm(std::string_view path) const
{
	std::ofstream file(std::string(path), std::ios_base::binary);
	file << "P6 " << m_width << ' ' << m_height << " 255\n";
	for (uint32_t* ptr = m_data; ptr != m_data + (m_width * m_height); ptr++)
	{
		file.write(reinterpret_cast<const char*>(ptr), 3);
	}
}

