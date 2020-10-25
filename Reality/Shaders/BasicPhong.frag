#version 330 core

struct Material 
{
	sampler2D diffuse ; 
	sampler2D specular ; 
	float shininess ; 
};

struct Light 
{
	vec3 position ; 

	vec3 ambient ; 
	vec3 diffuse ; 
	vec3 specular ;  
};

struct PointLight 
{
	float constant ; 
	float linear ; 
	float quadratic ; 
}; 


in vec3 fragPos ; 
in vec3 fragNormal ;  
in vec2 textCoord ;

out vec4 color ;

uniform Material material ; 

uniform vec3 viewPos ; 

uniform Light light ; 
uniform vec3 LightDirection ; 

uniform float ambientStrength ; 
uniform float diffStrength ; 
uniform float specStrength ; 

vec3 CalcNonDirLight() ;
vec3 CalcDirLight() ; 

void main()
{ 
	color = vec4(CalcDirLight() , 1.0) ; 
}

vec3 CalcDirLight() 
{
	
	// Calculate Ambient Component
	vec3 ambientComponent = light.ambient * vec3(texture(material.diffuse , textCoord)) ; 
	
	// Calculate Diffuse Components
	vec3 norm = normalize(fragNormal) ; 
	vec3 lightDir = normalize(LightDirection) ; 
	float angle = max(dot(norm , lightDir) , 0.0) ; 
	vec3 diffuseComponent = light.diffuse * angle * vec3(texture(material.diffuse , textCoord)) ; 


	// Calculate Specular Component
	vec3 viewDir = normalize(viewPos - fragPos) ; 
	vec3 reflectDir = reflect(-lightDir , norm) ; 
	float specular = pow( max(dot(viewDir , reflectDir) , 0.0) , material.shininess) ; 
	vec3 specularComponent = light.specular * specular * vec3(texture(material.specular , textCoord))  ; 

	return ((ambientComponent * ambientStrength) + (diffuseComponent * diffStrength) + (specularComponent * specStrength)) ; 
}

vec3 CalcNonDirLight() 
{
	
	// Calculate Ambient Component
	vec3 ambientComponent = light.ambient * vec3(texture(material.diffuse , textCoord)) ; 
	
	// Calculate Diffuse Components
	vec3 norm = normalize(fragNormal) ; 
	vec3 lightDir = normalize(light.position - fragPos) ; 
	float angle = max(dot(norm , lightDir) , 0.0) ; 
	vec3 diffuseComponent = light.diffuse * angle * vec3(texture(material.diffuse , textCoord)) ; 


	// Calculate Specular Component
	vec3 viewDir = normalize(viewPos - fragPos) ; 
	vec3 reflectDir = reflect(-lightDir , norm) ; 
	float specular = pow( max(dot(viewDir , reflectDir) , 0.0) , material.shininess) ; 
	vec3 specularComponent = light.specular * specular * vec3(texture(material.specular , textCoord))  ; 

	return ((ambientComponent * ambientStrength) + (diffuseComponent * diffStrength) + (specularComponent * specStrength)) ;
}