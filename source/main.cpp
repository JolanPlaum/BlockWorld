#include "pch.h"

#if defined(_DEBUG)
#include "vld.h"
#endif

#undef main
#include "Renderer.h"
#include <thread>

using namespace dae;

void ShutDown(SDL_Window* pWindow)
{
	SDL_DestroyWindow(pWindow);
	SDL_Quit();
}

int main(int argc, char* args[])
{
	//Unreferenced parameters
	(void)argc;
	(void)args;

	//Create window + surfaces
	SDL_Init(SDL_INIT_VIDEO);

	const uint32_t width = 1280;
	const uint32_t height = 720;

	SDL_Window* pWindow = SDL_CreateWindow(
		"DirectX - **Jolan Plaum**",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		width, height, 0);

	if (!pWindow)
		return 1;

	//Initialize "framework"
	const auto pTimer = new Timer();
	const auto pRenderer = new Renderer(pWindow);

	//Start loop
	pTimer->Start();
	float printTimer = 0.f;
	bool isLooping = true;

	std::thread worldThread{ [&]() {while (isLooping) pRenderer->UpdateWorld(0); } };
	std::thread worldThread1{ [&]() {while (isLooping) pRenderer->UpdateWorld(1); } };
	std::thread worldThread2{ [&]() {while (isLooping) pRenderer->UpdateWorld(2); } };
	std::thread worldThread3{ [&]() {while (isLooping) pRenderer->UpdateWorld(3); } };
	std::thread chunkThread{ [&]() {while (isLooping) pRenderer->UpdateChunkManager(); } };
	while (isLooping)
	{
		//--------- Get input events ---------
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_QUIT:
				isLooping = false;
				break;
			case SDL_KEYUP:
				//Test for a key
				if (e.key.keysym.scancode == SDL_SCANCODE_F2)
					pRenderer->ToggleChunkLoading();
				break;
			default: ;
			}
		}

		//--------- Update ---------
		pRenderer->Update(pTimer);

		//--------- Render ---------
		pRenderer->Render();

		//--------- Timer ---------
		pTimer->Update();
		printTimer += pTimer->GetElapsed();
		if (printTimer >= 1.f)
		{
			printTimer = 0.f;
			std::cout << "dFPS: " << pTimer->GetdFPS() << std::endl;
		}
	}
	pTimer->Stop();
	worldThread.join();
	worldThread1.join();
	worldThread2.join();
	worldThread3.join();
	chunkThread.join();

	//Shutdown "framework"
	delete pRenderer;
	delete pTimer;

	ShutDown(pWindow);
	return 0;
}