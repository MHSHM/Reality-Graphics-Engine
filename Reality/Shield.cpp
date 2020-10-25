#include "Shield.h"
#include "Game.h"
#include "Renderer.h"
#include "Model.h"
#include "ModelComponent.h"
#include "Shader.h"
#include "Texture.h"

Shield::Shield(Game* game):
	Actor(game)
{ 
	modelCmp = new ModelComponent(this); 
	modelCmp->SetModel(game->GetModel("Assets/Models/Shield.obj" , true)); 
}


Shield::~Shield()
{
}
