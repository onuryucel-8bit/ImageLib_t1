//----------------------------------------------------------------//
//Source
//https://www.youtube.com/watch?v=kpHFFFu9qeU
//https://www.youtube.com/watch?v=CmQoJEOeUT8
//https://www.youtube.com/watch?v=HaGySs2qByc 
//https://www.youtube.com/watch?v=13E0il2zxBA //Making .BMP images from scratch
// 
//https://dev.to/muiz6/c-how-to-write-a-bitmap-image-from-scratch-1k6m
//https://engineering.purdue.edu/ece264/19sp/hw/HW11#:~:text=little-endian,is%20a%20little%20endian%20format.
//BMP file format:
//microsoft 
//https://learn.microsoft.com/tr-tr/windows/win32/api/wingdi/ns-wingdi-bitmapinfoheader?redirectedfrom=MSDN
//https://www.ece.ualberta.ca/~elliott/ee552/studentAppNotes/2003_w/misc/bmp_file_format/bmp_file_format.htm //bmp format table
//https://en.wikipedia.org/wiki/BMP_file_format#cite_note-Image_Stride-18
//https://www-user.tu-chemnitz.de/~heha/hs/chm/petzold.chm/petzoldi/ch15b.htm //The DIB File Format
//https://cplusplus.com/articles/GwvU7k9E/ cpp forum 
//https://solarianprogrammer.com/2018/11/19/cpp-reading-writing-bmp-images/ solar 
// 
//STB
//https://solarianprogrammer.com/2019/06/10/c-programming-reading-writing-images-stb_image-libraries/ stb_image 
// 
// aligment [struct/bmp?]
// https://stackoverflow.com/questions/119123/why-isnt-sizeof-for-a-struct-equal-to-the-sum-of-sizeof-of-each-member
// https://en.wikipedia.org/wiki/Data_structure_alignment
//----------------------------------------------------------------//

//tool
//https://github.com/WerWolv/ImHex


#include <iostream>
#include <string>

#include "Core/ImageBMP.h"
#include "utils/tabulate/tabulate.hpp"

void testBmpRW()
{
	ImageBMP bmp;

	std::string path = "input_24bit.bmp";
	std::string outPath = "outputbit_t1.bmp";

	bmp.load(path);


	//-----------------create bmp-------------------//
	std::vector<Color> data =
	{

		Color::Red,
		Color::Red,
		Color::Red,
		Color::Red,

		Color::Blue,
		Color::Blue,
		Color::Red,
		Color::Red,

		Color::Green,
		Color::Green,
		Color::Green,
		Color::Blue,
		
	};

	std::vector<Color> rnddata;
	

	std::srand(std::time(0));

	uint32_t width = 100;
	uint32_t height = 100;

	for (size_t i = 0; i < width * height; i++)
	{
		int color = rand() % 100;
		if (color < 40)
		{
			rnddata.push_back(Color::Red);
		}
		else if (color < 50)
		{
			rnddata.push_back(Color::Green);
		}
		else if (color < 80)
		{
			rnddata.push_back(Color::Blue);
		}
		else if (color < 90)
		{
			rnddata.push_back(Color::Black);
		}
		else if (color < 102)
		{
			rnddata.push_back(Color::White);
		}
		

		
	}

	bmp.create(outPath, vec32bit(width,height), rnddata);

}

int main()
{
	testBmpRW();

	return 0;
}
