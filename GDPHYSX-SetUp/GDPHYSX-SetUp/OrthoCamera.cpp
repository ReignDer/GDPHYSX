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
		300.f); //Z-Far
}
void OrthoCamera::setProjection(float left, float right, float bottom, float top) {
	this->projectionMatrix = glm::ortho(-left, //Left
		right, //Right
		-bottom, //Bottom
		top, //Top
		-0.1f, //Z-Near
		100.f); //Z-Far
}

////New camera position
//void OrthoCamera::updateCameraPos() {
//	this->cameraPos = glm::vec3(0.f, 20.f, 0.f);
//
//
//
//	//Construct the Position Matrix
//	this->cameraPositionMatrix =
//		glm::translate(glm::mat4(1.0f), //Intialize it as an Identity Matrix
//			this->cameraPos * -1.f); //Multiply to -1 since we need -P
//
//	//World's Up Direction
//	this->WorldUp = glm::vec3(0.f, 1.f, 0.f);
//
//	//Camera's Center
//	this->Center = glm::vec3(0.f, 1.f, 0.f);
//
//	//Get Forward
//	this->F = glm::vec3(this->Center - this->cameraPos);
//
//	//Normalize the Forward
//	this->F = glm::normalize(this->F);
//
//	this->R = glm::normalize(
//		//R x F
//		glm::cross(this->F, WorldUp)
//	);
//
//	this->U = glm::normalize(
//		//R x F
//		glm::cross(this->R, this->F)
//	);
//}
////Update view withh lookAt function
////Source::learnopengl.com/Getting-started/Camera
//void OrthoCamera::updateViewMatrix() {
//	//Using the lookAt function for easy calculation of camera orientation and camera position
//	this->viewMatrix = glm::lookAt(this->cameraPos, this->cameraPos + this->F, this->U);
//}
//
////Update function that performs all updates
//void OrthoCamera::update() {
//	this->updateCameraPos();
//	this->updateViewMatrix();
//}

//Perfrom camera
void OrthoCamera::performCamera(GLuint shaderProg) {
	/*this->update();*/
	this->render(shaderProg);
}

