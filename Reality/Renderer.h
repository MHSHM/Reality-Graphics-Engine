#pragma once
#include "GL/glew.h"
#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ImGuiManager.h"
#include "FrameBuffer.h"
#include "ShadowBuffer.h"
#include "DirectionalLight.h"
#include "SkyBox.h"

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <array>

class Game; 
class Shader; 
class Material; 
class Texture;
class ShadowCascade; 

class Renderer
{
public:

	Renderer(Game* game); 
	~Renderer(); 

	void Initialize(); 
	void ShutDown(); 

	void Draw(); 
	void Draw3DScene(); 
	
	// Shadow Mapping
	void FillDepthMap(); 
	void SetShadowMapUniform(Shader* shader);
	void SetFarUniform(Shader* shader); 
	void SetShadowCascades(); 
	void SetClipFar(); 

	// Screen Quad
	void SetDrawQuad(); 
	void DrawQuad(); 

	// Textures
	class Texture* GetTexture(std::string texturePath);

	void LoadShaders();

	// Getters
	ImGuiManager& GetImGuiManager() { return mImGuiManager;  }
	std::unordered_map<std::string, Shader*>& GetShaders() { return mShaders;  }
	Game* GetGame() { return mGame;  }
	std::vector<ShadowCascade*> &GetCascades() { return mCascades;  }
	FrameBuffer& GetFrameBuffer() { return mFrameBuffer;  }
	DirectionalLight& GetDirectionalLight() { return mDirectionalLight;  }

	// Setters
	void SetLightUniforms(Shader* shader , Material* material);
	void SetMaterialUniforms(Shader* shader , Material* material , std::vector<Texture*>& textures);
	void SetFiltersUniforms(Shader* shader, bool sharp, bool blur, bool edgeDetect , bool gammaCorrection); 
	void SetLightFactors(Shader* shader , Material* material);

private:

	Game* mGame; 

	// Shadow Mapping 
	uint32_t mDepthMapWidth; 
	uint32_t mDepthMapHeight;
	uint32_t mCascadesNum;
	std::vector<ShadowCascade*> mCascades; 
	std::vector<glm::vec4> mCascadeFar; 

	// Shaders
	std::unordered_map<std::string, Shader*> mShaders; 

	// Textures
	std::unordered_map<std::string, Texture*> mTextures;

	// ImGui Manager
	ImGuiManager mImGuiManager; 

	// FrameBuffers
	FrameBuffer mFrameBuffer; 
	
	// Shadow Mapping
	ShadowBuffer mShadowBuffer;

	// Screen Quad 
	uint32_t mScreenQuadVAO;
	Shader* mScreenQuadShader; 

	// SkyBox 
	SkyBox mSkyBox; 

	// Directional Light
	DirectionalLight mDirectionalLight; 
};

