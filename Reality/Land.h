#pragma once
#include "Actor.h"
class Land : public Actor
{
public:
	Land(class Game* game);
	~Land();

private:
	class MeshComponent* msh; 

	class ModelComponent* modelCmp; 
};

