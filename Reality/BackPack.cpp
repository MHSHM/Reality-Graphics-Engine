#include "BackPack.h"
#include "Game.h"
#include "Renderer.h"
#include "Model.h"
#include "ModelComponent.h"
#include "Mesh.h"
#include "Texture.h"

BackPack::BackPack(Game* game) :
	Actor(game)
{
	modelCmp = new ModelComponent(this);
	modelCmp->SetModel(game->GetModel("Assets/Models/backpack.obj" , true)); 
}


BackPack::~BackPack()
{
}
