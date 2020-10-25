#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 2) out;

in vec3 normal[] ; 

const float offset = 0.8f ;

void GetNormal(int index)
{
	gl_Position = gl_in[index].gl_Position ; 
	EmitVertex();

	gl_Position = gl_in[index].gl_Position + vec4(normal[index] , 0.0) * offset  ;
	EmitVertex();
	EndPrimitive();
}

void main()
{
	GetNormal(0) ; 
	GetNormal(1) ; 
	GetNormal(3) ; 
}