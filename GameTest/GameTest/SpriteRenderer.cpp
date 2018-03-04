#include "SpriteRenderer.h"
#include <algorithm>


SpriteRenderer::SpriteRenderer(SdlWindow &parent) : SDLRenderer(parent) {


}

bool SpriteRenderer::Init() {
	SDLRenderer::Init();

	projMatrix = Matrix4::Orthographic(-1.0f, 10000.0f, width / 2.0f, -width / 2.0f, height / 2.0f, -height / 2.0f);
	camera = new Camera(0.0f, 0.0f, Vector3(0, 0, 750.0f));

	//do this somewhere else
	currentShader = new Shader(SHADERDIR"SceneVertex.glsl", SHADERDIR"SceneFragment.glsl");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	spriteMesh = Mesh::GenerateQuad();
	spriteBatchMesh = Mesh::GenerateSpriteBatch(512);

	viewMatrix = camera->BuildViewMatrix();
	frameFrustum.FromMatrix(projMatrix*viewMatrix);

	GLuint dragonTexture = SOIL_load_OGL_texture(TEXTUREDIR"dragon.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	GLuint raiderTexture = SOIL_load_OGL_texture(TEXTUREDIR"raider.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);

	for (int i = 0; i < 400; ++i){
		SpriteRenderable sprite(Vector3((-960.0f + (i % 24) * 80), (600.0f - ((i / 24) % 14) * 80), -200.0f), Vector3(50.0f, 50.0f, 50.0f), 100.0f);
		sprite.colour = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
		if (i % 2 == 0) {
			sprite.glTexture = dragonTexture;
		}else{
			sprite.glTexture = raiderTexture;
		}
		sprites.push_back(sprite);
	}

	SpriteRenderable sprite1(Vector3(0, 0.0f, -200.0f), Vector3(50.0f, 50.0f, 50.0f), 60.0f);
	sprite1.colour = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
	sprite1.glTexture = SOIL_load_OGL_texture(TEXTUREDIR"dragon.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	sprites.push_back(sprite1);

	SpriteRenderable sprite2(Vector3(-80.0f, 0.0f, -200.0f), Vector3(20.0f, 20.0f, 50.0f));
	sprite2.colour = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
	sprite2.glTexture = SOIL_load_OGL_texture(TEXTUREDIR"raider.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0);
	sprites.push_back(sprite2);
	

	return true;
}


SpriteRenderer::~SpriteRenderer(){
}

void SpriteRenderer::RenderScene(){
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glUseProgram(currentShader->GetProgram());
	UpdateShaderMatrices();

	DrawSprites();

	glUseProgram(0);
	SwapBuffers();


}


void SpriteRenderer::DrawSprites(){
	std::sort(sprites.begin(), sprites.end(), SpriteRenderable::compareByTexture);
	for (vector<SpriteRenderable>::iterator i = sprites.begin(); i != sprites.end(); ++i) {
		DrawSprite(*i);
	}
	//emptySpriteBatch(0);
}

void SpriteRenderer::DrawSprite(SpriteRenderable& sprite){

	if (!frameFrustum.InsideFrustum(sprite.position, 65.0f)) {
		return;
	}
	
	Matrix4 modelMatrix = Matrix4::Translation(sprite.position) * sprite.qRotation.ToMatrix() * Matrix4::Scale(sprite.scale);
	spriteMesh->SetTexture(sprite.glTexture);

	
	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, (float*)& modelMatrix);
	glUniform4fv(glGetUniformLocation(currentShader->GetProgram(), "nodeColour"), 1, (float*)& sprite.colour);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "useTexture"), (int)sprite.glTexture);

	GL_BREAKPOINT

	//binding the same texture multiple times has no effect so we don't really need to bother doing anything special as long as
	//sprites are correctly sorted by texture... unless we wan't to get into batching
	spriteMesh->Draw();
}

void SpriteRenderer::DrawSpriteSpriteBatch(SpriteRenderable& sprite){
	if (!frameFrustum.InsideFrustum(sprite.position, 65.0f)) {
		return;
	}

	Matrix4 modelMatrix = Matrix4::Translation(sprite.position) * sprite.qRotation.ToMatrix() * Matrix4::Scale(sprite.scale);

	if (!spriteBatchMesh->addSprite(modelMatrix, sprite.colour, sprite.glTexture)) {
		emptySpriteBatch(sprite.glTexture);
		spriteBatchMesh->addSprite(modelMatrix, sprite.colour, sprite.glTexture);

	} 
}

void SpriteRenderer::emptySpriteBatch(GLuint newTexture){
	if (!spriteBatchMesh->numSpritesFilled == 0) {
		Matrix4 identityMatrix;
		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, (float*)& identityMatrix);
		glUniform4fv(glGetUniformLocation(currentShader->GetProgram(), "nodeColour"), 1, (float*)& Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "useTexture"), (int)spriteBatchMesh->GetTexture());
		spriteBatchMesh->drawSpriteBatch();
	}

	spriteBatchMesh->resetSpriteBatch(newTexture);
}

void SpriteRenderer::UpdateScene(float mse){
	viewMatrix = camera->BuildViewMatrix();
	frameFrustum.FromMatrix(projMatrix*viewMatrix);
}
