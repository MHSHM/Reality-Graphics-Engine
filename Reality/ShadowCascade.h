#pragma once
#include "glm/mat4x4.hpp"

#include <array>
class ShadowCascade
{
public:
	ShadowCascade(class Renderer* renderer);
	~ShadowCascade();

	void GenerateDepthMap(); 

	void GetFrustumCenter();
	void CalcLightView(); 
	void CalcLightProj(); 

	unsigned int GetDepthID() { return mDepthMapID;  }

	void SetNearZ(float val) { mNear = val;  }
	void SetFarZ(float val) { mFar = val;  }
	void SetDepthID(unsigned int val) { mDepthMapID = val;  }

	glm::mat4& GetLightView() { return mLightView;  }
	glm::mat4& GetLightProj() { return mLightProj; }
	glm::vec3 GetLightPos() { return mLightPos;  }
	float GetFar() { return mFar;  }

	void Unload(); 

private:
	
	int mCornersNum; 

	class Renderer* mRenderer; 

	float mFar; 
	float mNear; 

	unsigned int mDepthMapID; 

	glm::mat4 mViewMatrix; 
	glm::mat4 mProjMatrix; 
	glm::mat4 mLightView; 
	glm::mat4 mLightProj; 
	glm::vec3 mLightPos; 

	glm::vec3 mCorners[8]; 

	glm::vec3 mFrustumCenter; 
	float mDistance; 
};

