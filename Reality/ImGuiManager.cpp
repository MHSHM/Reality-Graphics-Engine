#include "ImGuiManager.h"
#include "Game.h"
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_opengl3.h"
#include "ImGUI/imgui_impl_glfw.h"
#include "Actor.h"
#include "Model.h"
#include "ModelComponent.h"
#include "MeshComponent.h"
#include "Mesh.h"
#include "Shader.h"
#include "Material.h"
#include "Camera.h"

#include <string>

ImGuiManager::ImGuiManager()
{
}

void ImGuiManager::Initialize(Game* game) 
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(game->GetWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 330");
	ImGui::StyleColorsDark();
}

void ImGuiManager::CreateNewFrame() 
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImGuiManager::EndFrame() 
{
	ImGui::EndFrame(); 
}

void ImGuiManager::BeginWindow(std::string name) 
{
	ImGui::Begin(name.c_str()); 
}

void ImGuiManager::EndWindow() 
{
	ImGui::End(); 
}

void ImGuiManager::CreateDragFloat(std::string&& name ,  float& val , float &&speed , float &&min , float &&max) 
{
	ImGui::DragFloat(name.c_str() , &val , speed , min , max); 
}

void ImGuiManager::CreateCheckBox(std::string name , bool &flag) 
{
	
	ImGui::Checkbox(name.c_str() , &flag);

}

void ImGuiManager::CreateCameraWindow(Camera* camera) 
{
	float sensitivity = camera->GetSensitivity(); 
	float speed = camera->GetSpeed(); 
	glm::vec3 pos = camera->GetPos(); 
	glm::vec3 forward = camera->GetForward(); 
	glm::vec3 right = camera->GetRight(); 
	glm::vec3 up = camera->GetUp(); 

	ImGui::Begin("Camera"); 
	ImGui::DragFloat("Sensitivity", &sensitivity, 1.0f, 10.0f, 500.0f); 
	ImGui::DragFloat("Speed", &speed, 1.0f, 0.0f, 30.0f); 
	ImGui::DragFloat3("Position", &pos.x, 0.0f); 
	ImGui::DragFloat3("Forward", &forward.x, 0.0f); 
	ImGui::DragFloat3("Right", &right.x, 0.0f); 
	ImGui::DragFloat3("Up", &up.x, 0.0f); 
	ImGui::End(); 

	camera->SetSensitivity(sensitivity); 
	camera->SetSpeed(speed); 
}

void ImGuiManager::CreateModelWindow(class MeshComponent* mshCMP, std::unordered_map<std::string, class Shader*>& shaders) 
{

	glm::vec3 pos = mshCMP->GetOwner()->GetPosition();
	float scale = mshCMP->GetOwner()->GetScale();
	float angle = mshCMP->GetOwner()->GetAngle();
	glm::vec3 axis = mshCMP->GetOwner()->GetRotationAxis();

	std::string modelName = mshCMP->GetOwner()->GetName();



	ImGui::Begin(modelName.c_str());
	ImGui::DragFloat3("Position", &pos.x, 0.1f, -100.0f, 100.0f);
	ImGui::DragFloat("Angle", &angle, 1.0f, -360.0f, 360.0f);
	ImGui::DragFloat3("Rotation Axis", &axis.x, 1.0f, -1.0f, 1.0f);
	ImGui::DragFloat("Scale", &scale, 0.1f, -10.0f, 10.0f);


	if (ImGui::Button("Select Shader.."))
		ImGui::OpenPopup(mshCMP->GetOwner()->GetName().c_str());
	ImGui::SameLine();
	ImGui::TextUnformatted(mshCMP->GetMesh()->GetShaderName().c_str());
	if (ImGui::BeginPopup(mshCMP->GetOwner()->GetName().c_str()))
	{
		ImGui::Separator();
		for (auto &shader : shaders)
		{
			std::string shaderName = shader.first;
			if (ImGui::Selectable(shaderName.c_str())) mshCMP->GetMesh()->SetShader(shaderName);
		}
		ImGui::EndPopup();
	}



	ImGui::End();

	mshCMP->GetOwner()->SetPosition(pos);
	mshCMP->GetOwner()->SetScale(scale);
	mshCMP->GetOwner()->SetRotation(angle, axis);
}

void ImGuiManager::CreateModelWindow(ModelComponent *modelCMP , std::unordered_map<std::string,Shader*>& shaders)
{

	glm::vec3 pos = modelCMP->GetOwner()->GetPosition();
	float scale = modelCMP->GetOwner()->GetScale();
	float angle = modelCMP->GetOwner()->GetAngle();
	glm::vec3 axis = modelCMP->GetOwner()->GetRotationAxis();

	std::string modelName = modelCMP->GetOwner()->GetName();
	std::unordered_map<std::string, class Material*> materials = modelCMP->GetModel()->GetMaterials(); 

	ImGui::Begin(modelName.c_str());
	ImGui::DragFloat3("Position", &pos.x, 0.1f, -100.0f, 100.0f);
	ImGui::DragFloat("Angle", &angle, 1.0f, -360.0f, 360.0f);
	ImGui::DragFloat3("Rotation Axis", &axis.x, 1.0f, -1.0f, 1.0f);
	ImGui::DragFloat("Scale", &scale, 0.1f, -10.0f, 10.0f);

	if (ImGui::Button("Select Shader.."))
		ImGui::OpenPopup(modelCMP->GetOwner()->GetName().c_str());
	ImGui::SameLine();
	ImGui::TextUnformatted(modelCMP->GetModel()->GetShaderName().c_str());
	if (ImGui::BeginPopup(modelCMP->GetOwner()->GetName().c_str()))
	{
		ImGui::Separator();
		for (auto &shader : shaders)
		{
			std::string shaderName = shader.first;
			if (ImGui::Selectable(shaderName.c_str())) modelCMP->GetModel()->SetShader(shaderName);
		}
		ImGui::EndPopup();
	}

	for (auto& material : materials) 
	{
		ImGui::Text(material.first.c_str());
		Material* mat = material.second;  
		float shininess = mat->GetNs(); 
		float kaStreng = mat->GetKaStrength();
		float kdStreng = mat->GetKdStrength();
		float ksStreng = mat->GetKsStrength();
		ImGui::DragFloat("Shininess", &shininess, 1.0f, 0.0f, 512.0f); 
		ImGui::DragFloat("Ambient", &kaStreng, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("Diffuse", &kdStreng, 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("Specular", &ksStreng, 0.01f, 0.0f, 1.0f);
		mat->SetNs(shininess); 
		mat->SetKaStrength(kaStreng);
		mat->SetKdStrength(kdStreng); 
		mat->SetKsStrength(ksStreng); 
	}

	ImGui::End();

	modelCMP->GetOwner()->SetPosition(pos);
	modelCMP->GetOwner()->SetScale(scale);
	modelCMP->GetOwner()->SetRotation(angle, axis);

}

void ImGuiManager::Draw() 
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiManager::CreateFrameRateWindow() 
{
	ImGui::Begin("Frame Rate");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}

void ImGuiManager::CreateCheckBoxWindow(bool& va1, bool& va2,bool& va3 , bool& va4)
{
	ImGui::Begin("Filters");
	ImGui::Checkbox("Sharp", &va1);
	ImGui::Checkbox("Blur", &va2);
	ImGui::Checkbox("Edge Detection", &va3);
	ImGui::Checkbox("Gamma Correction", &va4);
	ImGui::End();
}

void ImGuiManager::ShutDown() 
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

ImGuiManager::~ImGuiManager()
{
}
