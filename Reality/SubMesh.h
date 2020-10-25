#pragma once
#include <string>
#include <vector>
#include "Model.h"
class SubMesh
{
public:
	SubMesh(class Material* mat , std::vector<Vertex> vertices , std::vector<unsigned int>& indecies , unsigned int layout);
	~SubMesh();


	class VertexArrayObject* GetVAO() { return mVAO;  }
	std::string& GetShaderName() { return mShader;  }
	class Material* GetMaterial() { return mMaterial;  }

	void Unload(); 

private:

	class VertexArrayObject* mVAO;
	std::string mShader;
	class Material* mMaterial; 
};

