#pragma once

#include <vector>
#include <string>

#include "Shader.h"

class SkyBox
{
public:
	SkyBox();
	~SkyBox();

	void Initilaize();

	void SetFaces(); 

	void Bind(Shader* shader , int unit);

	void Draw(class Renderer* renderer); 

private:

	unsigned int mVAO; 
	unsigned int mVBO; 
	unsigned int mCubeMapID; 

	Shader mShader; 

	std::vector<std::string> mFaces; 
};

