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
	Body* car;
	//Body* track;
	std::vector<Body*> tracks;


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