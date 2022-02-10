#ifndef BODY2_H
#define BODY2_H
#include "Vector.h"
#include <SDL_image.h>

using namespace MATH;

class Body2 {
private:
	float mass, rotationalInertia;
	Vec3 pos, vel, acc;
	float angle, angularVel, angularAcc;

	SDL_Surface* image;
	SDL_Texture* texture;

public:
	Body2();
	Body2(Vec3 pos_, float mass_, float rotationalInertia_);
	~Body2();

	void ApplyTorque(float torque);
	void ApplyForce(Vec3 force);
	void Update(const float deltaTime);

	SDL_Surface* getImage() { return image; }
	void setImage(SDL_Surface* image_) { image = image_; }
	SDL_Texture* getTexture() { return texture; }
	void setTexture(SDL_Texture* texture_) { texture = texture_; }

	float GetMass() { return mass; }
	float GetRotationalInertia() { return rotationalInertia; }
	Vec3 GetPos() { return pos; }
	Vec3 GetVel() { return vel; }
	Vec3 GetAcc() { return acc; }
	float GetAngle() { return angle; }
	float GetAngularVel() { return angularVel; }
	float GetAngularAcc() { return angularAcc; }
	


};


#endif