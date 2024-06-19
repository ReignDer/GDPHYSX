#include "PerspectiveCamera.h"

//Constructor

 PerspectiveCamera::PerspectiveCamera(float window_height, float window_width) : MyCamera() {
    this->window_height = window_height;
    this->window_width = window_width;
}

//Create Perspective Projection

void PerspectiveCamera::createProjection() {
    this->projectionMatrix = glm::perspective(
        glm::radians(90.f), //FOV
        this->window_height / this->window_width, //Aspect ratio
        0.1f, //ZNear > 0
        1000.f // ZFar
    );

}

void PerspectiveCamera::updateZfarView() {
    this->projectionMatrix = glm::perspective(
        glm::radians(90.f), //FOV
        this->window_height / this->window_width, //Aspect ratio
        0.1f, //ZNear > 0
        100.f // ZFar
    );
}

//Update camera's position
//Source::learnopengl.com/Getting-started/Camera

void PerspectiveCamera::updateCameraPos() {
    this->cameraPos = glm::vec3(0, 0.f, 500.f);
    /*float speed = 0.005f;*/

    ////camera movement
    //if (up)
    //    this->cameraPos += speed * this->F;
    //if (down)
    //    this->cameraPos -= speed * this->F;
    //if (left)
    //    this->cameraPos -= glm::normalize(glm::cross(this->F, this->U)) * speed;
    //if (right)
    //    this->cameraPos += glm::normalize(glm::cross(this->F, this->U)) * speed;
    //if (ascend)
    //    this->cameraPos += speed * this->U;
    //if (descend)
    //    this->cameraPos -= speed * this->U;
    //if (this->cameraPos.y > 5) {
    //    this->cameraPos.y = 5;
    //}
    this->cameraPositionMatrix =
        glm::translate(glm::mat4(1.0f), //Initialize it as an Identity Matrix
            this->cameraPos * -1.f); //Multiply to -1 since we need -P

}

//Update view with lookAt function
//Source::learnopengl.com/Getting-started/Camera

void PerspectiveCamera::updateViewMatrix() {
    //Using the lookAt function for easy calculation of camera orientation and camera position
    this->viewMatrix = glm::lookAt(this->cameraPos, this->cameraPos + this->F, this->U);
}

//Update function that performs all updates

void PerspectiveCamera::update() {
    this->updateCameraPos();
    //this->updateCameraOrientation();
    this->updateViewMatrix();
}

//Perform Camera

void PerspectiveCamera::performCamera(GLuint shaderProg) {
    this->update();
    this->render(shaderProg);
}
