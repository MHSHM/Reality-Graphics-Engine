#include "Mesh.h"
#include "VertexArrayObject.h"
#include "Texture.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

Mesh::Mesh()
{
}

bool Mesh::LoadOBJ(std::string objFile) 
{
	std::vector<glm::vec3> position;
	std::vector<glm::vec2> UVs;
	std::vector<glm::vec3> normals;
	std::vector< float > vertices;
	std::vector<unsigned int> indices;
	std::unordered_map< std::string, unsigned int> isProcessed;

	std::ifstream myFile;
	myFile.open(objFile);
	if (!myFile.is_open())
	{
		std::cout << "Error Openening OBJ file : " << objFile;
		return false; 
	}

	unsigned int cnt = 1; 
	unsigned int layout = 0; 
	while (!myFile.eof())
	{
		std::string type;
		myFile >> type;
		float x, y, z;
		if (type == "v") {
			myFile >> x >> y >> z;

			glm::vec3 v(x, y, z); 
			position.emplace_back(v);

			++layout; 
		}
		else if (type == "vt") {
			myFile >> x >> y;

			glm::vec2 v(x, y);
			UVs.emplace_back(v);

			++layout; 
		}
		else if (type == "vn") {
			myFile >> x >> y >> z;
			glm::vec3 v(x, y, z);

			normals.emplace_back(v);
			++layout; 
		}
		else if (type == "f") {
			std::string p1, p2, p3, p4;
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

				if (temp[0].size() > 0) {
					int vertexIndex = std::stoi(temp[0]);
					--vertexIndex;
					vertices.emplace_back(position[vertexIndex].x);
					vertices.emplace_back(position[vertexIndex].y);
					vertices.emplace_back(position[vertexIndex].z);
				}

				if (temp[1].size() > 0) {
					int UVsIndex = std::stoi(temp[1]);
					--UVsIndex;
					vertices.emplace_back(UVs[UVsIndex].x);
					vertices.emplace_back(UVs[UVsIndex].y);
				}

				if (temp[2].size() > 0) {
					int normalIndex = std::stoi(temp[2]);
					--normalIndex;
					vertices.emplace_back(normals[normalIndex].x);
					vertices.emplace_back(normals[normalIndex].y);
					vertices.emplace_back(normals[normalIndex].z);
				}

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

				if (temp[0].size() > 0) {
					int vertexIndex = std::stoi(temp[0]);
					--vertexIndex;
					vertices.emplace_back(position[vertexIndex].x);
					vertices.emplace_back(position[vertexIndex].y);
					vertices.emplace_back(position[vertexIndex].z);
				}

				if (temp[1].size() > 0) {
					int UVsIndex = std::stoi(temp[1]);
					--UVsIndex;
					vertices.emplace_back(UVs[UVsIndex].x);
					vertices.emplace_back(UVs[UVsIndex].y);
				}

				if (temp[2].size() > 0) {
					int normalIndex = std::stoi(temp[2]);
					--normalIndex;
					vertices.emplace_back(normals[normalIndex].x);
					vertices.emplace_back(normals[normalIndex].y);
					vertices.emplace_back(normals[normalIndex].z);
				}

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

				if (temp[0].size() > 0) {
					int vertexIndex = std::stoi(temp[0]);
					--vertexIndex;
					vertices.emplace_back(position[vertexIndex].x);
					vertices.emplace_back(position[vertexIndex].y);
					vertices.emplace_back(position[vertexIndex].z);
				}

				if (temp[1].size() > 0) {
					int UVsIndex = std::stoi(temp[1]);
					--UVsIndex;
					vertices.emplace_back(UVs[UVsIndex].x);
					vertices.emplace_back(UVs[UVsIndex].y);
				}

				if (temp[2].size() > 0) {
					int normalIndex = std::stoi(temp[2]);
					--normalIndex;
					vertices.emplace_back(normals[normalIndex].x);
					vertices.emplace_back(normals[normalIndex].y);
					vertices.emplace_back(normals[normalIndex].z);
				}

				++cnt;

			}
			else {
				indices.emplace_back(isProcessed[p3] - 1);
			}

		}
		else if (type == "Shader") 
		{
			myFile >> mShaderName; 
		}
		
	}

	//mVAO = new VertexArrayObject(vertices , indices , static_cast<unsigned int>(indices.size())); 

	indices.clear();
	vertices.clear();
	position.clear();
	UVs.clear();
	normals.clear();
	isProcessed.clear();

	myFile.close();
	return true ; 
}

void Mesh::Unload() 
{
	delete mVAO; 
	mVAO = nullptr;
}

Mesh::~Mesh()
{ 
}
