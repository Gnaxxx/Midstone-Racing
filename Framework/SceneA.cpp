#include "SceneA.h"
#include <SDL.h>
#include <SDL_image.h>
#include <math.h>
#include "Timer.h"

#define PI 3.14159265

SceneA::SceneA(SDL_Window* sdlWindow_) {
	window = sdlWindow_;
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr) {
		printf("%s\n", SDL_GetError());
	}
	car = new Body(Vec3(2.0f, 5.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 2.0);
	Timer::SetSingleEvent(5000, (void*)"Start");

}

SceneA::~SceneA() {
	delete car;

}

bool SceneA::OnCreate() {
	int w, h;
	SDL_GetWindowSize(window, &w, &h);

	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(0.0f, 30.0f, 0.0f, 15.0f, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;
	IMG_Init(IMG_INIT_PNG);
	SDL_Surface* carImage = IMG_Load("textures/PeepoPing.png");
	SDL_Texture* carTexture = SDL_CreateTextureFromSurface(renderer, carImage);
	if (carImage == nullptr) {
		printf("Can't open textures/PeepoPing.png\n");
		return false;
	}
	car->setTexture(carTexture);

	SDL_FreeSurface(carImage);

	return true;
}

void SceneA::OnDestroy() {}

void SceneA::Update(const float deltaTime) {

	car->Update(deltaTime);
}

void SceneA::HandleEvents(const SDL_Event& sdlEvent) {
	if (sdlEvent.type == SDL_EventType::SDL_KEYDOWN) {
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_D) {
			car->ApplyForce(Vec3(10.0f, 0.0f, 0.0f));
			car->isMoving(true);
		}
	}
	else if (sdlEvent.type == SDL_EventType::SDL_KEYUP) {
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_D) {
			car->ApplyForce(Vec3(0.0f, 0.0f, 0.0f));
			car->isMoving(false);
		}
	}

	if (sdlEvent.type == SDL_EventType::SDL_KEYDOWN) {
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_W) {
			car->ApplyForce(Vec3(0.0f, 10.0f, 0.0f));
			car->isMoving(true);
		}
	}
	else if (sdlEvent.type == SDL_EventType::SDL_KEYUP) {
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_W) {
			car->ApplyForce(Vec3(0.0f, 0.0f, 0.0f));
			car->isMoving(false);
		}
	}

	if (sdlEvent.type == SDL_EventType::SDL_KEYDOWN) {
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_A) {
			car->ApplyForce(Vec3(-10.0f, 0.0f, 0.0f));
			car->isMoving(true);
		}
	}
	else if (sdlEvent.type == SDL_EventType::SDL_KEYUP) {
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_A) {
			car->ApplyForce(Vec3(0.0f, 0.0f, 0.0f));
			car->isMoving(false);
		}
	}

	if (sdlEvent.type == SDL_EventType::SDL_KEYDOWN) {
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_S) {
			car->ApplyForce(Vec3(0.0f, -10.0f, 0.0f));
			car->isMoving(true);
		}
	}
	else if (sdlEvent.type == SDL_EventType::SDL_KEYUP) {
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_S) {
			car->ApplyForce(Vec3(0.0f, 0.0f, 0.0f));
			car->isMoving(false);
		}
	}
}

void SceneA::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 204, 255, 0);
	SDL_RenderClear(renderer);

	SDL_Rect square;
	Vec3 screenCoords;
	int w, h;

	screenCoords = projectionMatrix * car->getPos();
	SDL_QueryTexture(car->getTexture(), nullptr, nullptr, &w, &h);
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = w / 2;
	square.h = h / 2;

	float angle = atan2(car->getVelocity().y, car->getVelocity().x) * 180 / PI;


	if (car->getVelocity().x >= 0.0f) {
		SDL_RenderCopyEx(renderer, car->getTexture(), nullptr, &square, -angle, nullptr, SDL_FLIP_NONE);
	}
	else {

		SDL_RenderCopyEx(renderer, car->getTexture(), nullptr, &square, 180 - angle, nullptr, SDL_FLIP_HORIZONTAL);
	}

	SDL_RenderPresent(renderer);
}