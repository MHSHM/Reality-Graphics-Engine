#version 330

layout (location = 0) in vec3 position;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	vec3 posInView = vec3(view * vec4(position, 0.0)) ; 
	gl_Position = projection * vec4(posInView , 1.0) ;
	TexCoords = position;
}