#pragma once

#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>


class Game
{
public:
	Game();
	~Game();

	bool Initialize(); 

	// Game Loop
	void ProcessInput(); 
	void UpdateGame(); 
	void GenerateOutput(); 

	// Start Game
	void RunGame(); 

	// ShutDown Game
	void ShutDown(); 

	// Load and Unload Data
	void LoadData();
	void UnloadData();


	// Actors
	void AddActor(class Actor* act); 
	void RemoveActor(class Actor* act); 
	void AddMeshComponent(class MeshComponent* mshComp) { mMeshComponents.emplace_back(mshComp); }
	void AddModelComponent(class ModelComponent* modelCMP) { mModelComponents.emplace_back(modelCMP); }
	void RemoveMeshComponent(class MeshComponent* mshCmp); 
	void RemoveModelComponent(class ModelComponent* modelCmp); 

	class Renderer* GetRenderer() { return mRenderer; }

	// Model Loading
	class Mesh* GetMesh(std::string &&filePath);
	class Model* GetModel(std::string &&filePath, bool calcTangent); 

	// Getters
	GLFWwindow* GetWindow() { return mWindow;  }
	int GetWidth() { return mWindowWidth;  }
	int GetHeight() { return mWindowHeight;  }
	std::vector<class Actor*>& GetActors() { return mActors;  }
	class Cube* GetLightSource() { return mLightSource;  }
	class Camera* GetCam() { return mCam;  }
	std::vector<class Cube*>& GetLightSources() { return mLightSources;  }
	std::vector<class MeshComponent*>& GetMeshComponents() { return mMeshComponents;  }
	std::vector<class ModelComponent*>& GetModelComponents() { return mModelComponents;  }


private:

	// DeltaTime 
	float mCurrentFrame;
	float mLastFrame; 

	// Window
	GLFWwindow* mWindow;
	int mWindowWidth; 
	int mWindowHeight;

	bool mIsRunning; 

	// Renderer
	class Renderer* mRenderer; 

	// Actors
	std::vector<class Actor*> mActors; 

	// Models
	std::unordered_map<std::string, class Model*> mModels;
	std::unordered_map<std::string, class Mesh*> mMeshes;
	std::vector<class MeshComponent*> mMeshComponents;
	std::vector<class ModelComponent*> mModelComponents;


	// Light
	class Cube* mLightSource;
	std::vector<class Cube*> mLightSources; 

	// Camera 
	class Camera* mCam; 
};

