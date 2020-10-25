#include "VertexArrayObject.h"

VertexArrayObject::VertexArrayObject(std::vector<Vertex>& vertices, std::vector<unsigned int>& indecies,
	unsigned int EBOsize , unsigned int layout):
	EBOsize(EBOsize)
{

	glGenVertexArrays(1, &mVAOiD);
	glBindVertexArray(mVAOiD);

	glGenBuffers(1, &mVBOiD);
	glBindBuffer(GL_ARRAY_BUFFER, mVBOiD);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex) , vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &mEBOiD);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBOiD);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indecies.size() * sizeof(unsigned int), indecies.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex) , 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex) , (void*)offsetof(Vertex, UVs));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex) , (void*)offsetof(Vertex, normal));

	if (layout == 4) 
	{
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
	}
}


void VertexArrayObject::Bind() 
{
	glBindVertexArray(mVAOiD);
}

VertexArrayObject::~VertexArrayObject()
{
	glDeleteVertexArrays(1, &mVAOiD);
	glDeleteBuffers(1, &mVBOiD);
	glDeleteBuffers(1, &mEBOiD);
}
