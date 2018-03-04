#version 150 core

uniform sampler2D diffuseTex;
uniform int useTexture;

in Vertex	{
	vec2 texCoord;
	vec4 colour;
} IN;

out vec4 gl_FragColor;

void main(void)	{
	vec4 outColour;
	outColour = IN.colour;
	
	//gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
	
	if(useTexture > 0) {
		outColour	*= texture(diffuseTex, IN.texCoord);
	}
	if (outColour.w < 0.1)
      discard; 
	  
	gl_FragColor = outColour;
}