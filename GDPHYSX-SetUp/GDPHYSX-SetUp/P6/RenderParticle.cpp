#include "RenderParticle.h"

void RenderParticle::Draw()
{
	//Render while Particle is not Destroyed
	if (!PhysicsParticle->IsDestroyed()) {
		RenderObject->setColor((glm::vec3)Color);
		RenderObject->updateTranslate((glm::vec3)PhysicsParticle->Position);
		RenderObject->updateScale((glm::vec3)Scale);
		RenderObject->perform();
	}

}
