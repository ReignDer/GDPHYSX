#include "PhysicsWorld.h"

void P6::PhysicsWorld::GenerateContacts()
{
	Contacts.clear();

	this->GetOverlaps();

	for (std::list<ParticleLink*>::iterator i = Links.begin();
		i != Links.end();
		i++) {
		ParticleContact* contact = (*i)->GetContact();

		if (contact != nullptr) {
			Contacts.push_back(contact);
		}
	}
}

void P6::PhysicsWorld::GetOverlaps()
{
	for (int i = 0; i < Particles.size() - 1; i++) {
		std::list<P6Particles*>::iterator a = std::next(Particles.begin(), i);

		for (int h = i + 1; h < Particles.size(); h++) {
			std::list<P6Particles*>::iterator b = std::next(Particles.begin(), h);

			MyVector mag2Vec = (*a)->Position - (*b)->Position;

			float mag2 = mag2Vec.magnitudeSqrd();

			float rad = (*a)->radius + (*b)->radius;

			float rad2 = rad * rad;

			if (mag2 > rad2) continue;

			MyVector dir = mag2Vec.normalize();
			float r = rad2 - mag2;
			float depth = sqrt(r);

			float restitution = fmin(
				(*a)->restitution, (*b)->restitution
			);

			AddContact(*a, *b, restitution, dir, depth);
		}
	}
}

void P6::PhysicsWorld::AddContact(P6Particles* p1, P6Particles* p2, float restitution, MyVector contactNormal)
{
	ParticleContact* toAdd = new ParticleContact();

	toAdd->particles[0] = p1;
	toAdd->particles[1] = p2;
	toAdd->restitution = restitution;
	toAdd->contactNormal = contactNormal;

	Contacts.push_back(toAdd);
}

void P6::PhysicsWorld::AddContact(P6Particles* p1, P6Particles* p2, float restitution, MyVector contactNormal, float depth)
{
	ParticleContact* toAdd = new ParticleContact();

	toAdd->particles[0] = p1;
	toAdd->particles[1] = p2;
	toAdd->restitution = restitution;
	toAdd->contactNormal = contactNormal;
	toAdd->depth = depth;

	Contacts.push_back(toAdd);
}

void P6::PhysicsWorld::SetGravity(float gravity)
{
	this->Gravity = GravityForceGenerator(MyVector(0.f,gravity,0.f));
}

void P6::PhysicsWorld::AddParticle(P6Particles* toAdd)
{
	Particles.push_back(toAdd);

	if (toAdd->hasGravity) {
		//More forces
		forceRegistry.Add(toAdd, &Gravity);
	}
}

void P6::PhysicsWorld::Update(float time)
{
	this->UpdateParticleList();

	forceRegistry.UpdateForces(time);
	//Create an iterator that points at the start of the list
	for (std::list<P6Particles*>::iterator p = Particles.begin();
		//Continue looping until the end of the list
		p != Particles.end();
		//Move to the next particle
		p++)
	{
		//Call the particle update
		(*p)->Update(time);
	}

	//Call generate Contacts here
	GenerateContacts();

	if (Contacts.size() > 0) {
		contactResolver.ResolveContacts(Contacts, time);
	}
}

void P6::PhysicsWorld::UpdateParticleList()
{
	//Removes ALL Particles in the list that
	//Return true to the function below
	Particles.remove_if(
		[](P6Particles* p) {
			return p->IsDestroyed();
		}
	);
}
