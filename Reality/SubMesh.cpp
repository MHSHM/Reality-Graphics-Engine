#include "SubMesh.h"
#include "Material.h"
#include "VertexArrayObject.h"
#include "Model.h"

SubMesh::SubMesh(Material* mat, std::vector<Vertex> vertices, std::vector<unsigned int>& indecies, unsigned int layout):
	mMaterial(mat)
{
	mVAO = new VertexArrayObject(vertices, indecies, static_cast<unsigned int>(indecies.size()), layout); 
}

void SubMesh::Unload() 
{
	delete mVAO; 
}

SubMesh::~SubMesh()
{
}
