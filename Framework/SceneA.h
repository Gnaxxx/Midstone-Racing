#ifndef SCENEA_H
#define SCENEA_H

#include "MMath.h"
#include "Scene.h"
#include <SDL.h>
#include <vector>
#include "Body.h"

using namespace MATH;
class SceneA : public Scene {
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	Matrix4 projectionMatrix;
	Matrix4 invProjectionMatrix;
	Body* car;
	Body* enemy;
	//Body* track;
	std::vector<Body*> tracks;
	std::vector<Body*> obstacles;
	//bool isDead = false;

	bool gameStart = false;
	bool isForward;
	bool isLeft;
	bool isBackward;
	bool isRight;


public:

	SceneA(SDL_Window* sdlWindow);
	~SceneA();
	bool OnCreate();
	void OnDestroy();
	void Update(const float time);
	void Render();
	void HandleEvents(const SDL_Event& sdlEvent);
};

#endif