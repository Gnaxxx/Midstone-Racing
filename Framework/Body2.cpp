#include "Body2.h"
#include <math.h>

Body2::Body2() {

	mass = 1.0f;
	rotationalInertia = 1.0f;

	pos.x = 0.0f;
	pos.y = 0.0f;
	pos.z = 0.0f;

	vel.x = 0.0f;
	vel.y = 0.0f;
	vel.z = 0.0f;

	acc.x = 0.0f;
	acc.y = 0.0f;
	acc.z = 0.0f;

	angle = 0.0f;
	angularVel = 0.0f;
	angularAcc = 0.0f;
}

Body2::Body2(Vec3 pos_, float mass_, float rotationalInertia_) {

	pos = pos_;
	mass = mass_;
	rotationalInertia = rotationalInertia_;
	angle = 0.0f;
	angularVel = 0.0f;
	angularAcc = 0.0f;

}

Body2::~Body2() {}

void Body2::ApplyTorque(float torque) {

	// tor = Ia || a = tor/I
	angularAcc = torque / rotationalInertia;

}

void Body2::ApplyForce(Vec3 force) {

	// f = ma || a = f/m
	force.x = (force.x) * cos(angle) + force.y * (-sin(angle));
	force.y = (force.x) * sin(angle) + force.y * cos(angle);

	acc = force / mass;

}

void Body2::Update(const float deltaTime) {

	// angle = (wi)(t) + (1/2)(angAcc)(t^2)
	angle += angularVel * deltaTime + 0.5f * angularAcc * pow(deltaTime, 2);

	angularVel += angularAcc * deltaTime; // update angular velocity

	// s = (vi)(t) + (1/2)(a)(t^2)
	pos += vel * deltaTime + 0.5f * acc * pow(deltaTime, 2);
	vel += acc * deltaTime; // update velocity in the x

	/*pos.y += vel.y * deltaTime + 0.5f * acc.y * pow(deltaTime, 2);
	vel.y += acc.y * deltaTime; // update velocity in the y

	pos.z += vel.z * deltaTime + 0.5f * acc.z * pow(deltaTime, 2);
	vel.z += acc.z * deltaTime; // update velocity in the z*/

	// I shoulda just done overload operators
}
