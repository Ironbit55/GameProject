/*
* Class: ContentManager
* 
* Author: Ed Curran (b5021040)
* Description: A simple content manager, its simple because I'm going to try and get away with
* just loading all textures into opengl 
*
* If we want to make this future proof we could encapsulate texture with a texture class,
* could just hold gl texture id for now.
* then content manager holds texture and can pass out handles to requested texture
* this way if texture contained actual texture data it could be loaded in and out of
* gpu memory.
* 
* Todo: alter to use memory pool, we aren't storing real textures so there isn't much point
*/

#pragma once

#include "GL/glew.h"
#include "SOIL.h"
#include "../../nclgl/common.h"
#include <vector>
#include <map>


using namespace std;

struct Texture {
	GLuint glHandle;
};
class ContentManager
{
public:
	ContentManager(int numTextures, string directory = "");
	~ContentManager();
	
	/*
	* load texture into opengl, store a handle to the texture in the content manager
	* returns a int textureHandle which can which can be used to get the texture in the future
	*/
	int loadTexture(string filePath);
	bool getTexture(int textureHandle, Texture& outTexture);

	void free();

protected:
	//use soil to load texture into opengl and get an instance of texture with
	//the gl handle set
	Texture soilLoadTexture(string filePath);
	//should alocate this with a memory manager
	Texture* textures;
	//maps to the index location of the texture in the textures array
	//std::map<string, int> nameToTextureHandleMap;
	string textureDir;

	//stores how full the texture array is
	//used to assign the texture handle on loadTexture
	//deleting textures will leave a hole in the array
	//which we won't be able to use so don't even 
	//bother allowing to delete texture.
	//this could all be fixed of course by
	//using an actual pool allocator
	int textureIndex;
};

