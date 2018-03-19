#include "ContentManager.h"



ContentManager::ContentManager(int numTextures, string directory){
	textures = new Texture[numTextures];
	textureDir = directory;
	textureIndex = 0;
}

ContentManager::~ContentManager()
{
}

int ContentManager::loadTexture(string filePath){
	textures[textureIndex] = soilLoadTexture(filePath);
	textureIndex++;
	return textureIndex;
}

bool ContentManager::getTexture(int textureHandle, Texture& outTexture){
	if (textureHandle < 0 || textureHandle > textureIndex) {
		return false;
	}
	

	outTexture = textures[textureHandle];
	return true;
}

void ContentManager::free(){
	delete textures;
	textureIndex = 0;
}

Texture ContentManager::soilLoadTexture(string filePath){
	Texture texture;
	string file = textureDir + filePath;
	texture.glHandle = SOIL_load_OGL_texture(file.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	return texture;
}
