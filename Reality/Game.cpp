#include "Game.h"
#include "Actor.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Model.h"
#include "Texture.h"
#include "Cube.h"
#include "Camera.h"
#include "Shield.h"
#include "Land.h"
#include "BackPack.h"
#include <cstdio>
#include <cstdlib>
#include <time.h>

Game::Game():
	mWindow(nullptr),
	mWindowWidth(1920),
	mWindowHeight(1080),
	mCurrentFrame(0.0f),
	mLastFrame(0.0f),
	mIsRunning(false)
{

}

bool Game::Initialize() 
{
	if (!glfwInit()) 
	{
		std::cout << "Failed to Initialize GLFW! \n"; 
		return false; 
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR , 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); 

	mWindow = glfwCreateWindow(mWindowWidth, mWindowHeight, "Reality Engine", nullptr, nullptr);
	if (!mWindow) 
	{
		std::cout << "Failed to Create Window! \n"; 
		return false; 
	}

	glfwMakeContextCurrent(mWindow); 

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << "Failed to Initialize GLEW! \n";
		return false;
	}

	mRenderer = new Renderer(this); 
	mRenderer->Initialize(); 
	
	LoadData(); 

	return true; 
}

void Game::ProcessInput()
{
	glfwPollEvents();
	mIsRunning = glfwWindowShouldClose(mWindow); 


	int state = glfwGetKey(mWindow , GLFW_KEY_ESCAPE); 
	if (state == GLFW_PRESS) 
	{
		mIsRunning = true; 
	}

	for (auto actor : mActors) 
	{
		actor->ProcessInput(); 
	}

}

void Game::UpdateGame() 
{
	mCurrentFrame = static_cast<float>(glfwGetTime()); 
	float deltaTime = mCurrentFrame - mLastFrame; 
	mLastFrame = mCurrentFrame; 

	for (auto actor : mActors) 
	{
		actor->Update(deltaTime); 
	}
}

void Game::GenerateOutput() 
{
	mRenderer->Draw(); 
}

void Game::LoadData() 
{ 
	BackPack* backPack = new BackPack(this);
	backPack->SetScale(0.5f);
	backPack->SetPosition(glm::vec3(0.0f, 2.0f, 0.0f));
	backPack->SetName("Bag");
	
	Land* land = new Land(this);
	land->SetPosition(glm::vec3(0.0f, -2.0f, 0.0f));
	land->SetRotation(-90.0f , glm::vec3(1.0f , 0.0f , 0.0f));
	land->SetScale(0.5f);
	land->SetName("Terrain");
	
	mCam = new Camera(this , glm::vec3(0.0f, 2.0f, 5.0f));
}

Mesh* Game::GetMesh(std::string &&filePath)
{
	if (!mMeshes[filePath])
	{
		Mesh* ms = new Mesh();
		bool check = ms->LoadOBJ(filePath);
		if (check)
		{
			mMeshes[filePath] = ms;
			return ms;
		}

		delete ms;
		ms = nullptr;
		return ms;
	}

	return mMeshes[filePath];
}

Model* Game::GetModel(std::string &&filePath, bool calcTangent)
{
	if (!mModels[filePath])
	{
		Model* mo = new Model();
		bool check = mo->LoadSubMeshes(filePath, calcTangent);
		if (check)
		{
			mModels[filePath] = mo;
			mo->LoadTextures();
			return mo;
		}

		delete mo;
		mo = nullptr;
	}

	return mModels[filePath];
}


void Game::RemoveMeshComponent(MeshComponent* mshCmp)
{
	auto iter = std::find(mMeshComponents.begin(), mMeshComponents.end(), mshCmp);
	if (iter != mMeshComponents.end())
	{
		mMeshComponents.erase(iter);
	}
}

void Game::RemoveModelComponent(ModelComponent* modelCmp)
{
	auto iter = std::find(mModelComponents.begin(), mModelComponents.end(), modelCmp);
	if (iter != mModelComponents.end())
	{
		mModelComponents.erase(iter);
	}
}

void Game::UnloadData()
{
	for (auto& mesh : mMeshes)
	{
		mesh.second->Unload();
		delete mesh.second;
		mesh.second = nullptr;
	}
	mMeshes.clear();

	for (auto& model : mModels)
	{
		model.second->Unload();
		delete model.second;
		model.second = nullptr;
	}
	mModels.clear();

	while (!mActors.empty())
	{
		delete mActors.back(); 
	}
	mActors.clear(); 
}

void Game::AddActor(Actor *act) 
{
	mActors.emplace_back(act); 
}

void Game::RemoveActor(Actor *act) 
{
	mActors.pop_back(); 
}

void Game::RunGame() 
{
	while (!mIsRunning)
	{
		ProcessInput(); 
		UpdateGame(); 
		GenerateOutput(); 
	}
}

void Game::ShutDown() 
{ 
	glfwTerminate(); 
	UnloadData(); 
	mRenderer->ShutDown(); 
	delete mRenderer; 
	mRenderer = nullptr; 
	mCam = nullptr; 
}

Game::~Game()
{
}
