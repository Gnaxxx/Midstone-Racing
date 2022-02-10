#include "Scene0.h"
#include <SDL.h>
#include <SDL_image.h>

Scene0::Scene0(SDL_Window* sdlWindow_){
	window = sdlWindow_;
	balloon = new Body(Vec3(0.0f, 15.7f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, -9.81, 0.0f), 1.0f);
	cliff = new Body(Vec3(0.0f, 15.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1.0f);
}

Scene0::~Scene0(){
	delete balloon;
	delete cliff;
}

bool Scene0::OnCreate() {
	int w, h;
	SDL_GetWindowSize(window,&w,&h);
	
	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(0.0f, 30.0f, 0.0f, 15.0f, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;
	IMG_Init(IMG_INIT_PNG);
	SDL_Surface* ballImage = IMG_Load("textures/PeepoPing.png");
	if (ballImage == nullptr) {
		printf("Can't open textures/PeepoPing.png\n");
		return false;
	}
	balloon->setImage(ballImage);

	SDL_Surface* cliffImage = IMG_Load("textures/cliff.png");
	if (cliffImage == nullptr) {
		printf("Can't open textures/cliff.png\n");
		return false;
	}
	cliff->setImage(cliffImage);
	
	return true;
}

void Scene0::OnDestroy() {}

void Scene0::Update(const float deltaTime) {
	balloon->Update(deltaTime);
}

void Scene0::HandleEvents(const SDL_Event& sdlEvent) {
	if (sdlEvent.type == SDL_EventType::SDL_KEYDOWN) {
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_D) {
			balloon->ApplyForce(Vec3(10.0f, 0.0f, 0.0f));
			balloon->isMoving(true);
		}
	}
	else if (sdlEvent.type == SDL_EventType::SDL_KEYUP) {
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_D) {
			balloon->ApplyForce(Vec3(0.0f, 0.0f, 0.0f));
		}
	}
	
	if (sdlEvent.type == SDL_EventType::SDL_KEYDOWN) {
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_A) {
			balloon->ApplyForce(Vec3(-10.0f, 0.0f, 0.0f));
			balloon->isMoving(true);
		}
	}
	else if (sdlEvent.type == SDL_EventType::SDL_KEYUP) {
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_A) {
			balloon->ApplyForce(Vec3(0.0f, 0.0f, 0.0f));
		}
	}
}

void Scene0::Render() {
	SDL_Surface *screenSurface = SDL_GetWindowSurface(window);
	SDL_FillRect(screenSurface, nullptr, SDL_MapRGB(screenSurface->format, 0, 204, 255));
	SDL_Rect square;
	Vec3 screenCoords;

	screenCoords = projectionMatrix * cliff->getPos();
	square.x = (int)screenCoords.x; /// We must update this with C11 typecasting - SSF
	square.y = (int)screenCoords.y;
	square.w = 30;
	square.h = 30;
	SDL_BlitSurface(cliff->getImage(), nullptr, screenSurface, &square);

	screenCoords = projectionMatrix * balloon->getPos();
	square.x = (int) screenCoords.x; /// We must update this with C11 typecasting - SSF
	square.y = (int) screenCoords.y;
	square.w = 30;
	square.h = 30;
	// SDL_FillRect(screenSurface, &square, SDL_MapRGB(screenSurface->format,255, 255, 0));
	SDL_BlitSurface(balloon->getImage(), nullptr, screenSurface, &square);


	SDL_UpdateWindowSurface(window);
}