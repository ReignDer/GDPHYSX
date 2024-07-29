#include "ParticleLink.h"

float P6::ParticleLink::CurrentLength()
{
    return this->particles[0]->Position.distance(this->particles[1]->Position);
}


