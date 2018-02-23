#pragma once
#include <SDL.h>
#include <string>
#include <stdio.h>
#include <iostream>
using namespace std;

class SdlWindow
{
public:
	SdlWindow(const string title, const int width, const int height, const bool fullScreen = false) : title(title), width(width), height(height), fullScreen(fullScreen){}

	~SdlWindow(){
		if (window) {
			SDL_DestroyWindow(window);
			window = nullptr;
		}
		if(mainContext){
			SDL_GL_DeleteContext(mainContext);
		}
	}

	/*
	 * Use sdl to initialise our window
	 */
	bool init();

	/*
	 * set up the opengl render context for this window to use
	 */
	bool createRenderContext();
	/*
	 * gets the underlying sdl_window
	 */
	SDL_Window* getWindow() const { return window; }
private:
	string title;
	int width, height;
	bool fullScreen;
	bool minimised;

	SDL_Window* window = nullptr;

	// Our opengl context handle
	SDL_GLContext mainContext;

};

