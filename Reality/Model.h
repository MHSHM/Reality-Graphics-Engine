#pragma once

#include "glm/geometric.hpp"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

#include <vector>
#include <unordered_map>
#include <string>

class Material; 
class Texture; 
class SubMesh; 

struct Vertex
{
	glm::vec3 pos;
	glm::vec2 UVs;
	glm::vec3 normal;
	
	glm::vec3 tangent; 

	bool check; 

	Vertex() :
		pos(glm::vec3(0)),
		UVs(glm::vec2(0)),
		normal(glm::vec3(0)),
		tangent(glm::vec3(0)),
		check(false) {}
};

class Model
{
public:
	Model();
	~Model();

	bool LoadMtlFile(std::string file); 
	void LoadTextures(); 
	bool LoadSubMeshes(std::string file , bool calcTangent);

	std::vector<SubMesh*>& GetSubMeshes() { return mSubMeshes;  }
	std::unordered_map<std::string, Material*>& GetMaterials() { return mMaterials;  }
	std::unordered_map< std::string, std::vector<Texture*> >& GetTextures() { return mTextures;  }

	void Unload(); 

	void SetShader(std::string shader) { mShader = shader;  }

	std::string &GetShaderName() { return mShader;  }

private:

	std::string mShader; 
	std::vector<SubMesh*> mSubMeshes; 
	std::unordered_map<std::string, Material*> mMaterials; 
	std::unordered_map< std::string , std::vector<Texture*> > mTextures; 

	bool mTangent; 

};

