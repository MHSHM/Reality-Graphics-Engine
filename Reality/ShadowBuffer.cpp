#include "ShadowBuffer.h"
#include "GL/glew.h"

#include <iostream>

ShadowBuffer::ShadowBuffer():
	mBufferID(0),
	mDepthMapID(0)
{
}

void ShadowBuffer::Initialize(int width , int height) 
{

	mDepthShader.Link("Shaders/DepthMap.vert", "Shaders/DepthMap.frag");

	glGenFramebuffers(1, &mBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, mBufferID);

	CreateDepthMap(width, height);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthMapID, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	// Check if Frame Buffer is Successfully Created 
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Failed to Create FrameBuffer \n";
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void ShadowBuffer::CreateDepthMap(int width , int height) 
{
	glGenTextures(1, &mDepthMapID);
	glBindTexture(GL_TEXTURE_2D, mDepthMapID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

}

void ShadowBuffer::Bind() 
{
	glBindFramebuffer(GL_FRAMEBUFFER, mBufferID);
}

void ShadowBuffer::Unload() 
{
	glDeleteFramebuffers(1, &mBufferID);
}

ShadowBuffer::~ShadowBuffer()
{
}
