#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <GL\glew.h>
//#define GLFW_DLL
#include "GLFW\glfw3.h"

typedef struct
{
	unsigned char imageTypeCode;
	short int imageWidth;
	short int imageHeight;
	unsigned char bitCount;
	unsigned char* imageData;
} TGAFILE;

class Texture2D {
public:
	unsigned int texture_id;
	std::string* type;
	std::string* file_name;
	bool has_init_loaded = false;
	Texture2D(std::string* _file_name) {
		this->file_name = _file_name;
		init();
	}
	static std::string* read_image_file(std::string* file_name)
	{
		// TODO: read width,height,channels from image file
		std::ifstream t(file_name->c_str());
		std::stringstream buffer;
		buffer << t.rdbuf();

		std::string* result;
		if (t.good()) {
			result = new std::string(buffer.str());
		}
		else {
			result = nullptr;
		}
		
		return result;
	}

	bool LoadTGAFile(const char* filename, TGAFILE** tgaFile)
	{
		FILE* filePtr;
		unsigned char ucharBad;
		short int sintBad;
		long imageSize;
		int colorMode;
		unsigned char colorSwap;
		TGAFILE* result = *tgaFile;
		
		// Open the TGA file.
		filePtr = fopen(filename, "rb");
		if (filePtr == NULL)
		{
			return false;
		}

		// Read the two first bytes we don't need.
		fread(&ucharBad, sizeof(unsigned char), 1, filePtr);
		fread(&ucharBad, sizeof(unsigned char), 1, filePtr);

		// Which type of image gets stored in imageTypeCode.
		fread(&result->imageTypeCode, sizeof(unsigned char), 1, filePtr);

		// For our purposes, the type code should be 2 (uncompressed RGB image)
		// or 3 (uncompressed black-and-white images).
		if (result->imageTypeCode != 2 && result->imageTypeCode != 3 && result->imageTypeCode != 10)
		{
			fclose(filePtr);
			return false;
		}

		// Read 13 bytes of data we don't need.
		fread(&sintBad, sizeof(short int), 1, filePtr);
		fread(&sintBad, sizeof(short int), 1, filePtr);
		fread(&ucharBad, sizeof(unsigned char), 1, filePtr);
		fread(&sintBad, sizeof(short int), 1, filePtr);
		fread(&sintBad, sizeof(short int), 1, filePtr);

		// Read the image's width and height.
		fread(&result->imageWidth, sizeof(short int), 1, filePtr);
		fread(&result->imageHeight, sizeof(short int), 1, filePtr);

		// Read the bit depth.
		fread(&result->bitCount, sizeof(unsigned char), 1, filePtr);

		// Read one byte of data we don't need.
		fread(&ucharBad, sizeof(unsigned char), 1, filePtr);

		// Color mode -> 3 = BGR, 4 = BGRA.
		colorMode = result->bitCount / 8;
		imageSize = result->imageWidth * result->imageHeight * colorMode;

		// Allocate memory for the image data.
		//result->imageData = (unsigned char*)malloc(sizeof(unsigned char) * imageSize);

		result->imageData = new unsigned char[sizeof(unsigned char) * imageSize];

		// Read the image data.
		fread(result->imageData, sizeof(unsigned char), imageSize, filePtr);

		// Change from BGR to RGB so OpenGL can read the image data.
		for (int imageIdx = 0; imageIdx < imageSize; imageIdx += colorMode)
		{
			colorSwap = result->imageData[imageIdx];
			result->imageData[imageIdx] = result->imageData[imageIdx + 2];
			result->imageData[imageIdx + 2] = colorSwap;
		}

		(*tgaFile) = result;

		fclose(filePtr);

		return true;
	}
	void init() 
	{
		int width = 1024; 
		int height = 1024;
		int nrChannels = 3;
		//unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
		//std::string* data_str = read_image_file(this->file_name);

		TGAFILE* tga_file = new TGAFILE();
		LoadTGAFile(this->file_name->c_str(), &tga_file);

		if (tga_file->imageData == nullptr)
		{
			std::cout << "Failed to load texture: '" << *this->file_name << "'" << std::endl;
			return;
		}
		//unsigned char* data = (unsigned char*)(data_str->c_str());

		width = tga_file->imageWidth;
		height = tga_file->imageHeight;
		nrChannels = tga_file->bitCount / 8;
		unsigned char* data = tga_file->imageData;

		if (data != nullptr)
		{
			glGenTextures(1, &texture_id);

			//glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture_id);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			
			//float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
			//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			if (nrChannels == 3)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tga_file->imageWidth, tga_file->imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, tga_file->imageData); // GL_BGR
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else if (nrChannels == 4)
			{
				// GL_UNSIGNED_INT_8_8_8_8_REV
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tga_file->imageWidth, tga_file->imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, tga_file->imageData); // GL_BGR GL_UNSIGNED_INT_8_8_8_8_REV
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			


			has_init_loaded = true;
		
			// check OpenGL error
			GLenum err;
			while ((err = glGetError()) != GL_NO_ERROR) {
				std::cout << "OpenGL error: " << err << std::endl;
			}

			std::cout << "[Loaded] texture '" << *this->file_name << "' width: " << width << " height: " << height << " channels: " << nrChannels << std::endl;
		}
		else 
		{
			std::cout << "Failed to load texture: '" << *this->file_name << "'" << std::endl;
		}
		//free(data);
	}
	void bind()
	{
		if (has_init_loaded) 
		{
			glBindTexture(GL_TEXTURE_2D, texture_id);
		}
	}
};

#endif