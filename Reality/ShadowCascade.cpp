#include "ShadowCascade.h"
#include "Game.h"
#include "Renderer.h"
#include "Camera.h"
#include "DirectionalLight.h"
#include "GL/glew.h"
#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ImGUI/imgui.h"

#include <limits>
#include <algorithm>

ShadowCascade::ShadowCascade(Renderer* renderer) :
	mRenderer(renderer),
	mNear(0.1f),
	mFar(1000.f),
	mDepthMapID(0),
	mFrustumCenter(glm::vec3(0.0f)),
	mCornersNum(8)
{

}


void ShadowCascade::GenerateDepthMap() 
{
	glGenTextures(1, &mDepthMapID);
	glBindTexture(GL_TEXTURE_2D, mDepthMapID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, 2048, 2048, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void ShadowCascade::Unload() 
{
	glDeleteTextures(1, &mDepthMapID); 
}

void ShadowCascade::GetFrustumCenter() 
{

	float width = float(mRenderer->GetGame()->GetWidth()); 
	float height = float(mRenderer->GetGame()->GetHeight()); 
	
	mProjMatrix = glm::perspective(glm::radians(90.0f), (float)width / (float)height, mNear, mFar); 
	mViewMatrix = mRenderer->GetGame()->GetCam()->GetViewMatrix();
	
	glm::mat4 viewProj = mProjMatrix * mViewMatrix; 

	glm::vec3 frustumCorners[8] =
	{
		glm::vec3(-1.0f, 1.0f, -1.0f),
		glm::vec3(1.0f, 1.0f, -1.0f),
		glm::vec3(1.0f, -1.0f, -1.0f),
		glm::vec3(-1.0f, -1.0f, -1.0f),
		glm::vec3(-1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(1.0f, -1.0f, 1.0f),
		glm::vec3(-1.0f, -1.0f, 1.0f)
	};

	for (int i = 0; i < mCornersNum; ++i) 
	{
		glm::vec4 inversePoint = glm::inverse(viewProj) * glm::vec4(frustumCorners[i], 1.0f);
		mCorners[i] = glm::vec3(inversePoint / inversePoint.w); 
	}

	for (int i = 0; i < mCornersNum; ++i)
	{
		mFrustumCenter += mCorners[i]; 
	}

	mFrustumCenter /= (float)mCornersNum;
}

void ShadowCascade::CalcLightView() 
{
	glm::vec3 lightDir = mRenderer->GetDirectionalLight().GetDirection(); 
	mLightPos = mFrustumCenter + lightDir * (mFar - mNear);
	mLightView = glm::lookAt(mLightPos, mFrustumCenter , glm::vec3(0.0f, 1.0f, 0.0f));
}

void ShadowCascade::CalcLightProj() 
{
	
	for (int i = 0; i < mCornersNum; ++i) 
	{
		mCorners[i] = glm::vec3(mLightView * glm::vec4(mCorners[i], 1.0f)); 
	}

	float minX = std::numeric_limits<float>::max(); 
	float maxX = std::numeric_limits<float>::min(); 
	float minY = std::numeric_limits<float>::max(); 
	float maxY = std::numeric_limits<float>::min(); 
	float minZ = std::numeric_limits<float>::max();
	float maxZ = std::numeric_limits<float>::min();
	
	for (int i = 0; i < mCornersNum; ++i)
	{
		minX = std::min(minX, mCorners[i].x);
		maxX = std::max(maxX, mCorners[i].x);
		minY = std::min(minY, mCorners[i].y);
		maxY = std::max(maxY, mCorners[i].y);
		minZ = std::min(minZ, mCorners[i].z);
		maxZ = std::max(maxZ, mCorners[i].z);
	}

	mLightProj = glm::ortho(minX, maxX, minY, maxY, 0.1f, maxZ - minZ); 
	
}

ShadowCascade::~ShadowCascade()
{
}
