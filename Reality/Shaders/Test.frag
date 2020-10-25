#version 330 core

struct Light 
{
	vec3 position ; 

	vec3 ambient ; 
	vec3 diffuse ; 
	vec3 specular ;  
};  

in vec3 fragPos ; 
in vec3 fragNormal ; 

uniform Light light ; 
uniform vec3 lightColor ; 
uniform vec3 viewPos ; 
uniform float shininess ; 

out vec4 finalColor ; 

void main()
{
	float strength = 0.3 ; 
	vec3 ambient = lightColor * light.ambient * strength ; 

	vec3 norm = normalize(fragNormal) ; 
	vec3 lightDir = normalize(light.position - fragPos) ; 
	float diff = max(dot(lightDir , norm) , 0.0) ;
	vec3 diffuse = lightColor * diff * light.diffuse ; 

	vec3 viewDir = normalize(viewPos - fragPos) ;
	vec3 reflectDir = reflect(-lightDir , norm) ;
	float spec = pow(max(dot(viewDir , reflectDir) , 0.0) , shininess) ; 
	vec3 specular = lightColor * spec * light.specular ; 

	vec3 objColor = ambient + diffuse + specular ; 
	finalColor = vec4(objColor , 1.0) ; 
}