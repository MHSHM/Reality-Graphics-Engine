#pragma once

#include<string>

#include "glm/vec3.hpp"

class Material
{
public:
	Material(std::string& name , float ns , glm::vec3& ka , glm::vec3& kd ,
		glm::vec3& ks , std::string& kaMap , std::string& kdMap , std::string& ksMap);

	Material(std::string& name , float ns , glm::vec3& ka , glm::vec3& kd ,
		glm::vec3& ks , std::string& kaMap , std::string& kdMap , std::string& ksMap , std::string& normalMap);
	
	~Material(); 

	void SetName(std::string name) { mName = name;  }
	void SetNs(float val) { mNs = val; }
	void SetKa(glm::vec3 ka) { mKa = ka;  }
	void SetKd(glm::vec3 kd) { mKd = kd;  }
	void SetKs(glm::vec3 ks) { mKs = ks;  }

	glm::vec3 GetKa() { return mKa;  }
	glm::vec3 GetKd() { return mKd;  }
	glm::vec3 GetKs() { return mKs;  }

	void SetKaMap(std::string& map) { mKaMap = map;  }
	void SetKdMap(std::string& map) { mKdMap = map;  }
	void SetKsMap(std::string& map) { mKsMap = map;  }
	void SetNormalMap(std::string& map) { mNormal = map;  }
	
	void SetKaStrength(float &val) { mKaStrength = val;  }
	void SetKdStrength(float &val) { mKdStrength = val;  }
	void SetKsStrength(float &val) { mKsStrength = val;  }

	std::string& GetName() { return mName;  }

	std::string& GetKaMap() { return mKaMap;  }
	std::string& GetKdMap() { return mKdMap;  }
	std::string& GetKsMap() { return mKsMap;  }
	std::string& GetNormalMap() { return mNormal;  }

	float GetNs() { return mNs;  }
	float GetKaStrength() { return mKaStrength;  }
	float GetKdStrength() { return mKdStrength;  }
	float GetKsStrength() { return mKsStrength;  }
private:

	std::string mName; 

	float mNs; 
	glm::vec3 mKa; 
	glm::vec3 mKd; 
	glm::vec3 mKs;

	std::string mKaMap; 
	std::string mKdMap; 
	std::string mKsMap; 
	std::string mNormal; 

	float mKaStrength; 
	float mKdStrength; 
	float mKsStrength; 

};

