#pragma once
#include "../../nclgl/OGLRenderer.h"

#include "RenderObject.h"

#include <vector>
#include "../../nclgl/WinRenderer.h"
#include "../sdlgl/SDLRenderer.h"

using std::vector;

class Renderer : public SDLRenderer	{
public:
	Renderer(SdlWindow &parent);
	~Renderer(void);

	virtual void	RenderScene();

	virtual void	Render(const RenderObject &o);

	virtual bool Init() override;

	virtual void	UpdateScene(float msec);

	void	AddRenderObject(RenderObject &r) {
		renderObjects.push_back(&r);
	}

	GLuint LoadTexture(const string& filename);

protected:

	vector<RenderObject*> renderObjects;
};

