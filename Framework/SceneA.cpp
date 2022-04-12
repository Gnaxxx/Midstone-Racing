#include "SceneA.h"
#include <SDL.h>
#include <SDL_image.h>
#include <math.h>
#include "Timer.h"
#include "Randomizer.h"
#include "VMath.h"
#include <iostream>
#include <windows.h>
#include <MMSystem.h>
using namespace std;

#include <iostream>
#include <iomanip>
#include <fstream>

#define PI 3.14159265

SceneA::SceneA(SDL_Window* sdlWindow_) {
	window = sdlWindow_;
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr) {
		printf("%s\n", SDL_GetError());
	}
	car = new Body(Vec3(12.75f, 5.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 2.0);
	car->setRadius(0.14f);

	enemy = new Body(Vec3(12.25f, 4.4f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 2.0);
	enemy->setRadius(0.14f);

	for (int i = 0; i < 6; ++i) {
		tracks.push_back(new Body(Vec3(12.0f, i*2.5f + 5.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 2.0));
	}
	
	obstacles.push_back(new Body(Vec3(12.2f, 6.5f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 2.0));
	obstacles.push_back(new Body(Vec3(13.0f, 10.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 0.0f, 0.0f), 2.0));

	for (int i = 0; i < obstacles.size(); ++i) {
		obstacles[i]->setRadius(0.14f);
	}

	

	Timer::SetSingleEvent(5000, (void*)"Start");

}

SceneA::~SceneA() {
	delete car;
	delete enemy;
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
	invProjectionMatrix = MMath::inverse(projectionMatrix);
	IMG_Init(IMG_INIT_PNG);

	SDL_Surface* carImage = IMG_Load("textures/yellowCar.png");
	SDL_Texture* carTexture = SDL_CreateTextureFromSurface(renderer, carImage);
	if (carImage == nullptr) {
		printf("Can't open textures/yellowCar.png\n");
		return false;
	}

	/*Vec3 upperLeft(0.0f, 0.0f, 0.0f); //Screen Coords
	Vec3 lowerRight(static_cast<float>(carImage->w), static_cast<float>(carImage->h), 0.0f);
	Vec3 ulWorld = invProjectionMatrix * upperLeft;
	Vec3 lrWorld = invProjectionMatrix * lowerRight;
	Vec3 worldCoordsFromScreenCoords = lrWorld - ulWorld;
	float carRad = worldCoordsFromScreenCoords.x / 2.0f;
	car->setRadius(carRad);*/
	car->setTexture(carTexture);

	SDL_Surface* enemyImage = IMG_Load("textures/flappybird.png");
	SDL_Texture* enemyTexture = SDL_CreateTextureFromSurface(renderer, enemyImage);
	if (enemyImage == nullptr) {
		printf("Can't open textures/flappybird.png\n");
		return false;
	}
	enemy->setTexture(enemyTexture);

	SDL_Surface* trackImage = IMG_Load("textures/track.png");
	SDL_Texture* trackTexture = SDL_CreateTextureFromSurface(renderer, trackImage);
	if (trackImage == nullptr) {
		printf("Can't open textures/track.png\n");
		return false;
	}

	for (int i = 0; i < tracks.size(); ++i) {
		tracks[i]->setTexture(trackTexture);
	}

	SDL_Surface* obstacleImage = IMG_Load("textures/PeepoPing.png");
	SDL_Texture* obstacleTexture = SDL_CreateTextureFromSurface(renderer, obstacleImage);
	if (obstacleImage == nullptr) {
		printf("Can't open textures/PeepoPing.png\n");
		return false;
		
	}

	for (int i = 0; i < obstacles.size(); ++i) {
		obstacles[i]->setTexture(obstacleTexture);
	}

	SDL_FreeSurface(carImage);
	SDL_FreeSurface(trackImage);
	SDL_FreeSurface(obstacleImage);

	return true;
}

void SceneA::OnDestroy() {}

void SceneA::Update(const float deltaTime) {

	std::cout << "(" << car->getVelocity().x << ", " << car->getVelocity().y <<  ")" << std::endl;

	for (int i = 0; i < tracks.size(); ++i) {
		tracks[i]->Update(deltaTime);
	}

	for (int i = 0; i < obstacles.size(); ++i) {
		obstacles[i]->Update(deltaTime);
	}

	for (int i = 0; i < tracks.size(); ++i) {

		Vec3 prevPos;

		if (car->getPos().x > tracks[i]->getPos().x - 2.0f && car->getPos().x < tracks[i]->getPos().x + 2.0f) {
			
			for (int i = 0; i < obstacles.size(); ++i) {
				float dist = VMath::distance(Vec3(car->getPos().x-0.2f, car->getPos().y, 0.0f), Vec3(obstacles[i]->getPos().x-0.2f, obstacles[i]->getPos().y, 0.0f));
				if (dist < car->getRadius() + obstacles[i]->getRadius()) {
					car->setTexture(nullptr);
					car->setPos(prevPos);
				}

			}

			int w, h;
			SDL_GetWindowSize(window, &w, &h);

			Matrix4 ndc = MMath::viewportNDC(w, h);
			Matrix4 ortho = MMath::orthographic(car->getPos().x - 2.0f, car->getPos().x + 3.0f, car->getPos().y - 2.0f, car->getPos().y + 1.0f, 0.0f, 1.0f);
			projectionMatrix = ndc * ortho;
			IMG_Init(IMG_INIT_PNG);

			prevPos = car->getPos();
		}

		else {

			/*SDL_Surface* carImage = IMG_Load("textures/flappybird.png");
			SDL_Texture* carTexture = SDL_CreateTextureFromSurface(renderer, carImage);
			if (carImage == nullptr) {
				printf("Can't open textures/flappybird.png\n");
				
			}*/
			car->setTexture(nullptr);

			//car->setPos(prevPos);

			//isDead = true;
			
		}

	}


	if (car->getIs_Moving() == false) {
		//car->ApplyForce(Vec3(0.0f, 0.0f, 0.0f));
		car->setVelocity(Vec3(0.0f, 0.0f, 0.0f));
		car->setAccel(Vec3(0.0f, 0.0f, 0.0f));

	}

	car->Update(deltaTime);

	if (gameStart == true) {
		//printf("yes");
		Vec3 enemyPrevPos;

		enemy->ApplyForce(Vec3(0.0f, 10.0f, 0.0f));

		for (int i = 0; i < obstacles.size(); ++i) {
			float enemyDist = VMath::distance(Vec3(enemy->getPos().x - 0.2f, enemy->getPos().y, 0.0f), Vec3(obstacles[i]->getPos().x - 0.2f, obstacles[i]->getPos().y, 0.0f));

			if (enemyDist < 1.35f) {

				
				enemy->ApplyForce(Vec3(0.0f, -10.0f, 0.0f)); //if there is a distance of 1.35 between enemy and obstacle, then slow down

				if (enemy->getVelocity().y <= 0.1f) {
					
					enemyPrevPos = enemy->getPos(); //we need to store position as it slows down
					enemy->ApplyForce(Vec3(10.0f, 0.0f, 0.0f)); //if it stops, push it 10 in the x

					if (enemy->getPos().x - enemyPrevPos.x == 0.2f) {
						enemy->ApplyForce(Vec3(-10.0f, 0.0f, 0.0f)); //if the enemy travels 0.2 then slow down
						enemy->ApplyForce(Vec3(0.0f, 10.0f, 0.0f));
						

					}
				}
			}
		}


	}

	enemy->Update(deltaTime);

}

void SceneA::HandleEvents(const SDL_Event& sdlEvent) {

	if (sdlEvent.type == SDL_EventType::SDL_KEYDOWN) {
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_W) {
			
			gameStart = true;
			car->ApplyForce(Vec3(0.0f, 10.0f, 0.0f));
			car->isMoving(true);

		}
	}
	else if (sdlEvent.type == SDL_EventType::SDL_KEYUP) {
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_W) {
			
			car->ApplyForce(Vec3(0.0f, -10.0f, 0.0f));
			//car->setVelocity(Vec3(0.0f, 0.0f, 0.0f));

			if (car->getVelocity().y <= 1.0f) {
				car->isMoving(false);
			}
			
		}
		
	}

	if (sdlEvent.type == SDL_EventType::SDL_KEYDOWN) {
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_A) {
			
			gameStart = true;
			car->ApplyForce(Vec3(-10.0f, 0.0f, 0.0f));
			car->isMoving(true);
			//printf("(%f, %f)", car->getPos().x, car->getPos().y);

		}
	}
	else if (sdlEvent.type == SDL_EventType::SDL_KEYUP) {
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_A) {
			
			car->ApplyForce(Vec3(10.0f, 0.0f, 0.0f));
			//car->setVelocity(Vec3(0.0f, 0.0f, 0.0f));

			if (car->getVelocity().x >= 1.0f) {
				car->isMoving(false);
			}

		}

	}

	if (sdlEvent.type == SDL_EventType::SDL_KEYDOWN) {
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_S) {
			
			gameStart = true;
			car->ApplyForce(Vec3(0.0f, -10.0f, 0.0f));
			car->isMoving(true);
			//printf("(%f, %f)", car->getPos().x, car->getPos().y);

		}
	}
	else if (sdlEvent.type == SDL_EventType::SDL_KEYUP) {
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_S) {
			car->ApplyForce(Vec3(0.0f, 10.0f, 0.0f));
			//car->setVelocity(Vec3(0.0f, 0.0f, 0.0f));

			if (car->getVelocity().y >= 1.0f) {
				car->isMoving(false);
			}

		}

	}

	if (sdlEvent.type == SDL_EventType::SDL_KEYDOWN) {
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_D) {
			
			gameStart = true;
			car->ApplyForce(Vec3(10.0f, 0.0f, 0.0f));
			car->isMoving(true);
			//printf("(%f, %f)", car->getPos().x, car->getPos().y);

		}
	}
	else if (sdlEvent.type == SDL_EventType::SDL_KEYUP) {
		if (sdlEvent.key.keysym.scancode == SDL_SCANCODE_D) {
			car->ApplyForce(Vec3(-10.0f, 0.0f, 0.0f));
			//car->setVelocity(Vec3(0.0f, 0.0f, 0.0f));

			if (car->getVelocity().x <= 1.0f) {
				car->isMoving(false);
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
		square.w = w * 4;
		square.h = h * 4;
		SDL_RenderCopyEx(renderer, tracks[i]->getTexture(), nullptr, &square, 0.0, nullptr, SDL_FLIP_NONE);
		
	}

	for (int i = 0; i < obstacles.size(); ++i) {
		screenCoords = projectionMatrix * obstacles[i]->getPos();
		SDL_QueryTexture(obstacles[i]->getTexture(), nullptr, nullptr, &w, &h);
		square.x = static_cast<int>(screenCoords.x);
		square.y = static_cast<int>(screenCoords.y);
		square.w = w;
		square.h = h;
		SDL_RenderCopyEx(renderer, obstacles[i]->getTexture(), nullptr, &square, 0.0, nullptr, SDL_FLIP_NONE);

	}

	screenCoords = projectionMatrix * enemy->getPos();
	SDL_QueryTexture(enemy->getTexture(), nullptr, nullptr, &w, &h);
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = w * 1.7;
	square.h = h * 1.7;

	float enemyAngle = atan2(enemy->getVelocity().y, enemy->getVelocity().x) * 180 / PI;


	if (enemy->getVelocity().x >= 0.0f) {
		SDL_RenderCopyEx(renderer, enemy->getTexture(), nullptr, &square, -enemyAngle, nullptr, SDL_FLIP_NONE);
		SDL_RenderPresent(renderer);
	}
	else {

		SDL_RenderCopyEx(renderer, car->getTexture(), nullptr, &square, 180 - enemyAngle, nullptr, SDL_FLIP_HORIZONTAL);
		SDL_RenderPresent(renderer);

	}


	screenCoords = projectionMatrix * car->getPos();
	SDL_QueryTexture(car->getTexture(), nullptr, nullptr, &w, &h);
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = w / 3;
	square.h = h / 3;
	//SDL_RenderCopyEx(renderer, car->getTexture(), nullptr, &square, 0.0, nullptr, SDL_FLIP_NONE);

	
	float carAngle = atan2(car->getVelocity().y, car->getVelocity().x) * 180 / PI;


	if (car->getVelocity().x >= 0.0f) {
		SDL_RenderCopyEx(renderer, car->getTexture(), nullptr, &square, -carAngle, nullptr, SDL_FLIP_NONE);
		SDL_RenderPresent(renderer);
	}
	else {

		SDL_RenderCopyEx(renderer, car->getTexture(), nullptr, &square, 180 - carAngle, nullptr, SDL_FLIP_HORIZONTAL);
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

//Music
/* int main()
{
	PlaySound(TEXT("Victory.wav"), NULL, SND_ASYNC);
	return 0;
} */