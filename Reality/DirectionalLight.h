#pragma once

#include "glm/vec3.hpp"

class DirectionalLight
{
public:
	DirectionalLight(glm::vec3 direction);
	~DirectionalLight();

	glm::vec3& GetDirection(); 

private:
	glm::vec3 mDirection; 
};

