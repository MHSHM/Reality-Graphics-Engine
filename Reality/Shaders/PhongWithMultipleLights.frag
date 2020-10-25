#version 330 core

struct Material 
{
	sampler2D ambient ; 
	sampler2D diffuse ; 
	sampler2D specular ; 
	float shininess ; 
};

struct DirLight 
{
	vec3 direction ; 

	vec3 ambient ; 
	vec3 diffuse ; 
	vec3 specular ; 
};

struct PointLight 
{
	vec3 position ; 

	float constant ; 
	float linear ; 
	float quadratic ; 

	vec3 ambient ; 
	vec3 diffuse ; 
	vec3 specular ; 
}; 

in vec2 textCoord ; 
in vec3 fragPos ; 
in vec3 fragNormal ; 

out vec4 finalColor ; 

#define NR_POINT_LIGHTS 4  

uniform PointLight pointLights[NR_POINT_LIGHTS] ; 
uniform Material material ;
uniform DirLight dirLight ; 
uniform vec3 viewPos ; 

vec3 CalcDirLight(DirLight light , vec3 normal , vec3 viewDir) ; 

vec3 CalcPointLight(PointLight pointLight , vec3 normal , vec3 FragmentPos , vec3 viewDir); 

void main()
{	
	
	vec3 norm = normalize(fragNormal) ; 
	vec3 viewDir = normalize(viewPos - fragPos) ; 

	vec3 result = CalcDirLight(dirLight , norm , viewDir) ; 
	/*
	for(int i = 0 ; i < NR_POINT_LIGHTS ; ++i)
	{
		result += CalcPointLight(pointLights[i] , norm , fragPos , viewDir) ; 
	}
	*/
	finalColor = vec4(result , 1.0) ; 

}

vec3 CalcDirLight(DirLight light , vec3 normal , vec3 viewDir) 
{
	vec3 ambient = light.ambient * vec3(texture(material.diffuse , textCoord)) ; 

	vec3 lightDir = normalize(light.direction) ; 
	float diff = max(dot(lightDir , normal) , 0.0) ;
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse , textCoord)) ; 

	vec3 reflectDir = reflect(-lightDir , normal) ; 
	float spec = pow(max(dot(viewDir , reflectDir) , 0.0) , material.shininess) ; 
	vec3 specular = light.specular * spec * vec3(texture(material.specular , textCoord)) ; 

	return (diffuse + specular) ; 
}

vec3 CalcPointLight(PointLight pointLight , vec3 normal , vec3 FragmentPos , vec3 viewDir) 
{
	vec3 ambient = pointLight.ambient * vec3(texture(material.diffuse , textCoord)) ; 

	vec3 lightDir = normalize(pointLight.position - FragmentPos) ;
	float diff = max(dot(lightDir , normal) , 0.0) ; 
	vec3 diffuse = pointLight.diffuse * diff * vec3(texture(material.diffuse , textCoord)) ; 

	vec3 reflectDir = reflect(-lightDir , normal) ; 
	float spec = pow(max(dot(viewDir , reflectDir) , 0.0) , material.shininess) ; 
	vec3 specular = pointLight.specular * spec * vec3(texture(material.specular , textCoord))  ; 

	float distance = length(pointLight.position - FragmentPos) ; 
	float attenution = 1.0 / (pointLight.constant + (pointLight.linear * distance) + (pointLight.quadratic * (distance * distance)));

	ambient *= attenution ; 
	diffuse *= attenution ; 
	specular *= attenution ; 

	return (ambient + diffuse + specular) ; 
}







