#version 330 core

layout(location=0) in vec3 position ;
layout(location=1) in vec2 UVCoord ;
layout(location=2) in vec3 normal ;
layout(location=3) in vec3 tangent ; 

uniform mat4 uWorldTransform ; 
uniform mat4 uView ; 
uniform mat4 uProjection ; 
uniform mat4 uLightMatrix[3] ; 

out vec4 fragPosLightSpace[3] ;
out float FragViewZ ; 
out vec2 textCoord ; 
out vec3 fragPos ; 
out vec3 fragNormal ; 
out mat3 TBN ; 

void main()
{
	gl_Position = uProjection * uView * uWorldTransform * vec4(position, 1.0) ; 
	textCoord = UVCoord ; 
	fragPos =  vec3(uWorldTransform * vec4(position , 1.0)) ; 
	fragNormal = vec3(uWorldTransform * vec4(normal , 0.0)) ; 
	for(int i = 0 ; i < 3 ; ++i)
	{
		fragPosLightSpace[i] = uLightMatrix[i] * vec4(fragPos , 1.0) ; 
	}
	vec4 view = uView * uWorldTransform * vec4(position, 1.0) ; 
	FragViewZ = view.z ;
	
	/*
	vec3 T = normalize(vec3(uWorldTransform * vec4(tangent , 0.0))) ; 
	vec3 N = normalize(vec3(uWorldTransform * vec4(normal , 0.0))) ; 
	vec3 B = cross(N , T) ; 
	TBN = mat3(T , B , N) ;
	*/

	vec3 T = normalize(vec3(uWorldTransform * vec4(tangent, 0.0)));
	vec3 N = normalize(vec3(uWorldTransform * vec4(normal, 0.0)));
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);
	TBN = mat3(T, B, N); 
}