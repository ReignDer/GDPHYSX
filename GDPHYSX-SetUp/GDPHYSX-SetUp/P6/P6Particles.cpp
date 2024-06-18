#include "P6Particles.h"

using namespace P6;

void P6::P6Particles::AddForce(MyVector force)
{
	this->accumulatedForce += force;
}

void P6::P6Particles::ResetForce()
{
	this->accumulatedForce = MyVector(0, 0, 0);
	this->Acceleration = MyVector(0, 0, 0);
}

void P6::P6Particles::Destroy()
{
	this->isDestroyed = true;
}

P6::P6Particles::P6Particles()
{
	this->Acceleration = MyVector();
	this->Velocity = MyVector();
	this->Position = MyVector();
	this->mass = 0;
	this->lifespan = 1.f;
	this->lifeRemaining = 0.f;
}

void P6::P6Particles::UpdatePosition(float time)
{
	this->Position = (this->Velocity * time) + ((this->Acceleration * time * time) * (1.f / 2.f)) + this->Position;
}

void P6::P6Particles::UpdateVelocity(float time)
{
	this->Acceleration += accumulatedForce * (1 / mass);
	this->Velocity = (this->Acceleration * time) + this->Velocity;
	this->Velocity = this->Velocity * powf(damping, time);
}

void P6::P6Particles::Update(float time)
{
	this->lifespan -= time;
	if (lifespan <= 0) {
		this->Destroy();
		return;
	}
	this->UpdatePosition(time);
	this->UpdateVelocity(time);
	//Reset the forces after calculations
	this->ResetForce();
}

