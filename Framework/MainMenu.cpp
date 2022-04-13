#include "MainMenu.h"
#include <SDL.h>
#include <SDL_image.h>

MainMenu::MainMenu(SDL_Window* sdlWindow_) {
	window = sdlWindow_;
	startButton = new Body(Vec3(0.0f, 20.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1.0f);
	
}

MainMenu::~MainMenu() {
	delete startButton;
}

bool MainMenu::OnCreate() {
	int w, h;
	SDL_GetWindowSize(window, &w, &h);

	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(0.0f, 30.0f, 0.0f, 15.0f, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;
	IMG_Init(IMG_INIT_PNG);
	SDL_Surface* buttonImage = IMG_Load("textures/startgame.png");
	if (buttonImage == nullptr) {
		printf("Can't open textures/PeepoPing.png\n");
		return false;
	}
	startButton->setImage(buttonImage);


	return true;
}

void MainMenu::OnDestroy() {}

void MainMenu::Update(const float deltaTime) {
	startButton->Update(deltaTime);
}

void MainMenu::HandleEvents(const SDL_Event& sdlEvent) {
	
}

void MainMenu::Render() {
	/*SDL_SetRenderDrawColor(renderer, 0, 204, 255, 0);
	SDL_RenderClear(renderer);*/

	SDL_Surface* screenSurface = SDL_GetWindowSurface(window);

	SDL_FillRect(screenSurface, nullptr, SDL_MapRGB(screenSurface->format, 0, 204, 255));

	SDL_Rect square;
	Vec3 screenCoords;
	int w, h;

	/*screenCoords = projectionMatrix * startButton->getPos();
	SDL_QueryTexture(startButton->getTexture(), nullptr, nullptr, &w, &h);
	square.x = static_cast<int>(screenCoords.x); /// We must update this with C11 typecasting - SSF
	square.y = static_cast<int>(screenCoords.y);
	square.w = w / 2;
	square.h = h / 2;
	SDL_RenderCopyEx(renderer, startButton->getTexture(), nullptr, &square, 0.0, nullptr, SDL_FLIP_NONE);*/

	screenCoords = projectionMatrix * startButton->getPos();
	square.x = (int)screenCoords.x; /// We must update this with C11 typecasting - SSF
	square.y = (int)screenCoords.y;
	square.w = 30;
	square.h = 30;
	SDL_BlitSurface(startButton->getImage(), nullptr, screenSurface, &square);

	SDL_UpdateWindowSurface(window);
	//SDL_RenderPresent(renderer);

}