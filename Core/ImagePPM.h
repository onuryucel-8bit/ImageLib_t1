#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdint>

struct ColorRGB
{
	uint16_t r = 0;
	uint16_t g = 0;
	uint16_t b = 0;
};

class ImagePPM
{
public:
	ImagePPM(size_t width, size_t height);
	~ImagePPM();

	void setPixel(size_t x, size_t y, ColorRGB color);

	void write(const std::string& path);
	std::string read(const std::string& path);

private:

	std::vector<ColorRGB> pixels;

	size_t m_width;
	size_t m_height;
};
