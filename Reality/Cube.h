#pragma once
#include "Actor.h"
class Cube : public Actor
{
public:
	Cube(class Game* game);
	~Cube();


private:
	class MeshComponent* msh; 
};

