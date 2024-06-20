#pragma once
#include "MyCamera.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Input.h"

class OrthoCamera : public MyCamera
{

	//Constructor
public:
	OrthoCamera(float window_height, float window_width);
	
public:
	//Create Orthographic Projection
	void createProjection();
	void setProjection(float left, float right, float bottom, float top);
	
private:
	//New camera position
	void updateCameraPos(GLFWwindow* window);
	//Update view withh lookAt function
	//Source::learnopengl.com/Getting-started/Camera
	void updateViewMatrix();
	
	//Update function that performs all updates
	void update(GLFWwindow* window);
	
public:
	//Perfrom camera
	void performCamera(GLuint shaderProg, GLFWwindow* window);
	
};

