#include "Texture.h"
#include <iostream>
#include "SOIL2/SOIL2.h"

Texture::Texture()
{
}

bool Texture::Load(std::string texturePath) 
{
	int channels = 0; 
	
	unsigned char* image = SOIL_load_image(texturePath.c_str(), &mWidth, &mHeight, &channels, SOIL_LOAD_AUTO); 
	
	if (!image) 
	{
		std::cout << "Failed to load image " << texturePath; 
		return false; 
	}

	int format = GL_RGB;
	if (channels == 4) 
	{
		format = GL_RGBA; 
	}

	// Generate Texture
	glGenTextures(1, &mTextureID);  
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	// Set Texture data
	glTexImage2D(GL_TEXTURE_2D, 0, format, mWidth, mHeight, 0, format, GL_UNSIGNED_BYTE, image); 

	// Set Filters
	glGenerateMipmap(GL_TEXTURE_2D);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	// Free data
	SOIL_free_image_data(image);
	
	return true; 
}

void Texture::Unload() 
{
	glDeleteTextures(1, &mTextureID); 
}

void Texture::Bind(int unit) 
{
	glActiveTexture(GL_TEXTURE0 + unit); 
	glBindTexture(GL_TEXTURE_2D, mTextureID); 
}

Texture::~Texture()
{
}
