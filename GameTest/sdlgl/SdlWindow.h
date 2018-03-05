#pragma once
#include <SDL.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include <../../nclgl/Window.h>
using namespace std;

class SdlWindow{
public:
	SdlWindow(const string title, const int width, const int height, const bool fullScreen = false) : title(title), width(width), height(height), fullScreen(fullScreen){}

	~SdlWindow(){
		if (window) {
			SDL_DestroyWindow(window);
			window = nullptr;
		}

	}

	/*
	 * Use sdl to initialise our window
	 */
	bool init();

	void registerResizeCallback(ResizeCallbackFunction func);

	/*
	 * gets the underlying sdl_window
	 */
	SDL_Window* getWindow() const { return window; }

	void resize(const int w, const int h);

	void handleEvent(SDL_Event& e);
protected:
	string title;
	int width, height;
	bool fullScreen;
	bool minimised;

	SDL_Window* window = nullptr;

	std::vector<ResizeCallbackFunction> resizeListeners;

};

