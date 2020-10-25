#pragma once
#include "Actor.h"
class Camera : public Actor
{
public:
	Camera(class Game* game , glm::vec3 pos);
	~Camera();

	void ProcessInput() override; 
	void CustomUpdate(float deltaTime) override;

	void CalcProjectionMatrix(); 

	void SetSensitivity(float val) { mSensitivity = val;  }
	void SetMaxSpeed(float val) { mMaxSpeed = val;  }
	void SetSpeed(float val) { mSpeed = val;  }

	float GetSensitivity() { return mSensitivity;  }
	float GetMaxSpeed() { return mMaxSpeed;  }
	float GetSpeed() { return mSpeed;  }
	float GetFOV() { return mFOV; }
	float GetNear() { return mNear;  }
	float GetFar() { return mFar;  }

	glm::vec3& GetPos() { return mCamPos;  }
	glm::vec3& GetForward() { return mCamForward;  }
	glm::vec3& GetRight() { return mCamRight;  }
	glm::vec3& GetUp() { return mCamUp;  }
	glm::vec3& GetTarget() { return mCamTarget;  }

	glm::mat4& GetViewMatrix() { return mView;  }
	glm::mat4& GetProjectionMatrix() { return mProjection;  }

private:
	
	//View
	glm::mat4 mView; 

	glm::vec3 mCamPos;
	glm::vec3 mCamTarget;
	glm::vec3 mCamUp;
	glm::vec3 mCamForward;
	glm::vec3 mCamRight;

	// Projection
	glm::mat4 mProjection; 

	float mFOV; 
	float mNear; 
	float mFar; 


	// Linear Speeds
	float mSpeed; 
	float mForwardSpeed; 
	float mRightSpeed; 
	float mSensitivity;
	float mMaxSpeed; 

	// Angular Speeds 
	float mPitch; 
	float mYaw; 

	// Flag
	bool mCheckFirstMove; 

	// Cursor Position
	double mCurrentXPos; 
	double mPreviousXPos;
	double mCurrentYPos;
	double mPreviousYPos;

	bool mActiveInput; 

};