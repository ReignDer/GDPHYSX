#pragma once
#include "P6Particles.h"
#include "../Model3D.h"
#include "../Shader.h"
class RenderParticle
{
public:
	P6::P6Particles* PhysicsParticle;
	Model3D* RenderObject;
	P6::MyVector Color;
	P6::MyVector Scale;

	//Render Particle with its Model, Color, and Size
	RenderParticle(P6::P6Particles* p, Model3D* obj, P6::MyVector color, P6::MyVector scale) : PhysicsParticle(p), 
		RenderObject(obj), Color(color), Scale(scale) {}
	void Draw();

};

