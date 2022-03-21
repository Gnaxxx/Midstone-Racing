#include "SceneA.h"
#include <SDL.h>
#include <SDL_image.h>
#include <math.h>
#include "Timer.h"

#define PI 3.14159265

SceneA::SceneA(SDL_Window* sdlWindow_) {
	window = sdlWindow_;
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr) {
		printf("%s\n", SDL_GetError());
	}
	car = new Body(Vec3(12.0f, 5.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 2.0);
	for (int i = 0; i < 8; ++i) {
		tracks.push_back(new Body(Vec3(12.0f, i*1.5f + 2.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 2.0));
	}
	

	Timer::SetSingleEvent(5000, (void*)"Start");

}

SceneA::~SceneA() {
	delete car;
	for (Body* body : tracks) {
		delete body;
	}

}

bool SceneA::OnCreate() {
	int w, h;
	SDL_GetWindowSize(window, &w, &h);

	Matrix4 ndc = MMath::viewportNDC(w, h);
	printf("%d | %d", w, h);
	Matrix4 ortho = MMath::orthographic(0.0f, 30.0f, 0.0f, 7.5f, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;
	IMG_Init(IMG_INIT_PNG);

	SDL_Surface* carImage = IMG_Load("textures/yellowCar.png");
	SDL_Texture* carTexture = SDL_CreateTextureFromSurface(renderer, carImage);
	if (carImage == nullptr) {
		printf("Can't open textures/yellowCar.png\n");
		return false;
	}
	car->setTexture(carTexture);

	SDL_Surface* trackImage = IMG_Load("textures/track.png");
	SDL_Texture* trackTexture = SDL_CreateTextureFromSurface(renderer, trackImage);
	if (trackImage == nullptr) {
		printf("Can't open textures/track.png\n");
		return false;
	}

	for (int i = 0; i < tracks.size(); ++i) {
		tracks[i]->setTexture(trackTexture);
	}

	SDL_FreeSurface(carImage);
	SDL_FreeSurface(trackImage);

	return true;
}

void SceneA::OnDestroy() {}

void SceneA::Update(const float deltaTime) {

	int w, h;
	SDL_GetWindowSize(window, &w, &h);

	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(0.0f, car->getPos().x*2.5f, 0.0f, car->getPos().y * 1.5f, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;
	IMG_Init(IMG_INIT_PNG);

	car->Update(deltaTime);
	
	

}

void SceneA::HandleEvents(const SDL_Event& sdlEvent) {

	if (sdlEvent.type == SDL_EventType::SDL_KEYDOWN) {
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_W) {
			car->ApplyForce(Vec3(0.0f, 10.0f, 0.0f));
			car->isMoving(true);
			printf("(%f, %f)", car->getPos().x, car->getPos().y);

		}
	}
	else if (sdlEvent.type == SDL_EventType::SDL_KEYUP) {
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_W) {
			car->ApplyForce(Vec3(0.0f, -10.0f, 0.0f));
			car->setVelocity(Vec3(0.0f, 0.0f, 0.0f));
			car->isMoving(false);


			if (car->getIs_Moving() == false) {
				car->ApplyForce(Vec3(0.0f, 0.0f, 0.0f));
				
			}
			
		}
		
	}

	if (sdlEvent.type == SDL_EventType::SDL_KEYDOWN) {
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_A) {
			car->ApplyForce(Vec3(-10.0f, 0.0f, 0.0f));
			car->isMoving(true);
			printf("(%f, %f)", car->getPos().x, car->getPos().y);

		}
	}
	else if (sdlEvent.type == SDL_EventType::SDL_KEYUP) {
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_A) {
			car->ApplyForce(Vec3(10.0f, 0.0f, 0.0f));
			car->setVelocity(Vec3(0.0f, 0.0f, 0.0f));
			car->isMoving(false);


			if (car->getIs_Moving() == false) {
				car->ApplyForce(Vec3(0.0f, 0.0f, 0.0f));

			}

		}

	}

	if (sdlEvent.type == SDL_EventType::SDL_KEYDOWN) {
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_S) {
			car->ApplyForce(Vec3(0.0f, -10.0f, 0.0f));
			car->isMoving(true);
			printf("(%f, %f)", car->getPos().x, car->getPos().y);

		}
	}
	else if (sdlEvent.type == SDL_EventType::SDL_KEYUP) {
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_S) {
			car->ApplyForce(Vec3(0.0f, 10.0f, 0.0f));
			car->setVelocity(Vec3(0.0f, 0.0f, 0.0f));
			car->isMoving(false);


			if (car->getIs_Moving() == false) {
				car->ApplyForce(Vec3(0.0f, 0.0f, 0.0f));

			}

		}

	}

	if (sdlEvent.type == SDL_EventType::SDL_KEYDOWN) {
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_D) {
			car->ApplyForce(Vec3(10.0f, 0.0f, 0.0f));
			car->isMoving(true);
			printf("(%f, %f)", car->getPos().x, car->getPos().y);

		}
	}
	else if (sdlEvent.type == SDL_EventType::SDL_KEYUP) {
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_D) {
			car->ApplyForce(Vec3(-10.0f, 0.0f, 0.0f));
			car->setVelocity(Vec3(0.0f, 0.0f, 0.0f));
			car->isMoving(false);


			if (car->getIs_Moving() == false) {
				car->ApplyForce(Vec3(0.0f, 0.0f, 0.0f));

			}

		}

	}

}

void SceneA::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 204, 255, 0);
	SDL_RenderClear(renderer);

	SDL_Rect square;
	Vec3 screenCoords;
	int w, h;

	for (int i = 0; i < tracks.size(); ++i) {
		screenCoords = projectionMatrix * tracks[i]->getPos();
		SDL_QueryTexture(tracks[i]->getTexture(), nullptr, nullptr, &w, &h);
		square.x = static_cast<int>(screenCoords.x);
		square.y = static_cast<int>(screenCoords.y);
		square.w = w;
		square.h = h;
		SDL_RenderCopyEx(renderer, tracks[i]->getTexture(), nullptr, &square, 0.0, nullptr, SDL_FLIP_NONE);
	}


	screenCoords = projectionMatrix * car->getPos();
	SDL_QueryTexture(car->getTexture(), nullptr, nullptr, &w, &h);
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = w / 5;
	square.h = h / 5;
	//SDL_RenderCopyEx(renderer, car->getTexture(), nullptr, &square, 0.0, nullptr, SDL_FLIP_NONE);

	
	float angle = atan2(car->getVelocity().y, car->getVelocity().x) * 180 / PI;


	if (car->getVelocity().x >= 0.0f) {
		SDL_RenderCopyEx(renderer, car->getTexture(), nullptr, &square, -angle, nullptr, SDL_FLIP_NONE);
		SDL_RenderPresent(renderer);
	}
	else {

		SDL_RenderCopyEx(renderer, car->getTexture(), nullptr, &square, 180 - angle, nullptr, SDL_FLIP_HORIZONTAL);
		SDL_RenderPresent(renderer);
		
	}

	//SDL_RenderPresent(renderer);
}

/*
//player cannot run off screen (Screen Collision)

int main()
{
	//left side  
	if (yellowCar.getPosition().x < 0.0f)
		yellowCar.setPosition(0.0f, player.getPosition().y);

	//right side 
	if (yellowCar.getPosition().x + player.getGlobalBounds().width > WINDOW_WIDTH)
		yellowCar.setPosition(WINDOW_WIDTH - yellowCar.getGlobalBounds().width, yellowCar.getPosition().y);

	//top side 
	if(yellowCar.getPosition().y < 0.0f))
	yellowCar.setPosition(player.getPosition().x, 0.0f);
	
	//bottom side 
	if (yellowCar.getPosition().y + player.getGlobalBounds().height > WINDOW_HEIGHT)
		yellowCar.setPosition(yellowCar.getPosition().x, WNDOW_HEIGHT - yellowCar.getGlobalBounds().height); 
}  
player.GetGlobalBounds.width is something to do with the screen minus the width of the player (don't remember the exact words)
*/ 
//hello world!!!


/* //Menu.....ish

using namespace std; 

int menu; //stores user's number choice

int main() 
{
	cout << "Welcome TO The Game!" << endl;
	cout << "1-Play" << endl;
	cout << "2-Options" << endl;
	cout << "3-Quit" << endl;
	cin >> menu; 

	if (menu == 1)
	{
		cout << "I do not actually know how to make it switch scenes" << endl; 
	}

	if (menu == 2)
	{
		cout << "I do not actually know how to make it switch scenes, too" << endl;
	}

	if (menu == 3)
	{
		return 0; 
	}

	else
	{
		cout << "this is a bad basic menu, try again" << endl;
		return 0;
	}
	return 0;
} */ 
