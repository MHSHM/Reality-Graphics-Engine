#pragma once
#include "GL/glew.h"
#include "glm/mat4x4.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <string>
class Shader
{
public:
	Shader();
	~Shader();

	// Load Compile Link Shader
	unsigned int LoadCompileShader(std::string shaderPath, unsigned int type);
	void Link(std::string vertexPath, std::string fragmentPath);
	void Link(std::string vertexPath, std::string fragmentPath , std::string geometryPath);

	void Unload(); 

	// Use Shader Program
	void Bind(); 
	
	// Set Uniforms
	void SetMatrix4Uniform(std::string uniform , glm::mat4& matrix);
	void SetTextureUniform(std::string uniform, int val); 
	void SetVec3Uniform(std::string unfirom , glm::vec3 vector);
	void SetFloatUniform(std::string uniform , float val); 
	void SetBoolUniform(std::string uniform, bool val); 

private:
	
	unsigned int mShaderProgramID; 
	unsigned int mFragmentShaderID; 
	unsigned int mVertexShaderID; 
	unsigned int mGeometryShader; 
 
};

