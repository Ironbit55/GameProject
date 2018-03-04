#pragma once
#include "../../nclgl/Vector3.h"
#include "../../nclgl/Vector4.h"
#include "../../nclgl/WinRenderer.h"

class SpriteRenderable
{
public:
	SpriteRenderable(Vector3 position, Vector3 scale, float rotationDeg = 0.0f) : position(position), scale(scale), colour(1, 1, 1, 1), glTexture(0){
		qRotation = Quaternion::AxisAngleToQuaterion(Vector3(0, 0, 1), rotationDeg);
	}
	~SpriteRenderable() {};

	Vector3 position;
	Vector3 scale;
	Quaternion qRotation;
	Vector4 colour;

	//maybe create a texture class to abstract this
	//probably still just a glTexture underneath...
	//but in case we do something more sophistacted with textures
	//eg hold in ram
	GLuint glTexture;

	static bool compareByTexture(SpriteRenderable& a, SpriteRenderable& b) { return a.glTexture < b.glTexture; }
};

