#pragma once
#include "Component.h"
class MeshComponent :
	public Component
{
public:
	MeshComponent(class Actor* owner);
	~MeshComponent();

	void Draw(); 

	void SetMesh(class Mesh* msh) { mMesh = msh;  }

	class Mesh* GetMesh() { return mMesh;  }

private:
	class Mesh* mMesh; 
};

