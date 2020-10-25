#pragma once
#include<string>
#include <vector>
class Mesh
{
public:
	Mesh();
	~Mesh();

	void Unload(); 

	bool LoadOBJ(std::string objFile); 
	
	class VertexArrayObject* GetVAO() { return mVAO;  }

	std::string GetShaderName() { return mShaderName;  }

	void SetTexture(class Texture* text) { mTextures.emplace_back(text);  };
	void SetShader(std::string shader) { mShaderName = shader;  }

	std::vector<class Texture*>& GetTextures() { return mTextures;  }
	
private:
	class VertexArrayObject* mVAO; 

	std::string mShaderName; 

	std::vector<class Texture*> mTextures; 
};

