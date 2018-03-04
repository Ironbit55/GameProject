/*
 * Class: SpriteRenderer
 * Implements: SdlRenderer
 * Author:Ed Curran (b5021040)
 * Description: Used to render sprites. Includes some sprite rendering optimisations
 * Take some ideas from the 2d Renderer used in simulations coursework
 * 
 */

#pragma once
#include "../sdlgl/SDLRenderer.h"
#include "../../nclgl/Camera.h"
#include "SpriteRenderable.h"
#include "../../nclgl/Frustum.h"

using namespace std;

class SpriteRenderer :
	public SDLRenderer
{
public:
	SpriteRenderer(SdlWindow &parent);
	~SpriteRenderer();

	bool Init() override;

	//sprite container?
	virtual void UpdateScene(float msec);
	virtual void RenderScene();

	Camera* getCamera() { return camera; }

	
protected:
	Camera*	camera;
	Frustum frameFrustum;
	Mesh* spriteMesh;
	Mesh* spriteBatchMesh;
	vector<SpriteRenderable> sprites;

	void	DrawSprites();
	void DrawSprite(SpriteRenderable& sprite);
	void DrawSpriteSpriteBatch(SpriteRenderable& sprite);
	void emptySpriteBatch(GLuint newTexture);
};

