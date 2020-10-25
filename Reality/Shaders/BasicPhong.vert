#version 330 core

layout(location=0) in vec3 position ;
layout(location=1) in vec2 UVCoord ;
layout(location=2) in vec3 normal ;

uniform mat4 uWorldTransform ; 
uniform mat4 uView ; 
uniform mat4 uProjection ; 

out vec2 textCoord ; 
out vec3 fragPos ; 
out vec3 fragNormal ; 

void main()
{
	gl_Position = uProjection * uView * uWorldTransform * vec4(position, 1.0) ; 
	textCoord = UVCoord ; 
	fragPos =  vec3(uWorldTransform * vec4(position , 1.0)) ; 
	fragNormal = vec3(uWorldTransform * vec4(normal , 0.0)) ; 
}