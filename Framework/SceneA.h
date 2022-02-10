#ifndef SCENEA_H
#define SCENEA_H

#include "MMath.h"
#include "Scene.h"
#include <SDL.h>
#include "Body2.h"

using namespace MATH;
class SceneA : public Scene {
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	Matrix4 projectionMatrix;
	Body2* ship;


public:
	SceneA();
	SceneA(SDL_Window* sdlWindow);
	~SceneA();
	bool OnCreate();
	void OnDestroy();
	void Update(const float time);
	void Render();
	void HandleEvents(const SDL_Event& sdlEvent);
};

#endif