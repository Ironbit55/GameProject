#version 150 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;

uniform vec4 nodeColour;
uniform int spriteBatched;

in  vec3 position;
in  vec2 texCoord;
in  vec4 colour;

out Vertex	{
	vec2 texCoord;
	vec4 colour;
} OUT;

void main(void)	{
	OUT.texCoord	= (textureMatrix * vec4(texCoord, 0.0, 1.0)).xy;
	
	if(spriteBatched > 0){
		gl_Position		= (projMatrix * viewMatrix) * vec4(position, 1.0);
		OUT.colour		=  colour;
	}else{
		gl_Position		= (projMatrix * viewMatrix * modelMatrix) * vec4(position, 1.0);
		OUT.colour		=  nodeColour;
	}
}