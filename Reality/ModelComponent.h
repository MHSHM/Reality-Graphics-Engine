#pragma once
#include "Component.h"
class ModelComponent : public Component
{
public:
	ModelComponent(class Actor* owner);
	~ModelComponent();

	void Draw();

	void SetModel(class Model* mod) { mModel = mod;  }
	class Model* GetModel() { return mModel;  }

private:
	class Model* mModel; 
};

