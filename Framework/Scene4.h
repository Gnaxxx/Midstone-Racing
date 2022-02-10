#ifndef SCENE4_H
#define SCENE4_H

#include "MMath.h"
#include "Scene.h"
#include <SDL.h>
#include <vector>
#include "Body.h"

using namespace MATH;

//#define NUM_BALLS 10

class Scene4 : public Scene {
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	Matrix4 projectionMatrix;
	Matrix4 invProjectionMatrix;
	//Body *array[NUM_BALLS];
	std::vector<Body*> balls;


public:
	Scene4(SDL_Window* sdlWindow);
	~Scene4();
	bool OnCreate();
	void OnDestroy();
	void Update(const float time);
	void Render();
	void HandleEvents(const SDL_Event& sdlEvent);
};

#endif