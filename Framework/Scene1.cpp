#include "Scene1.h"
#include <SDL.h>
#include <SDL_image.h>
#include <math.h>
#include "Timer.h"

#define PI 3.14159265

Scene1::Scene1(SDL_Window* sdlWindow_){
	window = sdlWindow_;
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr) {
		printf("%s\n", SDL_GetError());
	}
	pepe = new Body(Vec3(2.0f, 5.0f, 0.0f), Vec3(26.73f, 13.62f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 2.0);
	cliff = new Body(Vec3(2.0f, 4.7f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1.0f);
	Timer::SetSingleEvent(5000, (void*)"Start");

}

Scene1::~Scene1(){
	delete pepe;
	delete cliff;
}

bool Scene1::OnCreate() {
	int w, h;
	SDL_GetWindowSize(window,&w,&h);
	
	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(0.0f, 30.0f, 0.0f, 15.0f, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;
	IMG_Init(IMG_INIT_PNG);
	SDL_Surface *ballImage = IMG_Load("textures/PeepoPing.png");
	SDL_Texture* ballTexture = SDL_CreateTextureFromSurface(renderer, ballImage);
	if (ballImage == nullptr) {
		printf("Can't open textures/PeepoPing.png\n");
		return false;
	}
	pepe->setTexture(ballTexture);

	SDL_Surface* cliffImage = IMG_Load("textures/cliff.png");
	SDL_Texture* cliffTexture = SDL_CreateTextureFromSurface(renderer, cliffImage);
	if (cliffImage == nullptr) {
		printf("Can't open textures/cliff.png\n");
		return false;
	}
	cliff->setTexture(cliffTexture);
	SDL_FreeSurface(ballImage);
	SDL_FreeSurface(cliffImage);
	
	return true;
}

void Scene1::OnDestroy() {}

void Scene1::Update(const float deltaTime) {

	float c = 0.2f;
	Vec3 dragForce;
	Vec3 gravityForce(0.0f, -9.81f * pepe->getMass(), 0.0f);
	Vec3 windForce(-30.0f, 0.0f, 0.0f);
	Vec3 totalForce;

	dragForce += -c * pepe->getVelocity();

	totalForce = windForce + gravityForce + dragForce;
	pepe->ApplyForce(totalForce);

	pepe->Update(deltaTime);
}

void Scene1::HandleEvents(const SDL_Event& sdlEvent) {
	if (sdlEvent.type == SDL_EventType::SDL_KEYDOWN) {
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_D) {
			pepe->ApplyForce(Vec3(0.0f, 0.0f, 0.0f));
			pepe->isMoving(true);
		}
	}
	else if (sdlEvent.type == SDL_EventType::SDL_KEYUP) {
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_D) {
			//while (balloon->getVelocity().x != 0) {
			pepe->ApplyForce(Vec3(0.0f, 0.0f, 0.0f));
			//}
		}
	}
	
	/*if (sdlEvent.type == SDL_EventType::SDL_KEYDOWN) {
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_A) {
			balloon->ApplyForce(Vec3(-30.0f, 0.0f, 0.0f));
			balloon->isMoving(true);
		}
	}
	else if (sdlEvent.type == SDL_EventType::SDL_KEYUP) {
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_A) {
			balloon->ApplyForce(Vec3(0.0f, 0.0f, 0.0f));
		}
	}

	else if (sdlEvent.type == SDL_USEREVENT) {
		//printf("%d %s\n", sdlEvent.user.data1, sdlEvent.user.data2);

	}*/
}

void Scene1::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 204, 255, 0);
	SDL_RenderClear(renderer);

	SDL_Rect square;
	Vec3 screenCoords;
	int w, h;

	screenCoords = projectionMatrix * cliff->getPos();
	SDL_QueryTexture(cliff->getTexture(), nullptr, nullptr, &w, &h);
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = w / 2;
	square.h = h / 2;
	SDL_RenderCopyEx(renderer, cliff->getTexture(), nullptr, &square, 0.0, nullptr, SDL_FLIP_NONE);

	screenCoords = projectionMatrix * pepe->getPos();
	SDL_QueryTexture(pepe->getTexture(), nullptr, nullptr, &w, &h);
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = w / 2;
	square.h = h / 2;

	float angle = atan2(pepe->getVelocity().y, pepe->getVelocity().x) * 180 / PI;


	if (pepe->getVelocity().x >= 0.0f) {
		SDL_RenderCopyEx(renderer, pepe->getTexture(), nullptr, &square, -angle, nullptr, SDL_FLIP_NONE);
	}
	else {
		
		SDL_RenderCopyEx(renderer, pepe->getTexture(), nullptr, &square, 180-angle, nullptr, SDL_FLIP_HORIZONTAL);
	}

	SDL_RenderPresent(renderer);
}