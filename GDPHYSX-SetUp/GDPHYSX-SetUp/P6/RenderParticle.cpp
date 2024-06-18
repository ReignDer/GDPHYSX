#include "RenderParticle.h"

void RenderParticle::Draw()
{
	if (!PhysicsParticle->IsDestroyed()) {
		RenderObject->setColor((glm::vec3)Color);
		RenderObject->updateTranslate((glm::vec3)PhysicsParticle->Position);
		RenderObject->updateScale((glm::vec3)Scale);
		RenderObject->perform();
	}

}
