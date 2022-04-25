#include "YouWin.h"
#include <SDL.h>
#include <SDL_image.h>

YouWin::YouWin(SDL_Window* sdlWindow_) {
	window = sdlWindow_;
	youWin = new Body(Vec3(0.0f, 20.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1.0f);

}

YouWin::~YouWin() {
	delete youWin;
}

bool YouWin::OnCreate() {
	int w, h;
	SDL_GetWindowSize(window, &w, &h);

	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(0.0f, 30.0f, 0.0f, 15.0f, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;
	IMG_Init(IMG_INIT_PNG);
	SDL_Surface* winImage = IMG_Load("textures/YouWin.png");
	if (winImage == nullptr) {
		printf("Can't open textures/YouWin.png\n");
		return false;
	}
	youWin->setImage(winImage);


	return true;
}

void YouWin::OnDestroy() {}

void YouWin::Update(const float deltaTime) {
	youWin->Update(deltaTime);
}

void YouWin::HandleEvents(const SDL_Event& sdlEvent) {

}

void YouWin::Render() {
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

	screenCoords = projectionMatrix * youWin->getPos();
	square.x = (int)screenCoords.x; /// We must update this with C11 typecasting - SSF
	square.y = (int)screenCoords.y;
	square.w = 10;
	square.h = 10;
	SDL_BlitSurface(youWin->getImage(), nullptr, screenSurface, &square);

	SDL_UpdateWindowSurface(window);
	//SDL_RenderPresent(renderer);

}