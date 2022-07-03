#include "GameInstance.h"

GameInstance* gameInstance = nullptr;


int main(int argc,char* argv[])
{
	gameInstance = new GameInstance();
	gameInstance->Init(
		"NebulaEngine",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		800,
		600,
		false);

	gameInstance->Start();

	while (gameInstance->IsRunning())
	{
		gameInstance->HandleEvents();
		gameInstance->Update();
		gameInstance->FixedUpdate();
		gameInstance->RenderUpdate();
	}
	gameInstance->Clean();
	return 0;
}