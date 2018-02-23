////Using SDL and standard IO
//#include <SDL.h>
//#include <stdio.h>
//#include <iostream>
//
////Screen dimension constants
//const int SCREEN_WIDTH = 640;
//const int SCREEN_HEIGHT = 480;
//
////The window we'll be rendering to
//SDL_Window* gWindow = NULL;
//
////The surface contained by the window
//SDL_Surface* gScreenSurface = NULL;
//
////The image we will load and show on the screen
//SDL_Surface* gHelloWorld = NULL;
//
////The window renderer
//SDL_Renderer* gRenderer = NULL;
//
////Event handler
//SDL_Event e;
//
//bool quit = false;
//
//bool init()
//{
//	//Initialization flag
//	bool success = true;
//
//	//Initialize SDL
//	if (SDL_Init(SDL_INIT_VIDEO) < 0)
//	{
//		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
//		success = false;
//	} else
//	{
//		//Create window
//		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
//		if (gWindow == NULL)
//		{
//			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
//			success = false;
//		} else
//		{
//			//Get window surface
//			gScreenSurface = SDL_GetWindowSurface(gWindow);
//
//
//			//Create renderer for window
//			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
//			if (gRenderer == NULL)
//			{
//				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
//				success = false;
//			} else
//			{
//				//Initialize renderer color
//				SDL_SetRenderDrawColor(gRenderer, 0x64, 0x95, 0xED, 0xFF);
//			}
//			
//		}
//	}
//
//	return success;
//}
//
//bool loadMedia()
//{
//	//Loading success flag
//	bool success = true;
//
//	//Nothing to load
//	return success;
//}
//
//void close()
//{
//	//Free loaded image
//	
//
//	//Destroy window    
//	SDL_DestroyRenderer(gRenderer);
//	SDL_DestroyWindow(gWindow);
//	gWindow = NULL;
//	gRenderer = NULL;
//
//	//Quit SDL subsystems
//	SDL_Quit();
//}
//
//
//int main(int argc, char* args[])
//{
//	
//	if (init()) {
//		if (loadMedia()) {
//
//			//While application is running
//			while (!quit)
//			{
//				//Handle events on queue
//				while (SDL_PollEvent(&e) != 0)
//				{
//					//User requests quit
//					if (e.type == SDL_QUIT)
//					{
//						quit = true;
//					}
//				}
//
//				
//				//Clear screen
//				SDL_SetRenderDrawColor(gRenderer, 0x64, 0x95, 0xED, 0xFF);
//				SDL_RenderClear(gRenderer);
//
//				//Render red filled quad
//				SDL_Rect fillRect = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
//				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x14, 0x93, 0xFF);
//				SDL_RenderFillRect(gRenderer, &fillRect);
//				
//				//Update screen
//				SDL_RenderPresent(gRenderer);
//			}
//		}
//	}
//
//	close();
//
//	std::cin.get();
//	return 0;
//}
