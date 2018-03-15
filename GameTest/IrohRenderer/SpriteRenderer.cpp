#include "SpriteRenderer.h"
#include <algorithm>
#include "MeshSpriteBatch.h"


SpriteRenderer::SpriteRenderer(SdlWindow &parent) : SDLRenderer(parent) {


}

bool SpriteRenderer::Init() {
	SDLRenderer::Init();

	projMatrix = Matrix4::Orthographic(-1.0f, 10000.0f, width / 2.0f, -width / 2.0f, height / 2.0f, -height / 2.0f);
	camera = new Camera(0.0f, 0.0f, Vector3(0, 0, 750.0f));

	//do this somewhere else
	currentShader = new Shader(SHADERDIR"SceneVertex.glsl", SHADERDIR"SceneFragment.glsl");
	//currentShader = new Shader(SHADERDIR"SceneVertex_spritebatch.glsl", SHADERDIR"SceneFragment.glsl");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	spriteMesh = Mesh::GenerateQuad();
	spriteBatchMesh = new MeshSpriteBatch(256);

	viewMatrix = camera->BuildViewMatrix();
	frameFrustum.FromMatrix(projMatrix*viewMatrix);

	GLuint dragonTexture = SOIL_load_OGL_texture(TEXTUREDIR"dragon.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	GLuint raiderTexture = SOIL_load_OGL_texture(TEXTUREDIR"raider.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);


	//create a bunch of sprite in contigous memory
	const int numSprites = 100;
	SpriteRenderable* spritesTemp = new SpriteRenderable[numSprites];

	for (int i = 0; i < numSprites; ++i){
		spritesTemp[i] = SpriteRenderable(Vector3((-960.0f + (i % 180) * 8), (600.0f - ((i / 180)) * 8), -200.0f), Vector3(5.0f, 5.0f, 5.0f), 0.0f);
		//spritesTemp[i] = SpriteRenderable(Vector3((-960.0f + (i % 180) * 120), (600.0f - ((i / 180)) * 120), -200.0f), Vector3(30.0f, 30.0f, 5.0f));
		if (i % 2 == 0) {
			spritesTemp[i].glTexture = dragonTexture;
			spritesTemp[i].colour = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
		}else{
			spritesTemp[i].glTexture = raiderTexture;
			spritesTemp[i].colour = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
		}


		sprites.push_back(&spritesTemp[i]);
	}

	SpriteRenderable* sprite1 = new SpriteRenderable(Vector3(0, 0.0f, -200.0f), Vector3(50.0f, 50.0f, 50.0f), 20.0f);
	sprite1->colour = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
	sprite1->glTexture = SOIL_load_OGL_texture(TEXTUREDIR"dragon.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	sprites.push_back(sprite1);

	SpriteRenderable* sprite2 = new SpriteRenderable(Vector3(-80.0f, 0.0f, -200.0f), Vector3(20.0f, 20.0f, 50.0f));
	sprite2->colour = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
	sprite2->glTexture = SOIL_load_OGL_texture(TEXTUREDIR"raider.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	sprites.push_back(sprite2);
	
	previousTexture = -1;
	return true;
}


SpriteRenderer::~SpriteRenderer(){
}

void SpriteRenderer::RenderScene(){
	BuildSpriteLists();

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glUseProgram(currentShader->GetProgram());
	UpdateShaderMatrices();

	DrawSprites();

	glUseProgram(0);
	SwapBuffers();

	clearSpriteLists();
}


void SpriteRenderer::clearSpriteLists(){
	transparentSpriteList.clear();
	opaqueSpriteList.clear();
}

void SpriteRenderer::DrawSprites(){
	
	//spriteBatchMesh->Begin(0);
	previousTexture = 0;
	int batchSize = -1;
	for (vector<SpriteRenderable*>::iterator i = opaqueSpriteList.begin(); i != opaqueSpriteList.end(); ++i) {
		
		DrawSprite(*i);
		//if  batch size greater than or equal to min match size than use spritebatch
		//otherwise just draw em.

		updateBatchSize(i, opaqueSpriteList.end(), batchSize);
		
		if(batchSize >= MIN_BATCH_SIZE){
			DrawSpriteSpriteBatch(*i);
		}else{
			DrawSprite(*i);
		}

	}

	emptySpriteBatch();
	//theres basiicaly no way transparent sprites are gunno be ordered enough to do batching
	for (vector<SpriteRenderable*>::iterator i = transparentSpriteList.begin(); i != transparentSpriteList.end(); ++i) {
		DrawSprite(*i);
	}

	//emptySpriteBatch();
}

void SpriteRenderer::DrawSprite(SpriteRenderable* sprite){

	
	Matrix4 modelMatrix = Matrix4::Translation(sprite->position) * sprite->qRotation.ToMatrix() * Matrix4::Scale(sprite->scale);
	spriteMesh->SetTexture(sprite->glTexture);

	
	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, (float*)& modelMatrix);
	glUniform4fv(glGetUniformLocation(currentShader->GetProgram(), "nodeColour"), 1, (float*)& sprite->colour);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "useTexture"), (int)sprite->glTexture);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "spriteBatched"), (int)0);

	GL_BREAKPOINT

	//binding the same texture multiple times has no effect so we don't really need to bother doing anything special as long as
	//sprites are correctly sorted by texture... unless we wan't to get into batching
	spriteMesh->Draw();
}

//using batches is slower than individual draw calls if our batches are small
//so ideally we check how many sprite of a texture we have, if lots batch them.
//if few draw them individually
//takes like 4-6 sprites per batch to be worth it.
void SpriteRenderer::DrawSpriteSpriteBatch(SpriteRenderable* sprite){

	Matrix4 modelMatrix = Matrix4::Translation(sprite->position) * sprite->qRotation.ToMatrix() * Matrix4::Scale(sprite->scale);

	
	if (!spriteBatchMesh->AddSprite(modelMatrix, sprite->colour, sprite->glTexture)) {
		emptySpriteBatch();
		spriteBatchMesh->Begin(sprite->glTexture);
		spriteBatchMesh->AddSprite(modelMatrix, sprite->colour, sprite->glTexture);
	} 
}

void SpriteRenderer::emptySpriteBatch(){
	if(spriteBatchMesh->IsEmpty()){
		return;
	}
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "spriteBatched"), (int)1);
	//Matrix4 identityMatrix;
	//glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, (float*)& identityMatrix);
	//glUniform4fv(glGetUniformLocation(currentShader->GetProgram(), "nodeColour"), 1, (float*)& Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "useTexture"), (int)spriteBatchMesh->GetTexture());

	spriteBatchMesh->Draw();
}

bool SpriteRenderer::updateBatchSize(vector<SpriteRenderable*>::iterator startSprite,
	vector<SpriteRenderable*>::iterator iteratorEnd, int& batchSizeOut){
	
	if ((*startSprite)->glTexture == previousTexture) {
		return false;
	}

	//we have started a new block of sprites 
	batchSizeOut = 1;

	//iterate MIN_MATCH_SIZE sprites ahead. if the end isn't before then
	if (iteratorEnd - startSprite > MIN_BATCH_SIZE) {
		vector<SpriteRenderable*>::iterator i = startSprite + 1;
		while (i < (startSprite + MIN_BATCH_SIZE)) {
			
			//once we find a sprite with a different texture we know our batch size
			if ((*startSprite)->glTexture != (*startSprite)->glTexture) {
				break;
			}

			++i;
			batchSizeOut++;
		}
	}


	return true;
}

void SpriteRenderer::BuildSpriteLists(){
	for (vector<SpriteRenderable*>::iterator spriteI = sprites.begin(); spriteI != sprites.end(); ++spriteI) {

		if (frameFrustum.InsideFrustum((*spriteI)->position, 48.0f)) {
			if((*spriteI)->colour.w < 0.98f){
				transparentSpriteList.push_back(*spriteI);
			}else{
				opaqueSpriteList.push_back(*spriteI);
			}
		}
	}

	std::sort(opaqueSpriteList.begin(), opaqueSpriteList.end(), compareByDepthAndTexture);
	std::sort(transparentSpriteList.begin(), transparentSpriteList.end(), compareByDepthAndTextureReverse);
}

void SpriteRenderer::UpdateScene(float mse){
	viewMatrix = camera->BuildViewMatrix();
	frameFrustum.FromMatrix(projMatrix*viewMatrix);
}
