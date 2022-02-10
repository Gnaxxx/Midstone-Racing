#include "SceneA.h"
#include <SDL.h>
#include <SDL_image.h>
#include <math.h>
#include "Timer.h"



SceneA::SceneA(SDL_Window* sdlWindow_) {
	window = sdlWindow_;
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr) {
		printf("%s\n", SDL_GetError());
	}
	ship = new Body2(Vec3(0.0f, 0.0f, 0.0f), 1000000.0f, 26700000000.0f);

	Timer::SetSingleEvent(5000, (void*)"Start");

}

SceneA::~SceneA() {
	delete ship;
}

bool SceneA::OnCreate() {
	int w, h;
	SDL_GetWindowSize(window, &w, &h);

	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(-90.0f, 90.0f, -45.0f, 45.0f, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;

	IMG_Init(IMG_INIT_PNG);
	SDL_Surface* shipImage = IMG_Load("textures/skull_texture.jpg");
	SDL_Texture* shipTexture = SDL_CreateTextureFromSurface(renderer, shipImage);

	if (shipImage == nullptr) {
		printf("Can't open textures/skull_texture.jpg\n");
		return false;
	}
	ship->setTexture(shipTexture);

	SDL_FreeSurface(shipImage);

	return true;

}

void SceneA::OnDestroy() {}

void SceneA::Update(const float deltaTime) {
	const Vec3 thrustForce = Vec3(50000000.0f, 0.0f, 0.0f);
	if (deltaTime < 10.0f) {
		ship->ApplyForce(thrustForce);
	}
	else {
		ship->ApplyTorque(629100000.0f);
		ship->ApplyForce(thrustForce / 2);
	}

	ship->Update(deltaTime);
}

void SceneA::HandleEvents(const SDL_Event& sdlEvent) {}

void SceneA::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 204, 255, 0);
	SDL_RenderClear(renderer);

	SDL_Rect square;
	Vec3 screenCoords;
	int w, h;

	screenCoords = projectionMatrix * ship->GetPos();
	SDL_QueryTexture(ship->getTexture(), nullptr, nullptr, &w, &h);
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = w / 2;
	square.h = h / 2;
	SDL_RenderCopyEx(renderer, ship->getTexture(), nullptr, &square, 0.0, nullptr, SDL_FLIP_NONE);

	SDL_RenderPresent(renderer);
}
