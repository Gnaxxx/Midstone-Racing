#ifndef SCENE1_H
#define SCENE1_H

#include "MMath.h"
#include "Scene.h"
#include <SDL.h>
#include "Body.h"

using namespace MATH;
class Scene1 : public Scene {
private:
	SDL_Window *window;
	SDL_Renderer *renderer;
	Matrix4 projectionMatrix;
	Body *pepe;
	Body *cliff;

	
public:
	Scene1(SDL_Window* sdlWindow);
	~Scene1();
	bool OnCreate();
	void OnDestroy();
	void Update(const float time);
	void Render();
	void HandleEvents(const SDL_Event& sdlEvent);
};

#endif

