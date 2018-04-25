/*
reimplements original nclgl renderer
windows specific components will exist in here
*/

#pragma once
#include "OGLRenderer.h"
class WinRenderer :
	public OGLRenderer
{
public:
	WinRenderer(Window &window); 
	~WinRenderer();

	bool Init() override;
	void SwapBuffers() override;
	
private:
	Window& window;
	HWND windowHandle;
	HDC		deviceContext;	//...Device context?
	HGLRC	renderContext;	//Permanent Rendering Context
};

