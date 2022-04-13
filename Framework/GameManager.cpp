#include "GameManager.h"
#include "Window.h"
#include "Timer.h"
#include "MainMenu.h"
#include "GameOver.h"
#include <iostream>

GameManager::GameManager() {
	windowPtr = nullptr;
	timer = nullptr;
	isRunning = true;
	currentScene = nullptr;
	sceneA = nullptr;
}


/// In this OnCreate() method, fuction, subroutine, whatever the word, 
bool GameManager::OnCreate() {
	const int SCREEN_WIDTH = 1280;
	const int SCREEN_HEIGHT = 720;
	windowPtr = new Window(SCREEN_WIDTH, SCREEN_HEIGHT);

	if (windowPtr == nullptr) {
		OnDestroy();
		return false;
	}
	if (windowPtr->OnCreate() == false) {
		OnDestroy();
		return false;
	}

	timer = new Timer();
	if (timer == nullptr) {
		OnDestroy();
		return false;
	}

	//currentScene = new SceneA(windowPtr->GetSDL_Window());

	BuildScene(MAINMENU);

	if (currentScene == nullptr) {
		OnDestroy();
		return false;
	}

	if (currentScene->OnCreate() == false) {
		OnDestroy();
		return false;
	}

	return true;
}

/// Here's the whole game
void GameManager::Run() {
	SDL_Event sdlEvent;
	timer->Start();
	isRunning = true;
	while (isRunning) {
		/*while (SDL_PollEvent(&sdlEvent) != 0) {

			if (sdlEvent.type == SDL_EventType::SDL_QUIT) {
				isRunning = false;
			}
			else if (sdlEvent.type == SDL_EventType::SDL_KEYDOWN) {
				if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
					isRunning = false;
				}
				else if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_Q) {
					isRunning = false;
				}
			}
			currentScene->HandleEvents(sdlEvent);
		}*/


		timer->UpdateFrameTicks();
		//currentScene->HandleEvents(sdlEvent);
		currentScene->Update(timer->GetDeltaTime());
		currentScene->Render();
		GetEvents();
		SDL_GL_SwapWindow(windowPtr->GetSDL_Window());
		/// Keeep the event loop running at a proper rate
		SDL_Delay(timer->GetSleepTime(60)); ///60 frames per sec
	}
}

void GameManager::GetEvents() {
	SDL_Event sdlEvent;
	while (SDL_PollEvent(&sdlEvent)) {
		const Uint8* state = SDL_GetKeyboardState(nullptr); /// Do not free this! Read the docs
		if (sdlEvent.type == SDL_EventType::SDL_QUIT) {
			isRunning = false;
			return;
		}

		else if (sceneA && sceneA->getIsDead()) {
			BuildScene(GAMEOVER);
		}

		else if (sdlEvent.type == SDL_KEYDOWN) {
			switch (sdlEvent.key.keysym.scancode) {
			case SDL_SCANCODE_ESCAPE:
			case SDL_SCANCODE_Q: 
				isRunning = false;
				return;

			
			case SDL_SCANCODE_F1:
				if (state[SDL_SCANCODE_RSHIFT] || state[SDL_SCANCODE_LSHIFT]) {

				}
				else {
					BuildScene(MAINMENU);
				}
				break;

			case SDL_SCANCODE_SPACE:
				if (state[SDL_SCANCODE_RSHIFT] || state[SDL_SCANCODE_LSHIFT]) {

				}
				else {
					BuildScene(SCENEA);
				}
				break;

			

			default:
				currentScene->HandleEvents(sdlEvent);
				break;
			}
		}
		if (currentScene == nullptr) {
			//Debug::FatalError("Failed to initialize Scene", __FILE__, __LINE__);
			isRunning = false;
			return;
		}
		currentScene->HandleEvents(sdlEvent);
	}
}

void GameManager::BuildScene(SCENE_NUMBER scene) {
	bool status;

	if (currentScene != nullptr) {
		currentScene->OnDestroy();
		delete currentScene;
		currentScene = nullptr;
	}

	switch (scene) {
	case MAINMENU:
		currentScene = new MainMenu(windowPtr->GetSDL_Window());
		status = currentScene->OnCreate();
		break;

	case GAMEOVER:
		currentScene = new GameOver(windowPtr->GetSDL_Window());
		status = currentScene->OnCreate();
		break;

	case SCENEA:
		currentScene = new SceneA(windowPtr->GetSDL_Window());
		sceneA = (SceneA*) currentScene;
		status = currentScene->OnCreate();
		break;

	default:
		//Debug::Error("Incorrect scene number assigned in the manager", __FILE__, __LINE__);
		currentScene = nullptr;
		break;
	}
}

GameManager::~GameManager() {}

void GameManager::OnDestroy(){
	if (windowPtr) delete windowPtr;
	if (timer) delete timer;
	if (currentScene) delete currentScene;
}