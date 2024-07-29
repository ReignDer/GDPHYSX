#pragma once
#include <list>
#include "P6Particles.h"

#include "ForceRegistry.h"
#include "GravityForceGenerator.h"
#include "ParticleContact.h"
#include "ContactResolver.h"
#include "Links/ParticleLink.h"
#include <vector>

namespace P6 {
	class PhysicsWorld
	{
	protected:
		ContactResolver contactResolver = ContactResolver(20);
		void GenerateContacts();
		void GetOverlaps();
	public:
		std::list<ParticleLink*> Links;
		std::vector<ParticleContact*> Contacts;
		void AddContact(P6Particles* p1, P6Particles* p2, float restitution, MyVector contactNormal);
		void AddContact(P6Particles* p1, P6Particles* p2, float restitution, MyVector contactNormal, float depth);
		void SetGravity(float gravity);

		ForceRegistry forceRegistry;

		std::list<P6Particles*> Particles;

		void AddParticle(P6Particles* toAdd);

		void Update(float time);

	private:
		void UpdateParticleList();

		GravityForceGenerator Gravity = GravityForceGenerator(MyVector(0, -9.8, 0));
	};
}

