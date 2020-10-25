#pragma once
#include "Actor.h"
class BackPack : public Actor
{
public:
	BackPack(class Game* game);
	~BackPack();

private:
	class ModelComponent* modelCmp; 
};

