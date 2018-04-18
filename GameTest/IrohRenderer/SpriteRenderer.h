/*
 * Class: SpriteRenderer
 * Implements: SdlRenderer
 * Author:Ed Curran (b5021040)
 * Description: Used to render sprites. Includes some sprite rendering optimisations
 * Takes some ideas from the 2d Renderer used in simulations coursework
 * 
 */

#pragma once
#include "../sdlgl/SDLRenderer.h"
#include "../../nclgl/Camera.h"
#include "SpriteRenderable.h"
#include "../../nclgl/Frustum.h"
#include "MeshSpriteBatch.h"
#include <array>

using namespace std;

const int MIN_BATCH_SIZE = 6;
const int MAX_NUM_SPRITES = 40000;
const int MAX_NUM_TRANSPARENT_SPRITES = 2000;
const int MAX_NUM_OPAQUE_SPRITES = 18000;
const float MIN_OPACITY = 0.93;

class SpriteRenderer :
	public SDLRenderer
{
public:
	SpriteRenderer(SdlWindow &parent);
	~SpriteRenderer();

	SpriteRenderer(const SpriteRenderer &r);

	bool Init() override;

	//sprite container?
	virtual void UpdateScene(float msec);
	virtual void RenderScene();

	Camera* getCamera() { return camera; }

	static bool compareByTexture(SpriteRenderable* a, SpriteRenderable* b) { return a->glTexture < b->glTexture; }

	static bool compareByDepthAndTexture(SpriteRenderable a, SpriteRenderable b) {
		if (fabs(a.depth - b.depth) < epsilon) {
			//depth is equal
			return a.glTexture < b.glTexture;
		} else {
			return a.depth < b.depth;
		}
	}

	static bool compareByDepthAndTexturePtr(SpriteRenderable* a, SpriteRenderable* b) {
		if (fabs(a->depth - b->depth) < epsilon) {
			//depth is equal
			return a->glTexture < b->glTexture;
		} else {
			return a->depth < b->depth;
		}
	}

	static bool compareByDepthAndTextureReverse(SpriteRenderable* a, SpriteRenderable* b) {
		if (fabs(a->depth - b->depth) < epsilon) {
			//depth is equal
			return a->glTexture > b->glTexture;
		} else {
			return a->depth > b->depth;
		}
	}

	void clearSprites() {
		spriteListCount = 0;
	}

	bool addSpriteToDrawList(SpriteRenderable* sprite) {
		if (frameFrustum.InsideFrustum2d(sprite->position, 48.0f)) {
			addSprite(sprite);
			return true;
		}
		return false;
	}

	void addSprite(SpriteRenderable* sprite) {
		(*spriteList)[spriteListCount] = sprite;
		spriteListCount++;
	}
	
protected:
	Camera*	camera;
	Frustum frameFrustum;
	
	Mesh* spriteMesh;
	MeshSpriteBatch* spriteBatchMesh;
	
	std::array <SpriteRenderable*, MAX_NUM_SPRITES>* spriteList;
	int spriteListCount;
	
	GLuint previousTexture;

	//vector<SpriteRenderable*> sprites;

	//vector<SpriteRenderable*> transparentSpriteList;
	//vector<SpriteRenderable*> opaqueSpriteList;


	//SpriteRenderable* spriteListAll;
	//int spriteListAllCount;
	

	/*array<SpriteRenderable, MAX_NUM_TRANSPARENT_SPRITES> transparentSpriteList;
	array<SpriteRenderable, MAX_NUM_OPAQUE_SPRITES> opaqueSpriteList;*/


	std::array <SpriteRenderable*, MAX_NUM_SPRITES>::iterator spriteTail(){
		return spriteList->begin() + spriteListCount;
	}

	void DrawSprites();
	void DrawSprite(SpriteRenderable* sprite);
	void DrawSpriteSpriteBatch(SpriteRenderable* sprite);
	void emptySpriteBatch();


	/*
	 * identify blocks of sprites sharing the same texture
	and count how large the block is
	 */
	bool updateBatchSize(array<SpriteRenderable*, MAX_NUM_SPRITES>::iterator startSprite, array<SpriteRenderable*, MAX_NUM_SPRITES>::iterator iteratorEnd, int& batchSizeOut);
	bool updateBatchSizeTransparent(array<SpriteRenderable*, MAX_NUM_SPRITES>::iterator startSprite,
		array<SpriteRenderable*, MAX_NUM_SPRITES>::iterator iteratorEnd, int& batchSizeOut);

	void	BuildSpriteLists();


};

