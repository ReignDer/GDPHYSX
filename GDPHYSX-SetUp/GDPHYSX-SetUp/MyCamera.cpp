#include "MyCamera.h"

MyCamera::MyCamera() {
	this->cameraPos = glm::vec3(0.f, 0.f, 3.f);
}

//Create the initial camera position
void MyCamera::createCameraPos() {

	//Construct the Position Matrix

	this->cameraPositionMatrix =
		glm::translate(glm::mat4(1.0f), //Initialize it as an Identity Matrix
			this->cameraPos * -1.f); //Multiply to -1 since we need -P

	//World's Up Direction
	//Normally just 1 in Y
	this->WorldUp = glm::vec3(0, 1.f, 0);

	//Camera's Center
	this->Center = glm::vec3(0.f, 0.f, 0.f);

	//Get Forward
	this->F = glm::vec3(this->Center - this->cameraPos);

	//Normalize the Forward
	this->F = glm::normalize(this->F);

	this->R = glm::normalize(
		//R x F
		glm::cross(this->F, WorldUp)
	);

	this->U = glm::normalize(
		//R x F
		glm::cross(this->R, this->F)
	);
}

//Create the initial camera orientation
void MyCamera::createCameraOrientation() {
	//Construct the Camera Orientation Matrix
	this->cameraOrientation = glm::mat4(1.f);

	//Manually assign the Matrix
	//Matrix[Column][Row]
	this->cameraOrientation[0][0] = R.x;
	this->cameraOrientation[1][0] = R.y;
	this->cameraOrientation[2][0] = R.z;

	this->cameraOrientation[0][1] = U.x;
	this->cameraOrientation[1][1] = U.y;
	this->cameraOrientation[2][1] = U.z;

	this->cameraOrientation[0][2] = -F.x;
	this->cameraOrientation[1][2] = -F.y;
	this->cameraOrientation[2][2] = -F.z;
}

//Create the initial camera view
void MyCamera::createCameraView() {
	//Camera View Matrix
	this->viewMatrix = cameraOrientation * cameraPositionMatrix;
}

//Create the Camera
void MyCamera::createCamera() {
	this->createProjection();
	this->createCameraPos();
	this->createCameraOrientation();
	this->createCameraView();
}

//Render the camera
void MyCamera::render(GLuint shaderProg) {

	unsigned int projectionLoc = glGetUniformLocation(shaderProg, "projection");

	glUniformMatrix4fv(projectionLoc,
		1,
		GL_FALSE,
		glm::value_ptr(this->projectionMatrix));

	unsigned int viewLoc = glGetUniformLocation(shaderProg, "view");

	glUniformMatrix4fv(viewLoc,
		1,
		GL_FALSE,
		glm::value_ptr(this->viewMatrix)); //View Matrix
}
//Virtual function for children to edit
void MyCamera::perform(GLuint shaderProg) {
	this->render(shaderProg);
}

//Getters
glm::vec3 MyCamera::getCameraPos() {
	return this->cameraPos;
}

glm::mat4 MyCamera::getViewMatrix() {
	return this->viewMatrix;
}

glm::mat4 MyCamera::getProjectionMatrix() {
	return this->projectionMatrix;
}
void MyCamera::setCameraPosition(glm::vec3 modelPos) {
	this->cameraPos = modelPos;
}
	


