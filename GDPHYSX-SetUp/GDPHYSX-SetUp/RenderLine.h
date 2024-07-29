#pragma once
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "P6/MyVector.h"
#include "P6/P6Particles.h"

class RenderLine
{
public:
	P6::P6Particles* p1;
	P6::P6Particles* p2;
	P6::MyVector anchorPoint;
private:
	GLuint shaderProg;
	GLuint VAO, VBO;
	GLuint colorAddress;
	
	glm::vec3 color;
	std::vector<GLfloat> vertices;
	glm::mat4 identity_matrix4 = glm::mat4(1.0f);

public:
	RenderLine() {}
	RenderLine(P6::P6Particles* _p1, P6::P6Particles* _p2) : p1(_p1), p2(_p2){}
	RenderLine(P6::MyVector _a, P6::P6Particles* _p2) : anchorPoint(_a), p2(_p2) {}

	~RenderLine();

public:
	void setUp(P6::MyVector p1, P6::MyVector p);

public:
	void draw();
	void setShaders(GLuint shaderProg);
	void setColor(glm::vec3(color));
};

