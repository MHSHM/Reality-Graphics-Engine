#include "Material.h"
#include "Texture.h"


Material::Material(std::string& name, float ns, glm::vec3& ka, glm::vec3& kd,
	glm::vec3& ks, std::string& kaMap, std::string& kdMap, std::string& ksMap):
	mName(name),
	mNs(ns),
	mKa(ka),
	mKd(kd),
	mKs(ks),
	mKaMap(kaMap),
	mKdMap(kdMap),
	mKsMap(ksMap),
	mKaStrength(1.0f),
	mKdStrength(1.0f),
	mKsStrength(1.0f)
{
}

Material::Material(std::string& name, float ns, glm::vec3& ka, glm::vec3& kd,
	glm::vec3& ks, std::string& kaMap, std::string& kdMap, std::string& ksMap, std::string& normalMap):
	mName(name),
	mNs(ns),
	mKa(ka),
	mKd(kd),
	mKs(ks),
	mKaMap(kaMap),
	mKdMap(kdMap),
	mKsMap(ksMap),
	mKaStrength(1.0f),
	mKdStrength(1.0f),
	mKsStrength(1.0f),
	mNormal(normalMap)
{
}


Material::~Material()
{
}
