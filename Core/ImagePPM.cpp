#include "ImagePPM.h"


ImagePPM::ImagePPM(size_t width, size_t height)
{
	m_width = width;
	m_height = height;

	pixels.resize(width * height, ColorRGB{ 255, 0, 0 });
}

ImagePPM::~ImagePPM()
{
}

void ImagePPM::setPixel(size_t x, size_t y, ColorRGB color)
{
	pixels[x * m_width + y] = color;
}

void ImagePPM::write(const std::string& path)
{
	std::ofstream file(path);

	file << "P3\n" << m_width << " " << m_height << "\n255\n";

	for (size_t i = 0; i < m_width * m_height; i++)
	{
		file << pixels[i].r << " "
			<< pixels[i].g << " "
			<< pixels[i].b << " ";

		if ((i + 1) % m_width == 0)
		{
			file << "\n";
		}
	}

	file.close();
}

std::string ImagePPM::read(const std::string& path)
{
	std::fstream file(path);


	if (!file.is_open())
	{
		std::cout << "ERROR:: couldnt open the file\n";
		return "";
	}

	std::stringstream ss;

	ss << file.rdbuf();

	return ss.str();

	file.close();
}