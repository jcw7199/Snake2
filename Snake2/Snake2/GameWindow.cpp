#include "GameWindow.h"
#include <iostream>

GameWindow::GameWindow(const char* title, const char* backgroundImage, SDL_Rect rect) {
	cout << "GameWindow::GameWindow\n";
	windowRect = &rect;
	gameTitle = title;
	gameWindow = nullptr;

	//windowRen = SDL_CreateRenderer(gameWindow, -1, SDL_RENDERER_SOFTWARE);
	windowSurface = nullptr;

	imageSurface = nullptr;
	backgroundSurface = SDL_LoadBMP(backgroundImage);
	
	
	//windowTexture = SDL_CreateTextureFromSurface(windowRen, windowSurface);
	//SDL_SetRenderDrawColor(windowRen, 0, 0, 200, 255);

	cout << "	Window created\n";
}

bool GameWindow::init() {
	cout << "GameWindow::init()\n";
	
	bool success = true;
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		gameWindow = SDL_CreateWindow(gameTitle, windowRect->x, windowRect->y, backgroundSurface->w, backgroundSurface->h, SDL_WINDOW_SHOWN);
		
		//Create window
		if (gameWindow == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			windowSurface = SDL_GetWindowSurface(gameWindow);
		}
			
		return success;
	}
	
}

bool GameWindow::loadObject(GameObject object)
{
	//Loading success flag
	bool success = true;

	//Load splash image
	imageSurface = SDL_LoadBMP(object.getTextureFile());
	if (imageSurface == NULL)
	{
		printf("Unable to load image %s! SDL Error: %s\n", object.getTextureFile(), SDL_GetError());
		success = false;
	}	
	
	if (SDL_BlitSurface(imageSurface, object.getRect(), windowSurface, NULL) < 0)
	{
		cout << "blit error: " << SDL_GetError() << endl;
	}

	if (SDL_UpdateWindowSurface(gameWindow) < 0)
	{
		cout << "update error: " << SDL_GetError() << endl;
	}

	cout << "image loaded" << endl;
	return success;
}

bool GameWindow::eventHandler() {
	cout << "polling started" << endl;
	SDL_Event e;
	bool quit = false;
	while (quit == false)
	{
		while (SDL_PollEvent(&e))
		{
			cout << "Polling" << endl;
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
		}

	}
	return quit;
}

void GameWindow::addToRenderer(const char* file, SDL_Rect rect) {
	//cout << "RENDER: " << file << endl;

	//swap renderers 
	SDL_Texture* t = SDL_CreateTextureFromSurface(windowRen, IMG_Load(file));
	
	SDL_RenderCopy(windowRen, t, NULL, &rect);
	SDL_DestroyTexture(t);
	t = nullptr;
	
	

}

void GameWindow::present() {
	SDL_RenderPresent(windowRen);
	SDL_RenderClear(windowRen);
	
}

GameWindow& GameWindow::operator=(const GameWindow& other) {
	if(this->gameWindow != other.gameWindow){
		SDL_DestroyWindow(this->gameWindow);
		SDL_DestroyRenderer(this->windowRen);
		this->gameWindow = nullptr;
		this->windowRen = nullptr;
	}
	this->gameWindow = other.gameWindow;
	this->windowRen = other.windowRen;
	this->gameTitle = other.gameTitle;
	this->windowRect = other.windowRect;

	return *this;
}

void GameWindow::deleteWindow() {
	SDL_DestroyRenderer(windowRen);
	SDL_DestroyWindow(gameWindow);
	SDL_FreeSurface(windowSurface);
	SDL_FreeSurface(imageSurface);

	imageSurface = nullptr;
	windowSurface = nullptr;
	gameTitle = nullptr;
	windowRen = nullptr;
	gameWindow = nullptr;

	SDL_Quit();
}

GameWindow::~GameWindow() {
	this->deleteWindow();
}