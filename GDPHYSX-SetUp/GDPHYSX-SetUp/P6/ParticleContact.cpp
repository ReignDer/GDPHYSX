#include "ParticleContact.h"

void P6::ParticleContact::Resolve(float time)
{
	ResolveVelocity(time);
	ResolveInterpenetration(time);
}

float P6::ParticleContact::GetSeparatingSpeed()
{
	MyVector velocity = particles[0]->Velocity;
	if (particles[1]) velocity -= particles[1]->Velocity;
	return velocity.dotProduct(contactNormal);
}

void P6::ParticleContact::ResolveInterpenetration(float time)
{
	//Not overlapping
	if (depth <= 0) return;

	float totalMass = (float)1 / particles[0]->mass;

	if (particles[1]) {
		totalMass += (float)1 / particles[1]->mass;
	}

	if (totalMass <= 0) return;

	float totalMoveByMass = depth / totalMass;
	MyVector moveByMass = contactNormal * totalMoveByMass;

	if (particles[0]->movable) {
		MyVector P_a = moveByMass * ((float)1 / particles[0]->mass);
		particles[0]->Position += P_a;
	}

	if (particles[1] && particles[1]->movable) {
		MyVector P_b = moveByMass * (-(float)1 / particles[1]->mass);
		particles[1]->Position += P_b;
	}
	depth = 0;
}

void P6::ParticleContact::ResolveVelocity(float time)
{
	float separatingSpeed = GetSeparatingSpeed();

	if (separatingSpeed > 0)
		return;

	float newSS = -restitution * separatingSpeed;
	float deltaSpeed = newSS - separatingSpeed;

	float totalMass = (float)1 / particles[0]->mass;
	if (particles[1]) totalMass += (float)1 / particles[1]->mass;
	
	if (totalMass <= 0) return;

	float impulse_mag = deltaSpeed / totalMass;
	MyVector Impulse = contactNormal * impulse_mag;

	if (particles[0]->movable) {
		MyVector V_a = Impulse * ((float)1 / particles[0]->mass);
		particles[0]->Velocity = particles[0]->Velocity + V_a;
	}

	if (particles[1] && particles[1]->movable) {
		MyVector V_b = Impulse * ((float)-1 / particles[1]->mass);
		particles[1]->Velocity = particles[1]->Velocity + V_b;
	}
}
