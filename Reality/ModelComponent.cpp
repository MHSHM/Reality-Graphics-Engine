#include "ModelComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "Actor.h"
#include "Model.h"
#include "VertexArrayObject.h"
#include "Shader.h"
#include "SubMesh.h"
#include "Material.h"
#include "Texture.h"
#include "Camera.h"
#include "ShadowCascade.h"

#include <array>

ModelComponent::ModelComponent(Actor* owner):
	Component(owner)
{
	owner->GetGame()->AddModelComponent(this); 
}


ModelComponent::~ModelComponent()
{
	GetOwner()->GetGame()->RemoveModelComponent(this);
}

void ModelComponent::Draw() 
{

	// Compute Model Matrix
	GetOwner()->ComputeWorldTransform();

	Game* game = GetOwner()->GetGame();
	Renderer *renderer = GetOwner()->GetGame()->GetRenderer();
	std::unordered_map<std::string, class Shader*> shaders = game->GetRenderer()->GetShaders();

	std::vector<ShadowCascade*> cascades = renderer->GetCascades(); 

	// Get Model in Submeshes Form
	std::vector<SubMesh*> submeshes = GetModel()->GetSubMeshes();

	// Bind Shader
	Shader* shader = shaders[GetModel()->GetShaderName()];
	shader->Bind();

	// Set Transformations Uniform
	shader->SetMatrix4Uniform("uWorldTransform", GetOwner()->GetWorldTransform());
	shader->SetMatrix4Uniform("uView", renderer->GetGame()->GetCam()->GetViewMatrix());
	shader->SetMatrix4Uniform("uProjection", renderer->GetGame()->GetCam()->GetProjectionMatrix());

	//shader->SetMatrix4Uniform("uLightMatrix", renderer->GetLightTransformation());
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

	renderer->SetShadowMapUniform(shader);

	renderer->SetFarUniform(shader); 

	// Set Camera Position
	shader->SetVec3Uniform("viewPos", game->GetCam()->GetPosition());

	// Get Material Maps 
	std::unordered_map< std::string, std::vector<Texture*> > texturesByMaterial = GetModel()->GetTextures();

	// Draw each Submesh with its materials and textures
	for (auto &submesh : submeshes)
	{
		// Get Submesh Materials
		Material* material = submesh->GetMaterial();

		// Get Maps for this Material
		std::vector<Texture*> textures = texturesByMaterial[material->GetName()];

		// Set Material Uniforms
		renderer->SetMaterialUniforms(shader, material, textures);

		// Set Cubemap Texture for reflections
		//renderer->BindCubeMap(shader, 2);

		// Set Light Uniforms
		renderer->SetLightUniforms(shader, material);

		// Draw This Submesh
		submesh->GetVAO()->Bind();
		glDrawElements(GL_TRIANGLES, submesh->GetVAO()->GetEBOsize(), GL_UNSIGNED_INT, nullptr);
	}
}