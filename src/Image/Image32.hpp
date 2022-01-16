#pragma once

#include <Image/Color32.hpp>

#include <string_view>

class Image32
{
public:
	Image32(const char* path);
	Image32(size_t width, size_t height);

public:
	void set(size_t x, size_t y, Color32 color);
	Color32 get(size_t x, size_t y) const;

	size_t width() const;
	size_t height() const;
	uint32_t* data();
	const uint32_t* data() const;

	void saveAsPpm(std::string_view path) const;

private:
	size_t m_width, m_height;
	uint32_t* m_data;
};