#version 330 core

in vec2 TexCoords;

out vec4 color;

uniform bool isSharpen ; 
uniform bool isBlur ; 
uniform bool isEdgeDetect ; 
uniform bool isGammaCorrection ; 
uniform sampler2D screenTexture;

const float offset = 1.0 / 300.0 ; 
const float gamma = 2.2f ; 

vec3 CalcSharpEffect(vec2 offsets[9] , vec3 sharpEffect) ; 
vec3 CalcBlurEffect(vec2 offsets[9] , vec3 blurEffect) ; 
vec3 CalcEdgeEffect(vec2 offsets[9] , vec3 edgeEffect) ; 

void main()
{
	vec2 offsets[9] = vec2[](
	
		vec2(-offset, offset), // top-left
		vec2(0.0f, offset), // top-center
		vec2(offset, offset), // top-right
		vec2(-offset, 0.0f), // center-left
		vec2(0.0f, 0.0f), // center-center
		vec2(offset, 0.0f), // center-right
		vec2(-offset, -offset), // bottom-left
		vec2(0.0f, -offset), // bottom-center
		vec2(offset, -offset) // bottom-right
	);
	
	vec3 black = vec3(0.0 , 0.0 , 0.0) ; 
	vec3 sharpEffect = vec3(0.0 , 0.0 , 0.0);
	vec3 blurEffect = vec3(0.0 , 0.0 , 0.0) ; 
	vec3 edgeEffect = vec3(0.0 , 0.0 , 0.0) ; 
	vec3 finalColor = vec3(0.0 , 0.0 , 0.0) ; 
	
	
	if(isSharpen)
	{
		finalColor += CalcSharpEffect(offsets , sharpEffect) ; 
	}

	if(isBlur)
	{
		finalColor += CalcBlurEffect(offsets , blurEffect) ; 
	}

	if(isEdgeDetect)
	{
		finalColor += CalcEdgeEffect(offsets , edgeEffect) ; 
	}
	
	if(isGammaCorrection)
	{
		finalColor *= 1.0 / gamma ; 
	}

	if(finalColor == black)
	{
		color = texture(screenTexture , TexCoords) ; 
	}
	else
	{
		color = vec4(finalColor , 1.0) ; 
	}
	

	/*
	float depth = texture(screenTexture , TexCoords).r ;
	color = vec4( vec3(depth) , 1.0) ; 
	*/
}

vec3 CalcSharpEffect(vec2 offsets[9] , vec3 sharpEffect) 
{
	float sharpen[9] = float[](
		-1, -1, -1,
		-1, 9, -1,
		-1, -1, -1
	);

	vec3 sampleTex[9] ; 

	for(int i = 0 ; i < 9 ; ++i)
	{
		sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
	}

	for(int i = 0 ; i < 9 ; ++i)
	{
		sharpEffect += sampleTex[i] * sharpen[i] ; 
	}	

	return sharpEffect ; 
}

vec3 CalcBlurEffect(vec2 offsets[9] , vec3 blurEffect) 
{
	float blur[9] = float[](
		1.0 / 16, 2.0 / 16, 1.0 / 16,
		2.0 / 16, 4.0 / 16, 2.0 / 16,
		1.0 / 16, 2.0 / 16, 1.0 / 16
	);

	vec3 sampleTex[9] ; 

	for(int i = 0 ; i < 9 ; ++i)
	{
		sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
	}

	for(int i = 0 ; i < 9 ; ++i)
	{
		blurEffect += sampleTex[i] * blur[i] ; 
	}

	return blurEffect ; 
}

vec3 CalcEdgeEffect(vec2 offsets[9] , vec3 edgeEffect) 
{
	float edgeDetection[9] = float[](
		1, 1, 1,
		1, -8, 1,
		1, 1, 1
	);

	vec3 sampleTex[9] ; 

	for(int i = 0 ; i < 9 ; ++i)
	{
		sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
	}

	for(int i = 0 ; i < 9 ; ++i)
	{
		edgeEffect += sampleTex[i] * edgeDetection[i] ; 
	}

	return edgeEffect ; 
}