#include "MeshComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "Actor.h"
#include "Mesh.h"
#include "Shader.h"
#include "VertexArrayObject.h"
#include "Texture.h"
#include "Camera.h"
#include "ShadowCascade.h"

#include <array>


MeshComponent::MeshComponent(Actor* owner):
	Component(owner)
{
	owner->GetGame()->AddMeshComponent(this); 
}


MeshComponent::~MeshComponent()
{
	GetOwner()->GetGame()->RemoveMeshComponent(this); 
}

void MeshComponent::Draw() 
{

	GetOwner()->ComputeWorldTransform();

	Game* game = GetOwner()->GetGame(); 
	Renderer *renderer = GetOwner()->GetGame()->GetRenderer(); 
	std::unordered_map<std::string, class Shader*> shaders = game->GetRenderer()->GetShaders();

	Shader* shader = shaders[GetMesh()->GetShaderName()]; 
	VertexArrayObject* vao = GetMesh()->GetVAO();
	std::vector<Texture*> textures = GetMesh()->GetTextures();
	std::vector<ShadowCascade*> cascades = renderer->GetCascades();

	shader->Bind();

	shader->SetMatrix4Uniform("uWorldTransform", GetOwner()->GetWorldTransform());
	shader->SetMatrix4Uniform("uView", renderer->GetGame()->GetCam()->GetViewMatrix());
	shader->SetMatrix4Uniform("uProjection", renderer->GetGame()->GetCam()->GetProjectionMatrix());
	
	for (int i = 0; i < 3; ++i)
	{
		glm::mat4 lightView = cascades[i]->GetLightView();
		glm::mat4 lightPtoj = cascades[i]->GetLightProj();
		glm::mat4 lightMatrix = lightPtoj * lightView;

		std::string uniform = "uLightMatrix";
		uniform += '[';
		uniform += std::to_string(i);
		uniform += ']';
		shader->SetMatrix4Uniform(uniform, lightMatrix);
	}

	// Set Shadow Map For Shadow Mapping
	renderer->SetShadowMapUniform(shader);
	
	renderer->SetFarUniform(shader); 

	// Set Camera Position for lighting calculations
	shader->SetVec3Uniform("viewPos", game->GetCam()->GetPosition());


	for (int i = 0; i < textures.size(); ++i)
	{
		textures[i]->Bind(i);
		shader->SetTextureUniform("myTexture", i);
	}

	vao->Bind();
	glDrawElements(GL_TRIANGLES, vao->GetEBOsize(), GL_UNSIGNED_INT, nullptr);
}