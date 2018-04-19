#pragma once
#include "IrohRenderer/SpriteRenderable.h"

struct SimpleTransform {
	SimpleTransform(float x = 0.0f, float y = 0.0f, float width = 0.0f, float height = 0.0f, float rotation = 0.0f) : position(x, y), scale(width, height), rotation(rotation) {};

	Vector2 getOrigin() const{
		return position;
	}

	Vector2 position;
	float rotation;
	Vector2 scale;
};

struct RenderComponent {
	RenderComponent(SimpleTransform* transform, SpriteRenderable& sprite) : transform(transform), sprite(sprite) {};
	SimpleTransform* transform;
	SpriteRenderable sprite;
};
