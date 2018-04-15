#include "SdlWindow.h"


bool SdlWindow::init(){
	Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
	if (fullScreen) {
		flags = flags | (SDL_WINDOW_FULLSCREEN);
	}
	window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);
	if (window == NULL){
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	return true;
}

void SdlWindow::registerResizeCallback(const ResizeCallbackFunction func) {
	resizeListeners.push_back(func);
	func(width, height);
}

void SdlWindow::resize(const int w, const  int h) {
	width = w;
	height = h;
	
	//call our resize callbacks
	for (std::vector<ResizeCallbackFunction>::iterator i = resizeListeners.begin(); i != resizeListeners.end(); ++i) {
		(*i)(width, height);
	}
}

void SdlWindow::handleEvent(SDL_Event& e){
	//Window event occured
	if (e.type == SDL_WINDOWEVENT) {
		switch (e.window.event)
		{
			//Get new dimensions and repaint on window size change
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				width = e.window.data1;
				height = e.window.data2;
				resize(width, height);
				break;
		}
	}
}


