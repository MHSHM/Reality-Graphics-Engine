#pragma once

#include "Shader.h"

class ShadowBuffer
{
public:
	ShadowBuffer();
	~ShadowBuffer();

	void Initialize(int width , int height); 

	void CreateDepthMap(int width , int height); 

	void Bind(); 

	void Unload(); 

	unsigned int GetDepthAttachment() const { return mDepthMapID; }
	Shader& GetDepthShader() { return mDepthShader;  }

private:
	unsigned int mBufferID;
	unsigned int mDepthMapID;

	Shader mDepthShader; 
};

