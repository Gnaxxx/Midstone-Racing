#ifndef SCENEA_H
#define SCENEA_H

#include "MMath.h"
#include "Scene.h"
#include <SDL.h>
#include "Body.h"

using namespace MATH;
class SceneA : public Scene {
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	Matrix4 projectionMatrix;
	Body* car;


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