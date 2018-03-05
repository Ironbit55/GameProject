/*
 * Class: MeshSpriteBatch
 * Implements: Mesh
 * Author: Ed Curran (b5021040)
 * Description: A hacky batch implemented using the mesh class
 * Represents a specific batch instance. Each instance holds sprites
 * sharing the same texture
 * 
 * We use indices to draw our gl_triangles as we can't use triangle strips
 * so this saves data we still waste a lot of data on duplicate 
 * normals, colours, tangents etc
 * 
 * I hacked this together using an instance of a mesh as a batch instance
 * but i didn't see much performance benefit (it was actually worse on my machine)
 * 
 * over sorting sprites by texture
 * and reusing the same quad buffer for each draw...
 * (pretty sure thats because im cpu bound. I hope. The batching requires more cpu as we
 * multiply each quad vertex by the sprite model matrix on cpu before sending to gpu)
 * 
 * Then I realised theres basically no way I'm going to be gpu bound at any point
 * with this game so I don't know why I bothered really.
 */

#pragma once
#include "../../nclgl/Mesh.h"

class MeshSpriteBatch : public Mesh
{
public:
	MeshSpriteBatch(int maxSprites);
	~MeshSpriteBatch(){}

	/*
	 * submit a sprite to batch to submit for drawing when batch
	 * is emptied, model matrix and colour are applied to each vertex
	 * in sprite
	 */
	bool AddSprite(Matrix4 modelMatrix, Vector4 colour, GLuint spriteTexture);

	/*
	 * submits sprites to be drawn 
	 * and empties the batch
	 */
	void Draw() override;

	/*
	 * Start new batch with given texture
	 * resets filled sprites to 0 
	 */
	void Begin(GLuint glTexture);

	void flushBuffers();
	void mapBuffers();
	void ImmutableBufferData();

	bool IsEmpty() { return numSpritesFilled == 0; }
protected:
	int numSpritesFilled;
	int numSpritesMax;

	Vector3* vertexBufferPtr;
	Vector4* colourBufferPtr;
	Vector2* textureCoordBufferptr;
	Vector3* normalBufferPtr;
	Vector3* tangentBufferPtr;
	unsigned int* indexBufferPtr;
};

