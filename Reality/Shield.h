#pragma once
#include "Actor.h"
class Shield :
	public Actor
{
public:
	Shield(class Game *game);
	~Shield();

private:
	class ModelComponent* modelCmp; 
};

