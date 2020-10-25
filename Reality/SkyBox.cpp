#include "SkyBox.h"
#include "GL/glew.h"
#include "SOIL2/SOIL2.h"
#include "Game.h"
#include "Renderer.h"
#include "Camera.h"

SkyBox::SkyBox():
	mVAO(0),
	mVBO(0),
	mCubeMapID(0)
{
}


void SkyBox::Initilaize()
{
	// Load Sky Box Shader ; 
	mShader.Link("Shaders/Skybox.vert", "Shaders/Skybox.frag"); 

	// Create VBO , VAO 
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	glGenBuffers(1, &mVBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindVertexArray(0); 
}

void SkyBox::SetFaces() 
{
	// Get Faces
	mFaces.reserve(6);
	mFaces.push_back("Assets/Skybox/Right.png");
	mFaces.push_back("Assets/Skybox/Left.png");
	mFaces.push_back("Assets/Skybox/Top.png");
	mFaces.push_back("Assets/Skybox/Bottom.png");
	mFaces.push_back("Assets/Skybox/Back.png");
	mFaces.push_back("Assets/Skybox/Front.png");

	// Generate Cube map Texture
	glGenTextures(1, &mCubeMapID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mCubeMapID);

	int width, height;
	unsigned char* image;
	for (int i = 0; i < mFaces.size(); ++i)
	{
		image = SOIL_load_image(mFaces[i].c_str(), &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

}

void SkyBox::Bind(Shader* shader , int unit) 
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mCubeMapID);
	shader->SetTextureUniform("skybox", unit);
}

void SkyBox::Draw(Renderer* renderer) 
{
	renderer->GetFrameBuffer().Bind();
	glViewport(0, 0, renderer->GetGame()->GetWidth(), renderer->GetGame()->GetHeight());
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	mShader.Bind(); 
	mShader.SetMatrix4Uniform("view", renderer->GetGame()->GetCam()->GetViewMatrix());
	mShader.SetMatrix4Uniform("projection", renderer->GetGame()->GetCam()->GetProjectionMatrix());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mCubeMapID);

	glBindVertexArray(mVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

SkyBox::~SkyBox()
{
}
