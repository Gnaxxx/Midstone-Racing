#ifndef YOUWIN_H
#define YOUWIN_H

#include "MMath.h"
#include "Scene.h"
#include <SDL.h>
#include <vector>
#include "Body.h"

using namespace MATH;
class YouWin : public Scene {
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	Matrix4 projectionMatrix;
	Matrix4 invProjectionMatrix;
	Body* youWin;

public:
	YouWin(SDL_Window* sdlWindow);
	~YouWin();
	bool OnCreate();
	void OnDestroy();
	void Update(const float time);
	void Render();
	void HandleEvents(const SDL_Event& sdlEvent);

};


#endif
