#include "Actor.h"
#include "Game.h"
#include "Component.h"
#include "glm/ext.hpp"
#include "glm/gtx/string_cast.hpp"


Actor::Actor(Game *game) :
	mGame(game),
	mIsComputeWorldTransform(true),
	mPosition(glm::vec3(0.0f, 0.0f, 0.0f)),
	mScale(1.0f),
	mAngle(0.0f),
	mAxis(glm::vec3(0.0f, 1.0f, 0.0f)),
	mTransformationMatrix(glm::mat4(1.0f))
{
	mGame->AddActor(this); 
}

void Actor::CustomUpdate(float deltaTime) 
{

}

void Actor::Update(float deltaTime) 
{
	// Update Components 
	for (auto cmp : mComponents) 
	{
		cmp->Update(deltaTime); 
	}

	// Process any Custome Update
	CustomUpdate(deltaTime);

	// Calculate World Transform Matrix
	//ComputeWorldTransform(); 
}

void Actor::ComputeWorldTransform() 
{
	if (mIsComputeWorldTransform) 
	{
		mTranslationMatrix = glm::translate(glm::mat4(1.0f), mPosition);
		mRotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(mAngle) , mAxis); 
		mScaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(mScale, mScale, mScale));
		mTransformationMatrix = mTranslationMatrix * mRotationMatrix * mScaleMatrix; 

		mIsComputeWorldTransform = false;
	}
}

void Actor::AddComponent(Component *cmp) 
{
	mComponents.emplace_back(cmp); 
}

void Actor::RemoveComponent(Component* cmp) 
{
	mComponents.pop_back();
}


Actor::~Actor()
{
	GetGame()->RemoveActor(this);
	while (!mComponents.empty())
	{
		delete mComponents.back(); 
	}
}
