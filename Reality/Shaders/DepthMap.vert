#version 330 core

layout(location=0) in vec3 position ;
layout(location=1) in vec2 UVCoord ;
layout(location=2) in vec3 normal ;
 
uniform mat4 uWorldTransform ; 
uniform mat4 uLightTransformation ; 

void main()
{
	gl_Position = uLightTransformation * uWorldTransform * vec4(position, 1.0) ; 
}