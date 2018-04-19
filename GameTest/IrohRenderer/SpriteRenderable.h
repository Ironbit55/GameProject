#pragma once
#include "../../nclgl/Vector3.h"
#include "../../nclgl/Vector4.h"
#include "../../nclgl/WinRenderer.h"


const float epsilon = 0.0001;



class SpriteRenderable
{
public:
	SpriteRenderable(Vector2 position = Vector2(), float depth = 200.0f, Vector3 scale = Vector3(10.0f, 10.0f, 10.0f), float rotationDeg = 0.0f) :
			position(position), depth(depth), scale(scale), colour(1, 1, 1, 1), textureId(-1), rotation(rotationDeg) {}
	SpriteRenderable(int tex, float depth = -200.0f, Vector4 colour = Vector4(1, 1, 1, 1), float rotation = 0.0f) : position(), depth(depth), scale(), colour(colour),
		textureId(tex), rotation(rotation) {};
	~SpriteRenderable() {};

	
	float depth;
	Vector4 colour;

	//maybe create a texture class to abstract this
	//probably still just a glTexture underneath...
	//but in case we do something more sophistacted with textures
	//eg hold in ram
	int textureId;
	
	//only used by renderer, maybe i can make these private and make
	//renderer a friend class?
	Vector2 position;
	float rotation;
	Vector3 scale;

	GLuint glTexture;

};

