#include "Camera.h"
#include "Game.h"
#include "Renderer.h"
#include "ImGuiManager.h"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtc/epsilon.hpp"


Camera::Camera(Game* game, glm::vec3 pos) :
	Actor(game),
	mSpeed(4.0f),
	mSensitivity(30.0f),
	mMaxSpeed(100.0f),
	mPitch(0.0f),
	mYaw(0.0f),
	mCamTarget(glm::vec3(0.0f, 0.0f, -100.0f)),
	mCamPos(pos),
	mCamUp(glm::vec3(0.0f, 1.0f, 0.0f)),
	mCamRight(glm::vec3(-1.0f, 0.0f, 0.0f)),
	mCamForward(glm::vec3(0.0f, 0.0f, -1.0f)),
	mCheckFirstMove(false),
	mCurrentXPos(0.0),
	mPreviousXPos(0.0),
	mCurrentYPos(0.0),
	mPreviousYPos(0.0),
	mActiveInput(true),
	mFOV(90.0f),
	mNear(0.1f),
	mFar(1000.0f)
{
	CalcProjectionMatrix(); 
}

void Camera::CalcProjectionMatrix() 
{
	int width = GetGame()->GetWidth(); 
	int height = GetGame()->GetHeight(); 

	mProjection = glm::perspective(glm::radians(mFOV), float(width) / float(height), mNear, mFar); 
}

void Camera::ProcessInput()
{	

	int ctrlState = glfwGetKey(GetGame()->GetWindow(), GLFW_KEY_LEFT_CONTROL); 
	int shiftState = glfwGetKey(GetGame()->GetWindow(), GLFW_KEY_LEFT_SHIFT); 
	
	if (ctrlState == GLFW_PRESS) 
	{
		mActiveInput = false; 
	}

	glfwGetCursorPos(GetGame()->GetWindow(), &mCurrentXPos, &mCurrentYPos);

	if (shiftState == GLFW_PRESS) 
	{
		mActiveInput = true;
		mPreviousXPos = mCurrentXPos; 
		mPreviousYPos = mCurrentYPos; 
	}

	if (!mActiveInput) 
	{
		glfwSetInputMode(GetGame()->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		return; 
	}

	// Handle mouse input
	glfwSetInputMode(GetGame()->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (glfwRawMouseMotionSupported())
	{
		glfwSetInputMode(GetGame()->GetWindow(), GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}

	if (!mCheckFirstMove)
	{
		mPreviousXPos = mCurrentXPos;
		mPreviousYPos = mCurrentYPos;
		mCheckFirstMove = true;
	}

	mPitch = static_cast<float>(mCurrentYPos) - static_cast<float>(mPreviousYPos);
	mPitch = (mPitch / mMaxSpeed) * mSensitivity;
	mPreviousYPos = mCurrentYPos;

	mYaw = static_cast<float>(mCurrentXPos) - static_cast<float>(mPreviousXPos);
	mYaw = (mYaw / mMaxSpeed) * mSensitivity;
	mPreviousXPos = mCurrentXPos;

	// Handle keyboard input
	mForwardSpeed = 0.0f;
	int wState = glfwGetKey(GetGame()->GetWindow(), GLFW_KEY_W);
	if (wState == GLFW_PRESS)
	{
		mForwardSpeed += mSpeed;
	}

	int sState = glfwGetKey(GetGame()->GetWindow(), GLFW_KEY_S);
	if (sState == GLFW_PRESS)
	{
		mForwardSpeed -= mSpeed;
	}

	mRightSpeed = 0.0f;
	int aState = glfwGetKey(GetGame()->GetWindow(), GLFW_KEY_A);
	if (aState == GLFW_PRESS)
	{
		mRightSpeed += mSpeed;
	}

	int dState = glfwGetKey(GetGame()->GetWindow(), GLFW_KEY_D);
	if (dState == GLFW_PRESS)
	{
		mRightSpeed -= mSpeed;
	}
}

void Camera::CustomUpdate(float deltaTime)	
{

	if (!mActiveInput) 
	{
		return; 
	}

	// Check if the up vector is rotated by 90 degrees
	// if so then lock pitch rotation
	if (glm::epsilonEqual(mCamForward.y, 1.0f, 0.1f))
	{
		if (mPitch < 0.0)
		{
			mPitch = 0.0f;
		}
	}
	else if (glm::epsilonEqual(mCamForward.y, -1.0f, 0.1f))
	{
		if (mPitch > 0.0f)
		{
			mPitch = 0.0f;
		}
	}

	// Rotate forward around the current right
	mCamForward = glm::rotate(mCamForward, mPitch * deltaTime, mCamRight);

	// Get new right
	mCamRight = glm::normalize(glm::cross(mCamUp, mCamForward));

	// Rotate forward around up
	mCamForward = glm::rotate(mCamForward, -mYaw * deltaTime, mCamUp);

	// Calculate new Camera position
	mCamPos += ((mCamForward * mForwardSpeed) + (mCamRight * mRightSpeed)) * deltaTime;

	// Calculate new view Matrix
	glm::mat4 view(1.0f);
	mView = glm::lookAt(mCamPos, mCamForward * 1000.0f, mCamUp);
}

Camera::~Camera()
{
}
