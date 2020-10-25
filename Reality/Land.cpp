#include "Land.h"
#include "Game.h"
#include "Renderer.h"
#include "Shader.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "ModelComponent.h"
#include "Model.h"

Land::Land(Game* game):
	Actor(game)
{
	modelCmp = new ModelComponent(this); 
	modelCmp->SetModel(game->GetModel("Assets/Models/Terrain.obj" , false)); 
}


Land::~Land()
{
}
