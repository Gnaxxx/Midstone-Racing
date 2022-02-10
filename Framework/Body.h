#ifndef BODY_H
#define BODY_H
#include "Vector.h"
#include <SDL_image.h>

using namespace MATH;

class Body {
private:
	Vec3 pos;
	Vec3 vel;
	Vec3 accel;
	float rotationZ;
	float mass;
	float rad;
	SDL_Surface *image;
	SDL_Texture* texture;
	bool is_moving;

public:
	Body();
	Body(Vec3 pos_, Vec3 vel_, Vec3 accel_, float mass_);
	~Body();
	void Update(float deltaTime);
	void ApplyForce(Vec3 force);
	Vec3 getPos() { return pos; }
	void isMoving(bool TorF) { is_moving = TorF; }
	void setImage(SDL_Surface* image_) { image = image_; }
	SDL_Surface *getImage() { return image; }
	SDL_Texture* getTexture() { return texture; }
	void setTexture(SDL_Texture *texture_) { texture = texture_; }
	Vec3 getVelocity() { return vel; }
	void setVelocity(Vec3 vel_) { vel = vel_; }
	float getMass() { return mass;  }
	float getRadius() { return rad; }
	float getRotationZ() { return rotationZ; }
};

#endif