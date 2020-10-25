#include "Renderer.h"
#include "Game.h"
#include "Actor.h"
#include "glm/gtx/string_cast.hpp"
#include "VertexArrayObject.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "ModelComponent.h"
#include "Model.h"
#include "SubMesh.h"
#include "Material.h"
#include "Shader.h"
#include "Texture.h"
#include "Cube.h"
#include "Camera.h"
#include "ShadowCascade.h"
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_opengl3.h"
#include "ImGUI/imgui_impl_glfw.h"
#include "SOIL2/SOIL2.h"

Renderer::Renderer(Game* game) :
	mGame(game),
	mCascadesNum(3),
	mDepthMapWidth(2048),
	mDepthMapHeight(2048),
	mDirectionalLight(glm::vec3(5.0f, 50.0f, 5.0f))
{
}

void Renderer::Initialize() 
{
	// Initialize ImGUI
	mImGuiManager.Initialize(mGame); 

	// Load Rendering Shaders
	LoadShaders(); 

	// Create VAO for ScreenQuad
	SetDrawQuad(); 

	mSkyBox.Initilaize();

	// Generate Cube map Texture
	mSkyBox.SetFaces(); 

	// Create Frame buffer to Render To
	mFrameBuffer.Initialize(mGame->GetWidth() , mGame->GetHeight()); 

	// Create Shadow Frame Buffer for Shadow Mapping
	mShadowBuffer.Initialize(mDepthMapWidth , mDepthMapHeight);
	
	// Initialize Shadow Cascades
	SetShadowCascades(); 
}

void Renderer::Draw() 
{
	// Start ImGUI Frame
	mImGuiManager.CreateNewFrame();

	// Create Camera Window
	mImGuiManager.CreateCameraWindow(mGame->GetCam());

	// Renderer Background (Skybox)
	mSkyBox.Draw(this); 

	// Create Shadow Map
	FillDepthMap(); 

	// Render Scene
	Draw3DScene();
	
	// Draw Quad with the Size of the screen
	// Then Sample from The Texture Scene rendererd before
	DrawQuad(); 

	// Render UI
	mImGuiManager.Draw(); 

	// Swap
	glfwSwapBuffers(mGame->GetWindow());
}

void Renderer::FillDepthMap() 
{
	glViewport(0, 0, mDepthMapWidth, mDepthMapHeight); 
	mShadowBuffer.Bind(); 

	for (int i = 0; i < mCascadesNum; ++i) 
	{
	
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mCascades[i]->GetDepthID(), 0); 
		glClear(GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		mCascades[i]->GetFrustumCenter(); 
		mCascades[i]->CalcLightView(); 
		mCascades[i]->CalcLightProj(); 
		
		// Render Models Without Materials
		for (auto &msh : mGame->GetMeshComponents())
		{

			msh->GetOwner()->ComputeWorldTransform();

			VertexArrayObject* vao = msh->GetMesh()->GetVAO();

			mShadowBuffer.GetDepthShader().Bind();

			// Set Transformations Matrices
			mShadowBuffer.GetDepthShader().SetMatrix4Uniform("uWorldTransform", msh->GetOwner()->GetWorldTransform());

			glm::mat4 lightView = mCascades[i]->GetLightView(); 
			glm::mat4 lightProj = mCascades[i]->GetLightProj(); 
			glm::mat4 lightViewProh = lightProj * lightView; 
			mShadowBuffer.GetDepthShader().SetMatrix4Uniform("uLightTransformation", lightViewProh);

			// Bind VAO Then Draw
			vao->Bind();
			glDrawElements(GL_TRIANGLES, vao->GetEBOsize(), GL_UNSIGNED_INT, nullptr);
		}

		// Render Models With Materials
		for (auto &model : mGame->GetModelComponents())
		{
			// Compute Model Matrix
			model->GetOwner()->ComputeWorldTransform();

			// Get Model in Submeshes Form
			std::vector<SubMesh*> submeshes = model->GetModel()->GetSubMeshes();

			// Bind Shader
			mShadowBuffer.GetDepthShader().Bind();

			// Set Transformations Uniform
			mShadowBuffer.GetDepthShader().SetMatrix4Uniform("uWorldTransform", model->GetOwner()->GetWorldTransform());

			glm::mat4 lightView = mCascades[i]->GetLightView();
			glm::mat4 lightProj = mCascades[i]->GetLightProj();
			glm::mat4 lightViewProh = lightProj * lightView;
			mShadowBuffer.GetDepthShader().SetMatrix4Uniform("uLightTransformation", lightViewProh);
			
			// Draw each Submesh with its materials and textures
			for (auto &submesh : submeshes)
			{
				// Draw This Submesh
				submesh->GetVAO()->Bind();
				glDrawElements(GL_TRIANGLES, submesh->GetVAO()->GetEBOsize(), GL_UNSIGNED_INT, nullptr);
			}
		}
	}
}

void Renderer::SetShadowCascades() 
{
	mCascades.resize(mCascadesNum); 

	ShadowCascade* shadowCascade = new ShadowCascade(this); 
	shadowCascade->SetDepthID(mShadowBuffer.GetDepthAttachment()); 
	shadowCascade->SetNearZ(0.1f); 
	shadowCascade->SetFarZ(20.0f); 
	mCascades[0] = shadowCascade; 

	shadowCascade = new ShadowCascade(this); 
	shadowCascade->GenerateDepthMap(); 
	shadowCascade->SetNearZ(0.1f);
	shadowCascade->SetFarZ(50.0f);
	mCascades[1] = shadowCascade; 
	
	shadowCascade = new ShadowCascade(this); 
	shadowCascade->GenerateDepthMap(); 
	shadowCascade->SetNearZ(0.1f);
	shadowCascade->SetFarZ(200.0f);
	mCascades[2] = shadowCascade; 

	SetClipFar(); 
}

void Renderer::SetClipFar() 
{
	mCascadeFar.resize(mCascadesNum); 
	for (int i = 0; i < mCascadesNum; ++i) 
	{
		glm::vec4 temp = glm::vec4(0.0f, 0.0f, mCascades[i]->GetFar(), 1.0f); 
		mCascadeFar[i] = temp;
	}
}

void Renderer::Draw3DScene() 
{
	mFrameBuffer.Bind(); 
	glViewport(0, 0, mGame->GetWidth(), mGame->GetHeight()); 
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT); 
	mImGuiManager.CreateFrameRateWindow();

	// Render Models Without Materials
	for (auto &msh : mGame->GetMeshComponents())
	{
		mImGuiManager.CreateModelWindow(msh, mShaders);

		msh->Draw();
	}

	// Render Models With Materials
	for (auto &model : mGame->GetModelComponents())
	{
		// Create UI For the Model
		mImGuiManager.CreateModelWindow(model, mShaders);

		model->Draw(); 
	}
}

void Renderer::SetShadowMapUniform(Shader* shader) 
{	
	glActiveTexture(GL_TEXTURE3); 
	glBindTexture(GL_TEXTURE_2D, mCascades[0]->GetDepthID()); 
	shader->SetTextureUniform("shadowMap[0]", 3);
	
	glActiveTexture(GL_TEXTURE4); 
	glBindTexture(GL_TEXTURE_2D, mCascades[1]->GetDepthID()); 
	shader->SetTextureUniform("shadowMap[1]", 4);
	
	glActiveTexture(GL_TEXTURE5); 
	glBindTexture(GL_TEXTURE_2D, mCascades[2]->GetDepthID()); 
	shader->SetTextureUniform("shadowMap[2]", 5); 
}

void Renderer::SetFarUniform(Shader* shader) 
{
	shader->SetFloatUniform("ViewFarValues[0]" , mCascadeFar[0].z);
	shader->SetFloatUniform("ViewFarValues[1]" , mCascadeFar[1].z);
	shader->SetFloatUniform("ViewFarValues[2]" , mCascadeFar[2].z);
}


void Renderer::SetFiltersUniforms(Shader* shader, bool sharp, bool blur, bool edgeDetect , bool gammaCorrection) 
{
	shader->SetBoolUniform("isSharpen", sharp);
	shader->SetBoolUniform("isBlur", blur); 
	shader->SetBoolUniform("isEdgeDetect", edgeDetect); 
	shader->SetBoolUniform("isGammaCorrection", gammaCorrection); 
}

void Renderer::SetLightFactors(Shader *shader , Material* material) 
{
	shader->SetFloatUniform("ambientStrength" , material->GetKaStrength()); 
	shader->SetFloatUniform("diffStrength" , material->GetKdStrength()); 
	shader->SetFloatUniform("specStrength" , material->GetKsStrength()); 
}


void Renderer::SetDrawQuad() 
{
	float quadVertices[] = {
		// Positions   // TexCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	unsigned int vbo;
	glGenVertexArrays(1, &mScreenQuadVAO);
	glBindVertexArray(mScreenQuadVAO);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glBindVertexArray(0);
}


void Renderer::DrawQuad() 
{
	glViewport(0, 0, mGame->GetWidth(), mGame->GetHeight()); 
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT); 
	glDisable(GL_DEPTH_TEST); 
	
	mScreenQuadShader->Bind(); 

	static bool blur = false; 
	static bool sharp = false; 
	static bool edgeDetect = false;
	static bool gammaCorrection = false; 

	mImGuiManager.CreateCheckBoxWindow(sharp , blur , edgeDetect , gammaCorrection); 

	SetFiltersUniforms(mScreenQuadShader, sharp, blur, edgeDetect , gammaCorrection); 

	glActiveTexture(GL_TEXTURE0); 
	glBindTexture(GL_TEXTURE_2D , mFrameBuffer.GetColorAttachment());
	mScreenQuadShader->SetTextureUniform("screenTexture", 0);

	glBindVertexArray(mScreenQuadVAO); 
	glDrawArrays(GL_TRIANGLES , 0 , 6);
	glBindVertexArray(0); 
}

void Renderer::LoadShaders() 
{

	Shader* basic = new Shader(); 
	basic->Link("Shaders/Basic.vert" , "Shaders/Basic.frag");
	mShaders["Basic"] = basic; 

	Shader* phong = new Shader(); 
	phong->Link("Shaders/BasicPhong.vert", "Shaders/BasicPhong.frag");
	mShaders["BasicPhong"] = phong; 

	mScreenQuadShader = new Shader();
	mScreenQuadShader->Link("Shaders/ScreenQuad.vert", "Shaders/ScreenQuad.frag");

	Shader* BlinPhong = new Shader(); 
	BlinPhong->Link("Shaders/BlinPhong.vert", "Shaders/BlinPhong.frag");
	mShaders["BlinPhong"] = BlinPhong;

	Shader* NormalMapping = new Shader(); 
	NormalMapping->Link("Shaders/NormalMapping.vert", "Shaders/NormalMapping.frag");
	mShaders["NormalMapping"] = NormalMapping;
}


void Renderer::SetLightUniforms(Shader* shader , Material* material)
{
	// Set Lighting Components ambient , diffuse , specular

	shader->SetVec3Uniform("LightDirection", mDirectionalLight.GetDirection()); 
	shader->SetVec3Uniform("light.position", glm::vec3(-3.0f , 10.0f , -3.0f)); 
	shader->SetVec3Uniform("light.ambient", material->GetKa());
	shader->SetVec3Uniform("light.diffuse", material->GetKd());
	shader->SetVec3Uniform("light.specular", material->GetKs());
	SetLightFactors(shader , material); 
}

void Renderer::SetMaterialUniforms(Shader* shader , Material* material , std::vector<Texture*>& textures)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]->GetID());
	shader->SetTextureUniform("material.diffuse", 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures[1]->GetID());
	shader->SetTextureUniform("material.specular", 1);
	
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, textures[2]->GetID());
	shader->SetTextureUniform("material.normals", 2);
	 
	shader->SetFloatUniform("material.shininess", material->GetNs()); 
}

Texture* Renderer::GetTexture(std::string texturePath)
{
	if (!mTextures[texturePath])
	{
		Texture* tex = new Texture();
		bool check = tex->Load(texturePath);
		if (check)
		{
			mTextures[texturePath] = tex;
			return tex;
		}

		delete tex;
		tex = nullptr;
	}

	return mTextures[texturePath];
} 

void Renderer::ShutDown() 
{
	for (auto &shader : mShaders) 
	{
		shader.second->Unload(); 
		delete shader.second; 
		shader.second = nullptr; 
	}
	mShaders.clear(); 

	for (auto &texture : mTextures) 
	{
		texture.second->Unload(); 
		delete texture.second; 
		texture.second = nullptr; 
	}
	mTextures.clear(); 

	for (auto cascade : mCascades) 
	{
		cascade->Unload(); 
		delete cascade; 
		cascade = nullptr; 
	}
	mCascades.clear(); 

	mFrameBuffer.Unload(); 
	mShadowBuffer.Unload(); 
	mImGuiManager.ShutDown(); 
}


Renderer::~Renderer()
{
}