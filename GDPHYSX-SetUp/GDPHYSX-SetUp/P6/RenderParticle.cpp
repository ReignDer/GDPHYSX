#include "RenderParticle.h"

void RenderParticle::Draw()
{
	if (!PhysicsParticle->IsDestroyed()) {
		RenderObject->setColor((glm::vec3)Color);
		RenderObject->updateTranslate((glm::vec3)PhysicsParticle->Position);
		RenderObject->updateScale(5.f, 5.f, 5.f);
		RenderObject->perform();
	}
}
