#include "Renderer.h"
#include <GL/glew.h>

Renderer::Renderer(SdlWindow &parent) : SDLRenderer(parent)	{
	projMatrix = Matrix4::Perspective(1, 100, 1.33f, 45.0f);
	viewMatrix = Matrix4::BuildViewMatrix(Vector3(0, 0, 0), Vector3(0, 0, -10));
}

bool Renderer::Init() {
	SDLRenderer::Init();
	glEnable(GL_DEPTH_TEST);
	return true;
}

Renderer::~Renderer(void)	{
	 
}

void	Renderer::RenderScene() {
	for(vector<RenderObject*>::iterator i = renderObjects.begin(); i != renderObjects.end(); ++i ) {
		Render(*(*i));
	}
}

void	Renderer::Render(const RenderObject &o) {
	modelMatrix = o.GetWorldTransform();

	if(o.GetShader() && o.GetMesh()) {
		SetCurrentShader(o.GetShader());
		UpdateShaderMatrices();

		o.Draw();
	}

	for(vector<RenderObject*>::const_iterator i = o.GetChildren().begin(); i != o.GetChildren().end(); ++i ) {
		Render(*(*i));
	}
}


void	Renderer::UpdateScene(float msec) {
	for(vector<RenderObject*>::iterator i = renderObjects.begin(); i != renderObjects.end(); ++i ) {
		(*i)->Update(msec);
	}
}

GLuint Renderer::LoadTexture(const string& filename) {
	GLuint texName = SOIL_load_OGL_texture(filename.c_str(), SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	if (texName == 0) {
		cout << "Texture file " << filename << " failed to load!" << std::endl;
	}

	return texName;
}