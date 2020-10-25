#pragma once
#include <unordered_map>
class ImGuiManager
{
public:
	ImGuiManager();
	~ImGuiManager();

	void Initialize(class Game* game);

	void CreateFrameRateWindow(); 

	void CreateNewFrame(); 
	void EndFrame(); 

	void BeginWindow(std::string name); 

	void EndWindow();

	void CreateCheckBox(std::string name , bool &flag); 

	void CreateCheckBoxWindow(bool& va1, bool& va2, bool& va3 , bool& va4);

	void CreateModelWindow(class ModelComponent* modelCMP , std::unordered_map<std::string, class Shader*>& shaders);

	void CreateCameraWindow(class Camera* camera); 

	void CreateModelWindow(class MeshComponent* mshCMP, std::unordered_map<std::string, class Shader*>& shaders);

	void CreateDragFloat(std::string&& name, float& val, float &&speed, float &&min, float &&max);

	void Draw(); 

	void ShutDown(); 
};

