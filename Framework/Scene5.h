#ifndef SCENE5_H
#define SCENE5_H

#include "MMath.h"
#include "Scene.h"
#include <SDL.h>
#include <vector>
#include "Body.h"

using namespace MATH;

//#define NUM_BALLS 10

class Scene5 : public Scene {
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	Matrix4 projectionMatrix;
	Matrix4 invProjectionMatrix;
	//Body *array[NUM_BALLS];
	std::vector<Body*> balls;


public:
	Scene5(SDL_Window* sdlWindow);
	~Scene5();
	bool OnCreate();
	void OnDestroy();
	void Update(const float time);
	void Render();
	void HandleEvents(const SDL_Event& sdlEvent);
};

#endif