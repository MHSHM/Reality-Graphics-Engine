#pragma once
#include "GL/glew.h"
#include "SOIL2/SOIL2.h"
#include <string>
class Texture
{
public:
	Texture();
	~Texture();

	bool Load(std::string texturePath); 
	
	void Unload(); 

	void Bind(int unit); 

	unsigned int GetID() { return mTextureID;  }

private:

	int mWidth; 
	int mHeight; 

	unsigned int mTextureID; 
};

