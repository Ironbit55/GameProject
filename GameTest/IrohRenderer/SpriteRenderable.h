#pragma once
#include "../../nclgl/Vector3.h"
#include "../../nclgl/Vector4.h"
#include "../../nclgl/WinRenderer.h"


const float epsilon = 0.0001;



class SpriteRenderable
{
public:
	SpriteRenderable(Vector2 position = Vector2(), float depth = 200.0f, Vector3 scale = Vector3(10.0f, 10.0f, 10.0f), float rotationDeg = 0.0f) :
			position(position), depth(depth), scale(scale), colour(1, 1, 1, 1), glTexture(0), rotation(rotationDeg) {}
	SpriteRenderable(GLuint tex, float depth = -200.0f, Vector4 colour = Vector4(1, 1, 1, 1), float rotation = 0.0f) : position(), depth(depth), scale(), colour(colour),
		glTexture(tex), rotation(rotation) {};
	~SpriteRenderable() {};

	Vector2 position;
	float depth;
	float rotation;
	Vector4 colour;

	//maybe create a texture class to abstract this
	//probably still just a glTexture underneath...
	//but in case we do something more sophistacted with textures
	//eg hold in ram
	GLuint glTexture;
	Vector3 scale;

};

