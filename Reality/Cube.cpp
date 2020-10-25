#include "Cube.h"
#include "Game.h"
#include "Renderer.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "Shader.h"


Cube::Cube(Game* game):
	Actor(game)
{ 
	msh = new MeshComponent(this); 
	msh->SetMesh(game->GetMesh("Assets/Models/Cube.obj")); 
}


Cube::~Cube()
{
}
