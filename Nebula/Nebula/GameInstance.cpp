#include "GameInstance.h"
#include <iostream>

GameInstance::GameInstance()
{
}

GameInstance::~GameInstance()
{
}

void GameInstance::Init(const char* title, int xPos, int yPos, int width, int height, bool fullscreen)
{
	int flags = 0;
	if (fullscreen) { flags = SDL_WINDOW_FULLSCREEN; }

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "subsystems Initalized!" << std::endl;

		window = SDL_CreateWindow(title, xPos, yPos, width, height, flags);
		if (window) { std::cout << "window created" << std::endl; }
		
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) 
		{ 
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			std::cout << "rendered created" << std::endl; 
		}
		bIsRunning = true;
	}
	else { bIsRunning = false; }
}

void GameInstance::HandleEvents()
{
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type)
	{
	case SDL_QUIT:
		bIsRunning = false;
		break;
	default:
		break;
	}
}

void GameInstance::Start()
{
}

void GameInstance::Update()
{
	//TODO call Update functions in all IUpdatable objects and pass delta time
}

void GameInstance::FixedUpdate()
{
	//TODO call FixedUpdate functions in all IUpdatable objects
}

void GameInstance::RenderUpdate()
{
	//TODO update rendering for everything on screen
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
}

void GameInstance::Clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game cleaned" << std::endl;
}

bool GameInstance::IsRunning()
{
	return bIsRunning;
}
