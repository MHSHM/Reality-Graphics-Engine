#include "Model.h"
#include "SubMesh.h"
#include "Texture.h"
#include "Material.h"
#include "SubMesh.h"

#include <fstream>
#include <iostream>


Model::Model():
	mTangent(false)
{
}

bool Model::LoadSubMeshes(std::string file , bool calcTangent) 
{

	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> UVs;
	std::vector<glm::vec3> normals;
	std::vector< Vertex > vertices;
	std::vector<unsigned int> indices;
	std::unordered_map< std::string, unsigned int> isProcessed;
	unsigned int layout = 3;

	std::ifstream myFile;
	myFile.open(file);
	if (!myFile.is_open())
	{
		std::cout << "Error Openening OBJ file " << file;
		return false; 
	}

	unsigned int cnt = 1;
	std::string materialName;
	while (!myFile.eof())
	{
		std::string type;
		myFile >> type;
		float x, y, z;
		if (type == "mtllib") 
		{
			std::string fileName; 
			myFile >> fileName; 
			std::string materialPath = "Assets/Materials/"; 
			materialPath += fileName; 
			LoadMtlFile(materialPath); 
		}
		else if (type == "v") {
			myFile >> x >> y >> z;

			glm::vec3 v(x, y, z);
			positions.emplace_back(v);
		}
		else if (type == "vt") {
			myFile >> x >> y;

			glm::vec2 v(x, y);
			UVs.emplace_back(v); 
		}
		else if (type == "vn") {
			myFile >> x >> y >> z;
			glm::vec3 v(x, y, z);

			normals.emplace_back(v);
		}
		else if (type == "usemtl") {

			if (indices.size() > 0) {
				
				SubMesh* submesh = new SubMesh(mMaterials[materialName] , vertices , indices , layout);
				mSubMeshes.emplace_back(submesh); 
				
				myFile >> materialName;
			
			}
			else {

				myFile >> materialName;
			
			}

		}
		else if (type == "f") {
			std::string p1, p2, p3; 
			std::vector<std::string> temp(3);

			myFile >> p1;
			if (!isProcessed[p1]) {

				isProcessed[p1] = cnt;
				indices.emplace_back(cnt - 1);

				temp[0] = "";
				temp[1] = "";
				temp[2] = "";

				int c1 = 0;
				for (int i = 0; i < p1.size(); ++i) {
					if (p1[i] == '/') {
						++c1;
						continue;
					}
					temp[c1] += p1[i];
				}

				Vertex vertex; 

				// Get Vertex Position
				if (temp[0].size() > 0) {
					int vertexIndex = std::stoi(temp[0]);
					--vertexIndex;
					vertex.pos.x = positions[vertexIndex].x;
					vertex.pos.y = positions[vertexIndex].y;
					vertex.pos.z = positions[vertexIndex].z;
				}

				// Get Vertex Texture Coordinates
				if (temp[1].size() > 0) {
					int UVsIndex = std::stoi(temp[1]);
					--UVsIndex;
					vertex.UVs.x = UVs[UVsIndex].x;
					vertex.UVs.y = UVs[UVsIndex].y;
				}

				// Get Vertex Normal
				if (temp[2].size() > 0) {
					int normalIndex = std::stoi(temp[2]);
					--normalIndex;
					vertex.normal.x = normals[normalIndex].x; 
					vertex.normal.y = normals[normalIndex].y; 
					vertex.normal.z = normals[normalIndex].z; 
				}

				vertices.emplace_back(vertex); 
				++cnt;

			}
			else {
				indices.emplace_back(isProcessed[p1] - 1);
			}

			myFile >> p2;
			if (!isProcessed[p2]) {

				isProcessed[p2] = cnt;
				indices.emplace_back(cnt - 1);

				temp[0] = "";
				temp[1] = "";
				temp[2] = "";

				int c1 = 0;
				for (int i = 0; i < p2.size(); ++i) {
					if (p2[i] == '/') {
						++c1;
						continue;
					}
					temp[c1] += p2[i];
				}

				Vertex vertex;

				if (temp[0].size() > 0) {
					int vertexIndex = std::stoi(temp[0]);
					--vertexIndex;
					vertex.pos.x = positions[vertexIndex].x;
					vertex.pos.y = positions[vertexIndex].y;
					vertex.pos.z = positions[vertexIndex].z;
				}

				if (temp[1].size() > 0) {
					int UVsIndex = std::stoi(temp[1]);
					--UVsIndex;
					vertex.UVs.x = UVs[UVsIndex].x;
					vertex.UVs.y = UVs[UVsIndex].y;
				}

				if (temp[2].size() > 0) {
					int normalIndex = std::stoi(temp[2]);
					--normalIndex;
					vertex.normal.x = normals[normalIndex].x;
					vertex.normal.y = normals[normalIndex].y;
					vertex.normal.z = normals[normalIndex].z;
				}

				vertices.emplace_back(vertex);
				++cnt;

			}
			else {
				indices.emplace_back(isProcessed[p2] - 1);
			}

			myFile >> p3;
			if (!isProcessed[p3]) {

				isProcessed[p3] = cnt;
				indices.emplace_back(cnt - 1);

				temp[0] = "";
				temp[1] = "";
				temp[2] = "";

				int c1 = 0;
				for (int i = 0; i < p3.size(); ++i) {
					if (p3[i] == '/') {
						++c1;
						continue;
					}
					temp[c1] += p3[i];
				}

				Vertex vertex;

				if (temp[0].size() > 0) {
					int vertexIndex = std::stoi(temp[0]);
					--vertexIndex;
					vertex.pos.x = positions[vertexIndex].x;
					vertex.pos.y = positions[vertexIndex].y;
					vertex.pos.z = positions[vertexIndex].z;
				}

				if (temp[1].size() > 0) {
					int UVsIndex = std::stoi(temp[1]);
					--UVsIndex;
					vertex.UVs.x = UVs[UVsIndex].x;
					vertex.UVs.y = UVs[UVsIndex].y;
				}

				if (temp[2].size() > 0) {
					int normalIndex = std::stoi(temp[2]);
					--normalIndex;
					vertex.normal.x = normals[normalIndex].x;
					vertex.normal.y = normals[normalIndex].y;
					vertex.normal.z = normals[normalIndex].z;
				}

				vertices.emplace_back(vertex);
				++cnt;

				
			}
			else {
				indices.emplace_back(isProcessed[p3] - 1);
			}


			if (calcTangent)
			{

				layout = 4; 

				// Calculate Tangent Vector
				Vertex& v0 = vertices[indices[indices.size() - 3]];
				Vertex& v1 = vertices[indices[indices.size() - 2]];
				Vertex& v2 = vertices[indices[indices.size() - 1]];

				glm::vec3 edge1 = v1.pos - v0.pos;
				glm::vec3 edge2 = v2.pos - v0.pos;

				glm::vec2 deltaUV1 = v1.UVs - v0.UVs;
				glm::vec2 deltaUV2 = v2.UVs - v0.UVs;


				float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

				glm::vec3 tangent;

				tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
				tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
				tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

				// Averaging the tangent vector in case of more than one triangle 
				// share the same vertex
				if (!v0.check) v0.tangent = tangent, v0.check = true;
				else v0.tangent += tangent;

				if (!v1.check) v1.tangent = tangent, v1.check = true;
				else v1.tangent += tangent;

				if (!v2.check) v2.tangent = tangent, v2.check = true;
				else v2.tangent += tangent;
			}
			
		}
		else if (type == "Shader") 
		{
			myFile >> mShader; 
		}
	}

	SubMesh* submesh = new SubMesh(mMaterials[materialName], vertices, indices , layout);
	mSubMeshes.emplace_back(submesh); 
	
	indices.clear(); 
	vertices.clear(); 
	positions.clear();
	UVs.clear(); 
	normals.clear(); 
	isProcessed.clear(); 

	myFile.close(); 
	return true; 
}

void Model::LoadTextures() 
{
	for (auto material : mMaterials) 
	{
		Material* mat = material.second; 
		if (mat) 
		{
			// Load Kd Map
			std::string kdPath = "Assets/Textures/";
			kdPath += mat->GetKdMap();
			Texture* kdMap = new Texture();
			if (kdMap->Load(kdPath))
			{
				mTextures[mat->GetName()].emplace_back(kdMap);
			}

			// Load Ks Map
			std::string ksPath = "Assets/Textures/";
			ksPath += mat->GetKsMap();
			Texture* ksMap = new Texture();
			if (ksMap->Load(ksPath))
			{
				mTextures[mat->GetName()].emplace_back(ksMap);
			}

			// Load Normal Map
			std::string normalPath = "Assets/Textures/";
			normalPath += mat->GetNormalMap();
			Texture* normalMap = new Texture();
			if (normalMap->Load(normalPath))
			{
				mTextures[mat->GetName()].emplace_back(normalMap);
			}
		}
	}
}


bool Model::LoadMtlFile(std::string file) 
{
	std::ifstream mtlFile; 
	mtlFile.open(file); 
	if (!mtlFile.is_open()) 
	{
		std::cout << "Failed to open mtl file : " << file; 
		return false; 
	}

	float ns = 1.0f;

	glm::vec3 ka(1.0f);
	glm::vec3 kd(1.0f);
	glm::vec3 ks(1.0f);

	std::string kaMap = "Default.png"; 
	std::string kdMap = "Default.png"; 
	std::string ksMap = "Default.png";
	std::string normalMap = "Default.png"; 

	bool checkFirstMaterial = false; 
	std::string materialName;
	std::string type;

	while (!mtlFile.eof()) {

		mtlFile >> type; 
		

		if (type == "newmtl") {


			if (!checkFirstMaterial) 
			{
				mtlFile >> materialName; 
				checkFirstMaterial = true; 
			}
			else 
			{ 
				Material* material = new Material(materialName, ns, ka, kd, ks, kaMap, kdMap, ksMap);
	
				mMaterials[materialName] = material; 

				ns = 1.0f;

				ka = glm::vec3(1.0f); 
				kd = glm::vec3(1.0f); 
				ks = glm::vec3(1.0f); 

				kaMap = "Default.png";
				kdMap = "Default.png";
				ksMap = "Default.png";
				normalMap = "Default.png";

				mtlFile >> materialName; 

			}
		}
		else if(type == "Ns") 
		{
			float val; 
			mtlFile >> val; 
			ns = val; 
		}
		else if (type == "Ka") 
		{
			float x, y, z; 
			mtlFile >> x >> y >> z; 
			ka = glm::vec3(x, y, z); 
		}
		else if (type == "Kd") 
		{
			float x, y, z; 
			mtlFile >> x >> y >> z; 
			kd = glm::vec3(x, y, z); 
		}
		else if (type == "Ks") 
		{
			float x, y, z; 
			mtlFile >> x >> y >> z; 
			ks = glm::vec3(x, y, z); 
		}
		else if (type == "map_Ka") 
		{
			mtlFile >> kaMap;
		}else if (type == "map_Ks") 
		{
			mtlFile >> ksMap;
		}else if (type == "map_Kd") 
		{
			mtlFile >> kdMap;
		}
		else if (type == "map_normals") 
		{
			mtlFile >> normalMap; 
		}
	}
	
	Material* material = new Material(materialName, ns, ka, kd, ks, kaMap, kdMap, ksMap , normalMap);

	mMaterials[materialName] = material; 
	mtlFile.close(); 
	return true; 
}

void Model::Unload() 
{

	for (auto &submesh : mSubMeshes) 
	{
		submesh->Unload(); 
		delete submesh; 
		submesh = nullptr; 
	}
	mSubMeshes.clear(); 

	for (auto &material : mMaterials) 
	{
		delete material.second; 
		material.second = nullptr; 
	}
	mMaterials.clear(); 

	for (auto &textures : mTextures) 
	{
		for (int i = 0; i < textures.second.size(); ++i) 
		{
			textures.second[i]->Unload(); 
			delete textures.second[i]; 
			textures.second[i] = nullptr; 
		}
	}
	mTextures.clear(); 
}

Model::~Model()
{
}
