#include "Shader.h"
#include <iostream>
#include <fstream> 
#include <sstream>
#include <vector>


Shader::Shader():
	mShaderProgramID(0),
	mVertexShaderID(0),
	mFragmentShaderID(0),
	mGeometryShader(0)
{
}

// Load Compile 
unsigned int Shader::LoadCompileShader(std::string shaderPath , unsigned int type) 
{
	std::ifstream shaderFile(shaderPath);
	std::string content;

	while (shaderFile.good()) {
		std::string line;
		std::getline(shaderFile, line);
		content.append(line + "\n");
	}

	shaderFile.close();

	unsigned int id = glCreateShader(type);
	const char* src = content.c_str(); 
	const int len = content.length(); 
	glShaderSource(id, 1, &src, &len); 
	glCompileShader(id); 

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(id, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
		infoLog << std::endl;
		return 0; 
	}

	return id; 
}

// Link Use
void Shader::Link(std::string vertexPath , std::string fragmentPath) 
{
	mShaderProgramID = glCreateProgram(); 
	mVertexShaderID = LoadCompileShader(vertexPath , GL_VERTEX_SHADER);
	mFragmentShaderID = LoadCompileShader(fragmentPath, GL_FRAGMENT_SHADER); 

	glAttachShader(mShaderProgramID, mVertexShaderID); 
	glAttachShader(mShaderProgramID, mFragmentShaderID);

	glLinkProgram(mShaderProgramID); 
	glValidateProgram(mShaderProgramID);

	glUseProgram(mShaderProgramID); 
}

void Shader::Link(std::string vertexPath , std::string fragmentPath , std::string geometryPath)
{
	mShaderProgramID = glCreateProgram(); 
	mVertexShaderID = LoadCompileShader(vertexPath , GL_VERTEX_SHADER);
	mGeometryShader = LoadCompileShader(geometryPath, GL_GEOMETRY_SHADER); 
	mFragmentShaderID = LoadCompileShader(fragmentPath, GL_FRAGMENT_SHADER); 

	glAttachShader(mShaderProgramID, mVertexShaderID); 
	glAttachShader(mShaderProgramID, mGeometryShader); 
	glAttachShader(mShaderProgramID, mFragmentShaderID);

	glLinkProgram(mShaderProgramID); 
	glValidateProgram(mShaderProgramID);

	glUseProgram(mShaderProgramID); 
}

void Shader::Bind() 
{
	glUseProgram(mShaderProgramID); 
}

void Shader::SetMatrix4Uniform(std::string uniform , glm::mat4& matrix) 
{
	GLuint location = glGetUniformLocation(mShaderProgramID, uniform.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix)); 
}

void Shader::SetTextureUniform(std::string uniform , int val) 
{
	GLuint location = glGetUniformLocation(mShaderProgramID , uniform.c_str());
	glUniform1i(location, val);
}

void Shader::SetVec3Uniform(std::string uniform , glm::vec3 vector) 
{
	GLuint location = glGetUniformLocation(mShaderProgramID, uniform.c_str()); 
	glUniform3f(location, vector.x, vector.y, vector.z); 
}

void Shader::SetFloatUniform(std::string uniform , float val) 
{
	GLuint location = glGetUniformLocation(mShaderProgramID, uniform.c_str());
	glUniform1f(location, val); 
}

void Shader::SetBoolUniform(std::string uniform , bool val) 
{
	GLuint location = glGetUniformLocation(mShaderProgramID , uniform.c_str());
	glUniform1i(location , val); 
}

void Shader::Unload() 
{
	glDeleteProgram(mShaderProgramID);
	glDeleteShader(mVertexShaderID); 
	glDeleteShader(mFragmentShaderID); 
}

Shader::~Shader()
{
}
