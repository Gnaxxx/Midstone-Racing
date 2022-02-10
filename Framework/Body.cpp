#include "Body.h"

Body::Body(){
	pos.x = 0.0f;
	pos.y = 0.0f;
	pos.z = 0.0f;

	vel.x = 0.0f;
	vel.y = 0.0f;
	vel.z = 0.0f;

	accel.x = 0.0f;
	accel.y = 0.0f;
	accel.z = 0.0f;

	rotationZ = 0.0f;
	mass = 0.0f;
	isMoving(false);
}

Body::Body(Vec3 pos_, Vec3 vel_, Vec3 accel_, float mass_){
	pos = pos_;
	vel = vel_;
	accel = accel_;
	mass = mass_;
	rotationZ = 0.0f;
	rad = 0.75f;
}

Body::~Body(){

}

void Body::ApplyForce(Vec3 force) {
	accel = force / mass;

}

void Body::Update(float deltaTime){

	if (is_moving == false) {
		return;
	}

	Vec3 gravity(0.0f, -9.81f, 0.0f);
	pos += vel * deltaTime + 0.5f * accel * deltaTime * deltaTime; //+ 0.5f * gravity * deltaTime * deltaTime;

	/*pos.x += vel.x * deltaTime + 0.5f * accel.x * deltaTime * deltaTime;
	pos.y += vel.y * deltaTime + 0.5f * accel.y * deltaTime * deltaTime;
	pos.z += vel.z * deltaTime + 0.5f * accel.z * deltaTime * deltaTime;*/

	vel += accel * deltaTime; //+ gravity * deltaTime;

	/*vel.x += accel.x * deltaTime;
	vel.y += accel.y * deltaTime;
	vel.z += accel.z * deltaTime;*/

	//Rigid Body Rotation

	rotationZ += (180.0f/M_PI) * vel.x * deltaTime / rad;
	
	/*if (pos.y < 1.12f) {
		vel.y = -vel.y;
		//vel.y = 0.0f;
		//vel.x = 0.0f;
		

	}
	
	if (pos.x < 0.0f) {
		vel.x = -vel.x;

	}
	if (pos.x > 28.6f) {
		vel.x = -vel.x;

	}*/
	
}
