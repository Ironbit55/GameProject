#pragma once
#include "IrohRenderer/SpriteRenderable.h"

struct SimpleTransform {
	Vector2 position;
	float rotation;
};

struct RenderComponent {
	SpriteRenderable sprite;
	SimpleTransform* transform;
};
