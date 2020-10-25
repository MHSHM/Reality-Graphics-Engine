#pragma once
class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();

	void Bind(); 

	void Unload(); 

	virtual void Initialize(int width, int height); 
	void CreateColorAttachment(int width , int height); 

	unsigned int GetColorAttachment() { return mColorAttachment;  }
	unsigned int GetID() { return mFrameBufferID;  }

private:

	unsigned int mFrameBufferID;

	unsigned int mColorAttachment; 
};

