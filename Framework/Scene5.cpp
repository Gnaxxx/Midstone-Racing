#include "Scene5.h"
#include <SDL.h>
#include <SDL_image.h>
#include <math.h>
#include "Timer.h"
#include "Randomizer.h"
#include "VMath.h"

#define PI 3.14159265

using namespace MATH;

Scene5::Scene5(SDL_Window* sdlWindow_) {
	window = sdlWindow_;
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr) {
		printf("%s\n", SDL_GetError());
	}

	//float yPos = 3.0f;
	Randomizer r;
	//int count = r.rand(1, 300);
	//for (int i = 0; i < 6; ++i) {
	balls.push_back(new Body(Vec3(5.0f, 1.1f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0, 0.0f), 1.0f));
	//balls.push_back(new Body(Vec3(5.0f, 7.5f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0, 0.0f), 1.0f));
	//balls.push_back(new Body(Vec3(9.0f, 7.5f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0, 0.0f), 1.0f));
	//balls.push_back(new Body(Vec3(13.0f, 7.5f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0, 0.0f), 1.0f));
	//balls.push_back(new Body(Vec3(16.0f, 7.5f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0, 0.0f), 1.0f));
	//balls.push_back(new Body(Vec3(20.0f, 7.5f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0, 0.0f), 1.0f));
	//balls.push_back(new Body(Vec3(r.rand(2.0f, 26.6f), r.rand(2.0f, 8.0f), 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0, 0.0f), 1.0f));
	//yPos += 1.0f;
//}
//Timer::SetSingleEvent(5000, (void*)"Start");

}

Scene5::~Scene5() {
	for (Body* body : balls) {
		delete body;
	}
}

bool Scene5::OnCreate() {
	int w, h;
	SDL_GetWindowSize(window, &w, &h);

	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(0.0f, 30.0f, 0.0f, 15.0f, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;
	invProjectionMatrix = MMath::inverse(projectionMatrix);
	IMG_Init(IMG_INIT_PNG);
	SDL_Surface* ballImage = IMG_Load("textures/guy_walk_spritesheet.png");
	SDL_Texture* ballTexture = SDL_CreateTextureFromSurface(renderer, ballImage);
	if (ballImage == nullptr) {
		printf("Can't open textures/PeepoPing.png\n");
		return false;
	}
	Vec3 upperLeft(0.0f, 0.0f, 0.0f); //Screen Coords
	Vec3 lowerRight(static_cast<float>(ballImage->w), static_cast<float>(ballImage->h), 0.0f);
	Vec3 ulWorld = invProjectionMatrix * upperLeft;
	Vec3 lrWorld = invProjectionMatrix * lowerRight;
	Vec3 worldCoordsFromScreenCoords = lrWorld - ulWorld;
	ulWorld.print();
	lrWorld.print();
	worldCoordsFromScreenCoords.print();
	float r = worldCoordsFromScreenCoords.x / 2.0f;
	printf("radius = %f\n", r);

	//for (Body* body : balls) {
	for (int i = 0; i < balls.size(); ++i) {
		balls[i]->setTexture(ballTexture);
	}

	SDL_FreeSurface(ballImage);

	return true;
}

void Scene5::OnDestroy() {}

void Scene5::Update(const float deltaTime) {
	for (int i = 0; i < balls.size(); ++i) {
		balls[i]->Update(deltaTime);
	}
	for (int i = 0; i < balls.size(); ++i) {
		if (balls[i]->getPos().y < balls[i]->getRadius()) {
			Vec3 vel = balls[i]->getVelocity();
			vel.y = -vel.y;
			balls[i]->setVelocity(vel);
		}
		if (balls[i]->getPos().x < balls[i]->getRadius()) {
			Vec3 vel = balls[i]->getVelocity();
			vel.x = -vel.x;
			balls[i]->setVelocity(vel);
		}
		if (balls[i]->getPos().x > 28.6f - balls[i]->getRadius()) {
			Vec3 vel = balls[i]->getVelocity();
			vel.x = -vel.x;
			balls[i]->setVelocity(vel);
		}
	}

	// Collision Detection
	for (int i = 0; i < balls.size() - 1; ++i) {
		for (int j = i + 1; j < balls.size(); ++j) {
			float dist = VMath::distance(balls[i]->getPos(), balls[j]->getPos());
			if (dist < balls[i]->getRadius() + balls[j]->getRadius()) {
				//printf("collision between %d and %d\n", i, j);	

				Vec3 v1 = balls[i]->getVelocity();
				Vec3 v2 = balls[j]->getVelocity();
				float e = 1.0f;
				float m1 = balls[i]->getMass();
				float m2 = balls[j]->getMass();

				Vec3 n = VMath::normalize(balls[i]->getPos() - balls[j]->getPos());
				float v1p = VMath::dot(v1, n);
				float v2p = VMath::dot(v2, n);
				if ((v1p - v2p) > 0.0f) {
					continue;
				}

				float v1prime = ((m1 - e * m2) * v1p + (1.0f + e) * m2 * v2p) / (m1 + m2);
				float v2prime = ((m2 - e * m1) * v2p + (1.0f + e) * m1 * v1p) / (m1 + m2);

				v1 += (v1prime - v1p) * n;
				v2 += (v2prime - v2p) * n;
				balls[i]->setVelocity(v1);
				balls[j]->setVelocity(v2);

			}
		}
	}
}

void Scene5::HandleEvents(const SDL_Event& sdlEvent) {

	Randomizer r;
	if (sdlEvent.type == SDL_EventType::SDL_KEYDOWN) {
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_D) {
			for (int i = 0; i < balls.size(); ++i) {
				balls[i]->ApplyForce(Vec3(30.0f, 0.0f, 0.0f));
				balls[i]->isMoving(true);
			}
		}
	}
	else if (sdlEvent.type == SDL_EventType::SDL_KEYUP) {
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_D) {
			for (int i = 0; i < balls.size(); ++i) {
				balls[i]->ApplyForce(Vec3(0.0f, 0.0f, 0.0f));
			}
		}
	}

	if (sdlEvent.type == SDL_EventType::SDL_KEYDOWN) {
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_A) {
			for (int i = 0; i < balls.size(); ++i) {
				balls[i]->ApplyForce(Vec3(-30, 0.0f, 0.0f));
				balls[i]->isMoving(true);
			}
		}
	}
	else if (sdlEvent.type == SDL_EventType::SDL_KEYUP) {
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_A) {
			for (int i = 0; i < balls.size(); ++i) {
				balls[i]->ApplyForce(Vec3(0.0f, 0.0f, 0.0f));
			}
		}
	}

	else if (sdlEvent.type == SDL_USEREVENT) {
		//printf("%d %s\n", sdlEvent.user.data1, sdlEvent.user.data2);

	}
	else if (sdlEvent.type == SDL_MOUSEBUTTONDOWN) {
		if (sdlEvent.button.button == SDL_BUTTON_LEFT) {
			printf("mouse is down! %d %d\n", sdlEvent.button.x, sdlEvent.button.y);
			Vec3 screenCoords(sdlEvent.button.x, sdlEvent.button.y, 0.0f);
			Vec3 worldCoords = invProjectionMatrix * screenCoords;
			printf("World Coords %f %f\n", worldCoords.x, worldCoords.y);
		}

	}
}

void Scene5::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 204, 255, 0);
	SDL_RenderClear(renderer);

	SDL_Rect square, boySquare;
	Vec3 screenCoords;
	int w, h;
	static int counter = 0;
	static int frameCounter = 0;
	frameCounter++;
	frameCounter %= 4;
	
	
	for (int i = 0; i < balls.size(); ++i) {
		screenCoords = projectionMatrix * balls[i]->getPos();
		SDL_QueryTexture(balls[i]->getTexture(), nullptr, nullptr, &w, &h);
		square.x = static_cast<int>(screenCoords.x) - (w / 2);
		square.y = static_cast<int>(screenCoords.y) - (h / 2);
		square.w = w / 8;
		square.h = h;
		boySquare.x = counter;

		if (frameCounter == 0) {
			counter += w / 8;
		}

		if (counter >= w) {
			counter = 0;
		}

		boySquare.y = 0;
		boySquare.w = w / 8;
		boySquare.h = h;
		SDL_RenderCopyEx(renderer, balls[i]->getTexture(), &boySquare, &square, 0.0f, nullptr, SDL_FLIP_NONE);
		/*float angle = atan2(balls[i]->getVelocity().y, balls[i]->getVelocity().x) * 180 / PI;

		if (balls[i]->getVelocity().x >= 0.0f) {
			SDL_RenderCopyEx(renderer, balls[i]->getTexture(), nullptr, &square, -angle, nullptr, SDL_FLIP_NONE);
		}
		else {
			SDL_RenderCopyEx(renderer, balls[i]->getTexture(), nullptr, &square, 180 - angle, nullptr, SDL_FLIP_HORIZONTAL);
		}*/

	}

	SDL_RenderPresent(renderer);
}
