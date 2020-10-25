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

struct DirLight 
{
	vec3 dir ;

	vec3 ambient ; 
	vec3 diffuse ; 
	vec3 specular ;  
};

const int MAX_SPLITS = 3 ; 

in vec3 fragPos ; 
in vec3 fragNormal ;  
in vec2 textCoord ;
in vec4 fragPosLightSpace[MAX_SPLITS] ; 
in float FragViewZ ; 

out vec4 color ;


uniform Material material ; 

uniform sampler2D shadowMap[MAX_SPLITS] ; 

uniform float ViewFarValues[MAX_SPLITS] ; 

uniform vec3 viewPos ; 

uniform Light light ; 
uniform vec3 LightDirection ; 

uniform float ambientStrength ; 
uniform float diffStrength ; 
uniform float specStrength ; 

float ShadowCalculation(int index , vec4 fragPosLightSpace) ; 
vec3 CalcNonDirLight() ;
vec3 CalcDirLight(float shadowVal) ; 


void main()
{
	float shadowVal = 0.0f ; 
	for(int i = 0 ; i < MAX_SPLITS ; ++i)
	{
		if(abs(FragViewZ) <= ViewFarValues[i])
		{
			shadowVal = ShadowCalculation(i , fragPosLightSpace[i]) ;
			break ; 
		}
	}

	color = vec4(CalcDirLight(shadowVal) , 1.0) ;
}

vec3 CalcDirLight(float shadowVal) 
{
	
	// Calculate Ambient Component
	vec3 ambientComponent = light.ambient * vec3(texture(material.diffuse , textCoord)) ; 
	
	// Get Normal from normal map if there is one
	vec3 norm = normalize(fragNormal) ; 


	// Calculate Diffuse Components
	vec3 lightDir = normalize(LightDirection) ; 
	float angle = max(dot(norm , lightDir) , 0.0) ; 
	vec3 diffuseComponent = light.diffuse * angle * vec3(texture(material.diffuse , textCoord)) ; 


	// Calculate Specular Component
	vec3 viewDir = normalize(viewPos - fragPos) ; 
	vec3 halfWay = normalize(viewDir + lightDir) ; 
	float specular = pow( max(dot(norm , halfWay) , 0.0) , material.shininess) ; 
	vec3 specularComponent = light.specular * specular * vec3(texture(material.specular , textCoord))  ; 

	float shadow = shadowVal ; 
	
	ambientComponent *= ambientStrength ; 
	diffuseComponent *= diffStrength ; 
	specularComponent *= specStrength ; 

	return (ambientComponent + ((1.0 - shadow) * (diffuseComponent + specularComponent)) ) ; 
}

vec3 CalcNonDirLight() 
{
	
	// Calculate Ambient Component
	vec3 ambientComponent = light.ambient * vec3(texture(material.diffuse , textCoord)) ; 

	// Get Normal from normal map if there is one
	vec3 norm = normalize(fragNormal) ; 
	
	// Calculate Diffuse Components
	vec3 lightDir = normalize(light.position - fragPos) ; 
	float angle = max(dot(norm , lightDir) , 0.0) ; 
	vec3 diffuseComponent = light.diffuse * angle * vec3(texture(material.diffuse , textCoord)) ; 


	// Calculate Specular Component
	vec3 viewDir = normalize(viewPos - fragPos) ; 
	vec3 halfWay = normalize(viewDir + lightDir) ; 
	float specular = pow( max(dot(norm , halfWay) , 0.0) , material.shininess) ; 
	vec3 specularComponent = light.specular * specular * vec3(texture(material.specular , textCoord))  ; 

	return ((ambientComponent * ambientStrength) + (diffuseComponent * diffStrength) + (specularComponent * specStrength)) ;
}
/*
float ShadowCalculation()
{
	vec3 projCoord = fragPosLightSpace.xyz / fragPosLightSpace.w ; 
	projCoord = projCoord * 0.5 + 0.5 ; 
	float closestDepth = texture(shadowMap , projCoord.xy).r ;
	float currentDepth = projCoord.z ;

	vec3 norm = normalize(fragNormal) ;
	
	vec3 lightDir = normalize(LightDirection) ; 
	
	// Solve Shadow Acne
	float bias = max(0.01 * (1.0 - dot(norm , lightDir)) , 0.005) ; 
	
	float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoord.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;

	// Solve Sampling from outside Depth Map Range
	if(projCoord.z > 1.0)
	{
		shadow = 0.0 ; 
	}

	return shadow ; 
}
*/

float ShadowCalculation(int index , vec4 fragPosLightSpace)
{
	vec3 projCoord = fragPosLightSpace.xyz / fragPosLightSpace.w ; 
	projCoord = projCoord * 0.5 + 0.5 ; 
	float closestDepth = texture(shadowMap[index] , projCoord.xy).r ;
	float currentDepth = projCoord.z ;
	vec3 norm = normalize(fragNormal) ; 
	vec3 lightDir = normalize(LightDirection) ; 
	
	// Solve Shadow Acne
	float bias = max(0.01 * (1.0 - dot(norm , lightDir)) , 0.005) ; 
	
	float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap[index], 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap[index], projCoord.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }

    shadow /= 9.0;

	// Solve Sampling from outside Depth Map Range
	if(projCoord.z > 1.0)
	{
		shadow = 0.0 ; 
	}

	return shadow ; 
}