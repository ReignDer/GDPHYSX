#include "OrthoCamera.h"

//Constructor
OrthoCamera::OrthoCamera(float window_height, float window_width) : MyCamera() {
	this->window_height = window_height;
	this->window_width = window_width;
}

//Create Orthographic Projection
void OrthoCamera::createProjection() {
	this->projectionMatrix = glm::ortho(-(this->window_width/2), //Left
		this->window_width / 2, //Right
		-(this->window_height/2), //Bottom
		this->window_height/2, //Top
		-0.1f, //Z-Near
		1000.f); //Z-Far
}
void OrthoCamera::setProjection(float left, float right, float bottom, float top) {
	this->projectionMatrix = glm::ortho(-left, //Left
		right, //Right
		-bottom, //Bottom
		top, //Top
		-0.1f, //Z-Near
		100.f); //Z-Far
}

//New camera position
void OrthoCamera::updateCameraPos(GLFWwindow* window) {

	float radius = 500.f;

	Input* input = static_cast<Input*>(glfwGetWindowUserPointer(window));

	//camera movement
	if (input->getUp()) {
		angleY += 0.001f;
		if (angleY > glm::radians(89.0f)) angleY = glm::radians(89.0f);
	}
	if (input->getDown()) {
		angleY -= 0.001f;
		if (angleY < glm::radians(-89.0f)) angleY = glm::radians(-89.0f);
	}
	if (input->getLeft()) {
		angleX += 0.001f;
	}
	if (input->getRight())
		angleX -= 0.001f;


	//Rotate Camera
	this->cameraPos.x = radius * std::sin(angleX) * std::cos(angleY);
	this->cameraPos.y = radius * std::sin(angleY);
	this->cameraPos.z = radius * std::cos(angleY) * std::cos(angleX);

	//Construct the Position Matrix
	this->cameraPositionMatrix =
		glm::translate(glm::mat4(1.0f), //Intialize it as an Identity Matrix
			this->cameraPos * -1.f); //Multiply to -1 since we need -P

	//Get Forward
	this->F = glm::normalize(this->Center - this->cameraPos);


}
//Update view withh lookAt function
//Source::learnopengl.com/Getting-started/Camera
void OrthoCamera::updateViewMatrix() {
	//Using the lookAt function for easy calculation of camera orientation and camera position
	this->viewMatrix = glm::lookAt(this->cameraPos, this->Center, this->U);
}

//Update function that performs all updates
void OrthoCamera::update(GLFWwindow* window) {
	this->updateCameraPos(window);
	this->updateViewMatrix();
}

//Perfrom camera
void OrthoCamera::performCamera(GLuint shaderProg, GLFWwindow* window) {
	this->update(window);
	this->render(shaderProg);
}

