//vector assign - iterator
//https://stackoverflow.com/questions/4423361/constructing-a-vector-with-istream-iterators

#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "../utils/Vec2.hpp"
#include "../utils/Radix.h"
#include "../utils/tabulate/tabulate.hpp"

//#define DEBUG_PRINT_BMP_ALL

//#define DEBUG_PRINT_BMPinfoHeader
//#define DEBUG_PRINT_BMPheader
//#define DEBUG_PRINT_BUFFER_SIZE
//#define DEBUG_PRINT_CREATE_HEADER_SIZE

#ifdef DEBUG_PRINT_BMP_ALL
	#define DEBUG_PRINT_BMPinfoHeader
	#define DEBUG_PRINT_BMPheader
	#define DEBUG_PRINT_BUFFER_SIZE
	#define DEBUG_PRINT_CREATE_HEADER_SIZE
#endif // DEBUG_PRINT_BMP_ALL

//for holding image x = width and y = height
using vec32bit = sml::Vector2<uint32_t>;

//14 bytes 112 bits
#pragma pack(push, 1)
typedef struct BMP_Header
{
	uint16_t bitmapSignature_Bytes = 0;//2byte 'BM'
	uint32_t FileSize = 0;//4byte

	//4byte wiki => app specific
	uint32_t reservedBytes = 0;//4byte
	
	//Offset from beginning of file to the beginning of the bitmap data
	uint32_t pixelDataOffset = 0; //4byte start point of the image data

}bmpHeader;
#pragma pack(pop)

//40 bytes 320 bit
typedef struct BMP_InfoHeader
{
	uint32_t sizeOfInfoHeader = 40; //4byte

	uint32_t width = 0; //4bytes
	uint32_t height = 0;//4bytes

	uint16_t numberOfColorPlanes = 1;//2byte

	uint16_t bitsPerPixel = 0;//2byte

	//0 no Compression , 1 = 8bit RLE , 2 = 4bit RLE
	uint32_t compressionMethod = 0;//4byte

	//(compressed) Size of Image 
	//It is valid to set this = 0 if Compression = 0
	uint32_t imageSize = 0;//4byte

	//XpixelsPerM
	int32_t horizantalResolution = 0;//4byte

	//YpixelsPerM
	int32_t verticalResolution = 0;//4byte

	uint32_t colorsUsed = 0;//4byte

	uint32_t importantColors = 0;//4byte

}bmpInfoHeader;

//3 bytes 24bit

struct Color
{
	uint8_t b = 0;
	uint8_t g = 0;
	uint8_t r = 0;

	Color()
	{

	}
	//for easy use
	Color(uint8_t r_, uint8_t g_, uint8_t b_)
	{
		r = r_;
		g = g_;
		b = b_;
	}


	static const Color Red;
	static const Color Blue;
	static const Color Green;
	static const Color White;
	static const Color Black;
	static const Color TestColor;
};


const Color Color::Red = Color{ 0xff, 0,0 };
const Color Color::Blue = Color{ 0, 0, 0xff };
const Color Color::Green = Color{ 0, 0xff, 0 };
const Color Color::White = Color{ 0xff, 0xff, 0xff };
const Color Color::Black = Color{ 0, 0, 0 };
const Color Color::TestColor = Color{ 0xff,0xff,0x0 };

class ImageBMP
{
public:
	ImageBMP();
	~ImageBMP();

	void load(std::string path);
	void create(std::string fileName, vec32bit size, std::vector<Color>& pixels);
	void createStandart(std::string fileName, vec32bit size);
	void save();

	void rotate(int base, float radian);


	std::vector<uint8_t> getSubPart(vec32bit pos, vec32bit size);
	void setSubPart(vec32bit pos, vec32bit size, std::vector<uint8_t>& data);

	void setPixel(vec32bit pos, Color color);
	Color getPixel(vec32bit pos);

	void DEBUG_printRawData();
	void DEBUG_printDataTable();

private:
	std::vector<char> m_buffer;
	uint32_t m_width; //4bytes
	uint32_t m_height;//4bytes
};

ImageBMP::ImageBMP()
{
	m_width = 0;
	m_height = 0;
}

ImageBMP::~ImageBMP()
{

}

void ImageBMP::load(std::string path)
{
	std::cout << "#! loading bmp path = " << path << "\n";
	std::ifstream image(path, std::ios::binary);

	if (!image)
	{
		std::cerr << "ERROR :: ImageBMP.load() path is invalid\n";
		return;
	}

	//read headers
	bmpHeader header;
	image.read((char*)&header, sizeof(bmpHeader));

	bmpInfoHeader infoHeader;
	image.read((char*)&infoHeader, sizeof(bmpInfoHeader));

	m_width = infoHeader.width;
	m_height = infoHeader.height;
	
	//check image format 
	if (header.bitmapSignature_Bytes != 0x4D42)
	{
		std::cerr << "ERROR :: ImageBMP.load() :: check image format .bmp\n";
		return;
	}

	//set file cursor to data start point
	image.seekg(header.pixelDataOffset, std::ios::beg);

	//calc padding
	uint32_t paddingAmaount = (4 - infoHeader.width % 4) % 4;

	//calc row size
	uint32_t rowDataSize = infoHeader.width * 3;

	//resize the buffer for data(without padding)
	m_buffer.resize(infoHeader.width * infoHeader.height * 3);

	size_t offset = 0;
	for (size_t i = 0; i < infoHeader.width; i++)
	{
		//read row
		image.read(m_buffer.data() + offset, rowDataSize);
		//move to next part of data (if padding exist skip it)
		image.seekg(paddingAmaount, std::ios::cur);
		offset += rowDataSize;
	}
	
	image.close();
	
	//DEBUG_printRawData();
	DEBUG_printDataTable();

#ifdef DEBUG_PRINT_BUFFER_SIZE
	std::cout << "m_buffer.size(): " << m_buffer.size() << ": width * height: "<< infoHeader.width * infoHeader.height << "\n";
#endif // DEBUG_PRINT_BUFFER_SIZE

#ifdef DEBUG_PRINT_BMPinfoHeader
	std::cout << "------------------------------------\n"
		<< "DEBUG_PRINT BmpInfoHeader\n"
		<< "width " << infoHeader.width << "\n"
		<< "height " << infoHeader.height << "\n"
		<< "colorUsed " << infoHeader.colorsUsed << "\n"
		<< "------------------------------------\n";


#endif // DEBUG_PRINT_BMP_INFO_HEADER

#ifdef DEBUG_PRINT_BMPheader

	std::cout << "------------------------------------\n"
		<< "DEBUG_PRINT BmpHeader: \n"
		<< "signature :" << header.bitmapSignature_Bytes << "\n"
		<< "fileSize :" << header.FileSize << "\n"
		<< "reserved 0 :" << header.reservedBytes_0 << "\n"
		<< "reserved 1:" << header.reservedBytes_1 << "\n"
		<< "pixelDataOffset :" << header.pixelDataOffset << "\n"
		<< "------------------------------------\n";
#endif // DEBUG_PRINT_BMP_HEADER
	
}

void ImageBMP::create(std::string fileName, vec32bit size, std::vector<Color>& pixels)
{
	std::cout << "#! creating bmp filename = " << fileName << "\n";

	if (size.x * size.y != pixels.size())
	{
		std::cerr << "ERROR :: ImageBMP::create() width*height != given pixel vector size\n";
		return;
	}

	//----calc padding----

	//calc padding
	uint32_t paddingAmaount = (4 - (size.x * 3) % 4) % 4;

	//calc row size
	uint32_t rowDataSize = size.x * 3;

	std::cout << "TEST paddingAmount" << paddingAmaount << " rowDataSize: " << rowDataSize << "\n";

	//------------------------------------------//
	bmpHeader header;
	header.bitmapSignature_Bytes = 0x4D42;//bm	
	header.pixelDataOffset = 0x36;//data start point
	
	bmpInfoHeader infoHeader;	
	infoHeader.width = size.x;
	infoHeader.height = size.y;

	infoHeader.bitsPerPixel = 24; //0x18 => 24bit
	//------------------------------------------//


	//output file
	std::ofstream file(fileName, std::ios::binary);

	file.write((char*)&header, sizeof(bmpHeader));
	file.write((char*)&infoHeader, sizeof(infoHeader));
	//pixels.size() amount of pixels * color size 3 byte = 16*3 = 48byte
	


	//write pixels + padding to file
	size_t offset = 0;
	for (size_t i = 0; i < size.y; i++)
	{
		//write data row by row
		file.write((char*)(pixels.data() + offset), rowDataSize);
		
		//write padding if theres a padding
		if (paddingAmaount > 0)
		{
			file.write((char*)0, paddingAmaount);
		}
		
		//calc offset
		offset += size.x;
	}

	std::cout << "TEST pixels size = " << pixels.size() << " pixels.size() * sizeof Color = " << pixels.size() * sizeof(Color) << "\n";

	file.close();


#ifdef DEBUG_PRINT_CREATE_HEADER_SIZE
	std::cout << "-----Size of headers-----\n"
		<< "sizeof infoheader " << std::hex << sizeof(infoHeader) << "\n"
		<< "sizeof header " << sizeof(header) << "\n"
		<< "--------------------------------\n";
#endif // DEBUG_PRINT_CREATE_HEADER_SIZE
}

void ImageBMP::createStandart(std::string fileName, vec32bit size)
{
	if (size.x <= 0 || size.y <= 0)
	{
		std::cerr << "ERROR::ImageBMP::createStandart() :: check image width height for <= 0\n";
		return;
	}

	std::vector<Color> pixels;
	for (size_t i = 0; i < size.x * size.y; i++)
	{
		pixels.push_back(Color::Green);
	}

	create(fileName, size, pixels);
}

void ImageBMP::DEBUG_printRawData()
{
	//print the data as hex
	//& 0xff for wrapping .. -1 & 0xff => 0xff
	for (size_t i = 0; i < m_buffer.size(); i++)
	{
		std::cout << std::hex << (static_cast<int>(m_buffer[i]) & 0xff) << " : ";

		if ((i + 1) % 3 == 0)
		{
			std::cout << "\n";
		}

	}
	std::cout << std::dec << "\n";
	
}

void ImageBMP::DEBUG_printDataTable()
{
	tabulate::Table table;

	std::vector<std::string> row;
	for (size_t i = 0; i < m_width * 3; ++i) 
	{
		row.push_back(std::to_string(i));
	}
	table.add_row(tabulate::Table::Row_t(row.begin(), row.end()));

	
	row.clear();
	
	size_t rowCounter = 0;
	for (char val : m_buffer) {
		row.push_back(rdx::dec_toHex(val & 0xff));
		rowCounter++;

		if (rowCounter >= m_width * 3)
		{
			table.add_row(tabulate::Table::Row_t(row.begin(), row.end()));
			row.clear();
			rowCounter = 0;
		}
		
	}

	std::cout << table << std::endl;
}

#pragma region TODO_utils

void ImageBMP::save()
{
}

void ImageBMP::rotate(int base, float radian)
{

}

std::vector<uint8_t> ImageBMP::getSubPart(vec32bit pos, vec32bit size)
{
	std::vector<uint8_t> data;

	return data;
}

void ImageBMP::setSubPart(vec32bit pos, vec32bit size, std::vector<uint8_t>& data)
{
}

void ImageBMP::setPixel(vec32bit pos, Color color)
{
}

Color ImageBMP::getPixel(vec32bit pos)
{
	return Color();
}



#pragma endregion