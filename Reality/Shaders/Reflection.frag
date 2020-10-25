#version 330

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

in vec3 fragPos ; 
in vec3 fragNormal ;  
in vec2 textCoord ;

uniform Material material ; 
uniform samplerCube skybox;

uniform vec3 viewPos ; 

uniform Light light ; 
uniform vec3 LightDirection ; 
uniform bool dirLight ;
uniform bool nonDirLight ;

uniform float ambientStrength ; 
uniform float diffStrength ; 
uniform float specStrength ; 

vec3 CalcReflectDir() ; 
vec3 CalcNonDirLight() ;
vec3 CalcDirLight() ; 

out vec4 color ; 

void main()
{
	
	// Final object Color
	vec3 objectColor = vec3(0.0 , 0.0 , 0.0) ;
	
	if(dirLight)
	{
		objectColor += CalcDirLight() ; 
	}

	if(nonDirLight)
	{
		objectColor += CalcNonDirLight() ; 
	}
	
	vec3 reflectDir = CalcReflectDir() ;
	vec4 reflectColor = texture(skybox , reflectDir) ;
	objectColor += vec3(reflectColor) ; 
	color = vec4(objectColor , 1.0) ; 
}

vec3 CalcReflectDir() 
{
	vec3 viewDir = normalize(fragPos - viewPos) ; 
	vec3 norm = normalize(fragNormal) ; 
	vec3 reflectDir = reflect(viewDir , norm) ;
	return reflectDir ; 
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