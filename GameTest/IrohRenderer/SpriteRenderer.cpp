#include "SpriteRenderer.h"
#include <algorithm>
#include "MeshSpriteBatch.h"


SpriteRenderer::SpriteRenderer(SdlWindow &parent) : SDLRenderer(parent), spriteListCount(0), previousTexture(-1){
	spriteList = new array<SpriteRenderable*, MAX_NUM_SPRITES>;

	
	//spriteListAll = new SpriteRenderable[5];

}

bool SpriteRenderer::Init() {
	SDLRenderer::Init();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//do this somewhere else
	currentShader = new Shader(SHADERDIR"SceneVertex.glsl", SHADERDIR"SceneFragment.glsl");
	
	if (GetCurrentShader()->UsingDefaultShader()) {
		cout << "Warning: Using default shader! Your shader probably hasn't worked..." << endl;
		cout << "Press any key to continue." << endl;
		std::cin.get();
	}

	spriteMesh = Mesh::GenerateQuad();
	spriteBatchMesh = new MeshSpriteBatch(256);
	camera = new Camera(0.0f, 0.0f, Vector3(0, 0, 750.0f));

	projMatrix = Matrix4::Orthographic(-1.0f, 10000.0f, width / 2.0f, -width / 2.0f, height / 2.0f, -height / 2.0f);
	viewMatrix = camera->BuildViewMatrix();
	frameFrustum.FromMatrix(projMatrix*viewMatrix);

	/*spriteMesh = Mesh::GenerateQuad();
	spriteBatchMesh = new MeshSpriteBatch(256);*/



	///this is just for testing it doesn't belong here promise

	//GLuint dragonTexture = SOIL_load_OGL_texture(TEXTUREDIR"dragon.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	//GLuint raiderTexture = SOIL_load_OGL_texture(TEXTUREDIR"raider.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);


	//create a bunch of sprite in contigous memory
	//const int numSprites = 35000;
	//int i;
	//for (i = 0; i < numSprites; ++i){
	//	spriteListAll[i] = SpriteRenderable(Vector2((-960.0f + (i % 180) * 8), (600.0f - ((i / 180)) * 8)), -200.0f, Vector3(5.0f, 5.0f, 5.0f), 0.0f);
	//	
	//	//spritesTemp[i] = SpriteRenderable(Vector3((-960.0f + (i % 180) * 120), (600.0f - ((i / 180)) * 120), -200.0f), Vector3(30.0f, 30.0f, 5.0f));
	//	if (i % 2 == 0) {
	//		spriteListAll[i].glTexture = dragonTexture;
	//		spriteListAll[i].colour = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
	//	}else{
	//		spriteListAll[i].glTexture = raiderTexture;
	//		spriteListAll[i].colour = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
	//	}

	//	//spriteListTail++;
	//	spriteListAllCount++;
	//	//sprites.push_back(&spritesTemp[i]);
	//}

	//SpriteRenderable sprite1 = SpriteRenderable(Vector2(0.0f, 0.0f), -200.0f, Vector3(50.0f, 50.0f, 50.0f), 20.0f);
	//sprite1.colour = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
	//sprite1.glTexture = SOIL_load_OGL_texture(TEXTUREDIR"dragon.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	//
	//spriteListAll[i] = sprite1;
	//i++;
	//spriteListAllCount++;
	////addSprite(&spriteListAll[i]);
	////addSprite(sprite1);
	////sprites.push_back(sprite1);

	//SpriteRenderable sprite2 = SpriteRenderable(Vector2(-80.0f, 0.0f), -200.0f, Vector3(20.0f, 20.0f, 50.0f));
	//sprite2.colour = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
	//sprite2.glTexture = SOIL_load_OGL_texture(TEXTUREDIR"raider.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	//
	//spriteListAll[i] = sprite2;
	//spriteListAllCount++;
	//i++;
	
	//addSprite(&sprite2);
	//sprites.push_back(sprite2);
	
	previousTexture = -1;
	return true;
}


SpriteRenderer::~SpriteRenderer(){
	//delete[] spriteListAll;
	delete spriteList;
}

void SpriteRenderer::RenderScene(){
	BuildSpriteLists();

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glUseProgram(currentShader->GetProgram());
	UpdateShaderMatrices();

	DrawSprites();

	//maybe try and use same shader over frames where
	//possible
	glUseProgram(0);
	SwapBuffers();
}




void SpriteRenderer::DrawSprites(){
	
	previousTexture = -1;
	spriteBatchMesh->Begin(0);
	
	int batchSize = -1;

	std::array<SpriteRenderable*, MAX_NUM_SPRITES>::iterator transparentListTail = spriteTail();
	//draw all opaque then all transparent
	//loop front to back
	for (auto it = spriteList->begin(); it != spriteTail(); ++it) {
		//rare case
		if((*it)->colour.w < 0.93){
			transparentListTail = it;
			break;
		}

		
		//if  batch size greater than or equal to min match size than use spritebatch
		//otherwise just draw em.

		updateBatchSize(it, spriteTail(), batchSize);

		if (batchSize >= MIN_BATCH_SIZE) {
			DrawSpriteSpriteBatch((*it));
		} else {
			DrawSprite((*it));
		}
	}

	emptySpriteBatch();
	//loop back to front
	auto it = spriteTail();
	while(it != transparentListTail){
		--it;
		DrawSprite((*it));
	}

	emptySpriteBatch();

}

void SpriteRenderer::DrawSprite(SpriteRenderable* sprite){

	
	Matrix4 modelMatrix = Matrix4::Translation(Vector3(sprite->position.x, sprite->position.y, sprite->depth)) * Matrix4::Rotation(sprite->rotation, Vector3(0, 0, 1)) * Matrix4::Scale(sprite->scale);
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

	Matrix4 modelMatrix = Matrix4::Translation(Vector3(sprite->position.x, sprite->position.y, sprite->depth)) * Matrix4::Rotation(sprite->rotation, Vector3(0, 0, 1)) * Matrix4::Scale(sprite->scale);

	
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
	
	//Matrix4 identityMatrix;
	//glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, (float*)& identityMatrix);
	//glUniform4fv(glGetUniformLocation(currentShader->GetProgram(), "nodeColour"), 1, (float*)& Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	//tell shader this sprite is in a batch
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "spriteBatched"), (int)1);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "useTexture"), (int)spriteBatchMesh->GetTexture());

	spriteBatchMesh->Draw();
}

bool SpriteRenderer::updateBatchSize(array<SpriteRenderable*, MAX_NUM_SPRITES>::iterator startSprite,
	array<SpriteRenderable*, MAX_NUM_SPRITES>::iterator iteratorEnd, int& batchSizeOut){
	
	if ((*startSprite)->glTexture == previousTexture) {
		return false;
	}

	//we have started a new block of sprites 
	batchSizeOut = 1;

	//iterate MIN_MATCH_SIZE sprites ahead. if the end isn't before then
	if (iteratorEnd - startSprite > MIN_BATCH_SIZE) {
		auto i = startSprite + 1;
		while ((i < (startSprite + MIN_BATCH_SIZE)) && (*i)->colour.w > MIN_OPACITY) {
			
			//once we find a sprite with a different texture we know our batch size
			if ((*i)->glTexture != (*startSprite)->glTexture) {
				break;
			}

			++i;
			batchSizeOut++;
		}
	}


	return true;
}

bool SpriteRenderer::updateBatchSizeTransparent(array<SpriteRenderable*, MAX_NUM_SPRITES>::iterator startSprite,
	array<SpriteRenderable*, MAX_NUM_SPRITES>::iterator iteratorEnd, int& batchSizeOut) {

	if ((*startSprite)->glTexture == previousTexture) {
		return false;
	}

	//we have started a new block of sprites 
	batchSizeOut = 1;

	//iterate MIN_MATCH_SIZE sprites ahead. if the end isn't before then
	if (startSprite - iteratorEnd > MIN_BATCH_SIZE) {
		auto i = startSprite - 1;
		while (i > (startSprite - MIN_BATCH_SIZE)) {

			//once we find a sprite with a different texture we know our batch size
			if (((*i)->glTexture != (*startSprite)->glTexture)) {
				break;
			}

			--i;
			batchSizeOut++;
		}
	}
}

void SpriteRenderer::BuildSpriteLists(){
	std::sort(spriteList->begin(), spriteTail(), compareByDepthAndTexturePtr);
}

void SpriteRenderer::UpdateScene(float mse){
	viewMatrix = camera->BuildViewMatrix();
	frameFrustum.FromMatrix(projMatrix*viewMatrix);
}
