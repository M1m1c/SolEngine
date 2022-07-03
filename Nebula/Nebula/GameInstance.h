#pragma once
#include "SDL.h"

class GameInstance
{
public:
	GameInstance();
	~GameInstance();

	void Init(const char* title, int xPos, int yPos, int width, int height, bool fullscreen);

	void HandleEvents();
	void Start();
	void Update();
	void FixedUpdate();
	void RenderUpdate();
	void Clean();

	bool IsRunning();

	//TODO introduce the concept of scenes, 
	//only call functions on all objects in scene

private:
	bool bIsRunning;

	SDL_Window* window;
	SDL_Renderer* renderer;

};

