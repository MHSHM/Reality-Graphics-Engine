#include "DirectionalLight.h"

#include"glm/geometric.hpp"

DirectionalLight::DirectionalLight(glm::vec3 direction):
	mDirection(direction)
{
}

glm::vec3& DirectionalLight::GetDirection() 
{
	mDirection = glm::normalize(mDirection); 
	return mDirection; 
}

DirectionalLight::~DirectionalLight()
{
}
