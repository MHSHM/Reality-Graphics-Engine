#include "FrameBuffer.h"
#include "Game.h"
#include "GL/glew.h"

#include <iostream>

FrameBuffer::FrameBuffer():
	mFrameBufferID(0),
	mColorAttachment(0)
{

}

void FrameBuffer::Initialize(int width , int height) 
{

	glGenFramebuffers(1, &mFrameBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferID);

	// Create Color Attachment (Color Buffer)
	CreateColorAttachment(width, height);

	// Bind Color Buffer to New Frame Buffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorAttachment, 0);

	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);

	// Set Render Buffer as a Depth and Stencil Buffers
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	// Bind Default Render Buffer After Allocation 
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// Bind Render Buffer Object to the new Frame Buffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	// Check if Frame Buffer is Successfully Created 
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Failed to Create FrameBuffer \n";
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void FrameBuffer::Bind() 
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferID);
}

void FrameBuffer::CreateColorAttachment(int width , int height) 
{
	glGenTextures(1, &mColorAttachment);
	glBindTexture(GL_TEXTURE_2D, mColorAttachment); 
	glTexImage2D(GL_TEXTURE_2D , 0 , GL_RGB , width , height , 0 , GL_RGB , GL_UNSIGNED_BYTE , nullptr); 

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
}

void FrameBuffer::Unload() 
{
	glDeleteFramebuffers(1, &mFrameBufferID); 
}

FrameBuffer::~FrameBuffer()
{
}
