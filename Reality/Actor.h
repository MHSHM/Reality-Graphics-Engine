#pragma once
#include "glm/vec3.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"


#include <vector>
#include <string>

class Actor
{
public:
	Actor(class Game* game);
	virtual ~Actor();

	// Input Phase
	virtual void ProcessInput() {}


	// Update Phase
	virtual void CustomUpdate(float deltaTime); 
	void Update(float deltaTime); 

	// Transformations
	void ComputeWorldTransform(); 

	// Components Management
	void AddComponent(class Component* cmp); 
	void RemoveComponent(class Component* cmp); 

	// Transformations Setters
	void SetPosition(glm::vec3 pos) { mIsComputeWorldTransform = true , mPosition = pos;  }
	void SetScale(float val) { mIsComputeWorldTransform = true ,  mScale = val;  }
	void SetRotation(float angle, glm::vec3 axis) { mIsComputeWorldTransform = true, mAngle = angle, mAxis = axis;  }
	void SetName(std::string val) { mName = val;  }


	// Getters
	glm::vec3& GetPosition() { return mPosition;  }
	class Game* GetGame() { return mGame; }
	glm::mat4& GetWorldTransform() { return mTransformationMatrix;  }
	float GetScale() { return mScale;  }
	float GetAngle() { return mAngle;  }
	glm::vec3& GetRotationAxis() { return mAxis;  }
	std::string& GetName() { return mName;  }
	
private:

	// Game Reference
	class Game* mGame; 

	// Transformation
	glm::vec3 mPosition; 
	float mScale; 
	float mAngle; 
	glm::vec3 mAxis; 

	glm::mat4 mTranslationMatrix; 
	glm::mat4 mRotationMatrix; 
	glm::mat4 mScaleMatrix; 

	std::string mName; 

	bool mIsComputeWorldTransform; 
	
	// Rendering 
	glm::mat4 mTransformationMatrix; 
	
	// Components
	std::vector<class Component*> mComponents; 

};

