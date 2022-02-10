#include "Scene3.h"
#include <SDL.h>
#include <SDL_image.h>
#include <math.h>
#include "Timer.h"
#include "VMath.h"

#define PI 3.14159265

Scene3::Scene3(SDL_Window* sdlWindow_) {
	window = sdlWindow_;
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr) {
		printf("%s\n", SDL_GetError());
	}
	pepe = new Body(Vec3(10.0f, 20.0f, 0.0f), Vec3(6.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 1.0);
	flappy1 = new Body(Vec3(5.0f, 10.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 100.0f);
	flappy2 = new Body(Vec3(15.0f, 25.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 150.0f);
	Timer::SetSingleEvent(5000, (void*)"Start");

}

Scene3::~Scene3() {
	delete pepe;
	delete flappy1;
	delete flappy2;
}

bool Scene3::OnCreate() {
	int w, h;
	SDL_GetWindowSize(window, &w, &h);

	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(-10.0f, 40.0f, -10.0f, 40.0f, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;
	IMG_Init(IMG_INIT_PNG);

	SDL_Surface* pepeImage = IMG_Load("textures/PeepoPing.png");
	SDL_Texture* pepeTexture = SDL_CreateTextureFromSurface(renderer, pepeImage);
	if (pepeImage == nullptr) {
		printf("Can't open textures/PeepoPing.png\n");
		return false;
	}
	pepe->setTexture(pepeTexture);

	SDL_Surface* flappy1Image = IMG_Load("textures/flappybird.png");
	SDL_Texture* flappy1Texture = SDL_CreateTextureFromSurface(renderer, flappy1Image);
	if (flappy1Image == nullptr) {
		printf("Can't open textures/cliff.png\n");
		return false;
	}
	flappy1->setTexture(flappy1Texture);

	SDL_Surface* flappy2Image = IMG_Load("textures/flappybird.png");
	SDL_Texture* flappy2Texture = SDL_CreateTextureFromSurface(renderer, flappy2Image);
	if (flappy2Image == nullptr) {
		printf("Can't open textures/cliff.png\n");
		return false;
	}
	flappy2->setTexture(flappy2Texture);

	SDL_FreeSurface(pepeImage);
	SDL_FreeSurface(flappy1Image);
	SDL_FreeSurface(flappy2Image);

	return true;
}

void Scene3::OnDestroy() {}

void Scene3::Update(const float deltaTime) {

	float g = 1.0f;

	Vec3 pepeToFlappy1dir = flappy1->getPos() - pepe->getPos();
	float r1 = VMath::mag(pepeToFlappy1dir);
	float gravFlappy1Mag = (g * flappy1->getMass() * pepe->getMass()) / (pow(r1, 2));
	Vec3 normPepeToFlappy1dir = VMath::normalize(pepeToFlappy1dir);
	Vec3 flappy1Grav = gravFlappy1Mag * normPepeToFlappy1dir;

	Vec3 pepeToFlappy2dir = flappy2->getPos() - pepe->getPos();
	float r2 = VMath::mag(pepeToFlappy2dir);
	float gravFlappy2Mag = (g * flappy2->getMass() * pepe->getMass()) / (pow(r2, 2));
	Vec3 normPepeToFlappy2dir = VMath::normalize(pepeToFlappy2dir);
	Vec3 flappy2Grav = gravFlappy2Mag * normPepeToFlappy2dir;

	Vec3 netForce = flappy1Grav + flappy2Grav;

	//std::cout << netForce.x << " " << netForce.y << std::endl;
	pepe->ApplyForce(netForce);

	pepe->Update(deltaTime);
}

void Scene3::HandleEvents(const SDL_Event& sdlEvent) {
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

void Scene3::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	SDL_Rect square;
	Vec3 screenCoords;
	int w, h;

	screenCoords = projectionMatrix * flappy1->getPos();
	SDL_QueryTexture(flappy1->getTexture(), nullptr, nullptr, &w, &h);
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = w * 2;
	square.h = h * 2;
	SDL_RenderCopyEx(renderer, flappy1->getTexture(), nullptr, &square, 0.0, nullptr, SDL_FLIP_NONE);

	screenCoords = projectionMatrix * flappy2->getPos();
	SDL_QueryTexture(flappy2->getTexture(), nullptr, nullptr, &w, &h);
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = w * 2;
	square.h = h * 2;
	SDL_RenderCopyEx(renderer, flappy2->getTexture(), nullptr, &square, 0.0, nullptr, SDL_FLIP_NONE);

	screenCoords = projectionMatrix * pepe->getPos();
	SDL_QueryTexture(pepe->getTexture(), nullptr, nullptr, &w, &h);
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = w / 3;
	square.h = h / 3;

	float angle = atan2(pepe->getVelocity().y, pepe->getVelocity().x) * 180 / PI;


	if (pepe->getVelocity().x >= 0.0f) {
		SDL_RenderCopyEx(renderer, pepe->getTexture(), nullptr, &square, -angle, nullptr, SDL_FLIP_NONE);
	}
	else {

		SDL_RenderCopyEx(renderer, pepe->getTexture(), nullptr, &square, 180 - angle, nullptr, SDL_FLIP_HORIZONTAL);
	}

	SDL_RenderPresent(renderer);
}