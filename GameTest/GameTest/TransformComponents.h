#pragma once
#include "IrohRenderer/SpriteRenderable.h"

struct SimpleTransform {
	SimpleTransform(float x = 0.0f, float y = 0.0f, float rotation = 0.0f) : position(x, y), rotation(rotation) {};
	Vector2 position;
	float rotation;
};

struct RenderComponent {
	RenderComponent(SimpleTransform* transform, SpriteRenderable& sprite) : transform(transform), sprite(sprite) {};
	SimpleTransform* transform;
	SpriteRenderable sprite;
};
