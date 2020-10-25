#pragma once
#include "GL/glew.h"
#include <vector>

#include "Model.h"

class VertexArrayObject
{
public:
	VertexArrayObject(std::vector<Vertex>& v ,std::vector<unsigned int> &indecies, unsigned int EBOsize , unsigned int layout);
	~VertexArrayObject();

	void Bind(); 

	unsigned int GetEBOsize() { return EBOsize; }

private:

	unsigned int EBOsize; 

	unsigned int mVAOiD;
	unsigned int mVBOiD; 
	unsigned int mEBOiD; 

};

