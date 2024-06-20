#include "PerspectiveCamera.h"

//Constructor

 PerspectiveCamera::PerspectiveCamera(float window_height, float window_width) : MyCamera() {
    this->window_height = window_height;
    this->window_width = window_width;
    this->cameraPos = glm::vec3(0, 0.f, 500.f);

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

//Not used
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

void PerspectiveCamera::updateCameraPos(GLFWwindow* window) {
    float radius = 500.f;

    Input* input = static_cast<Input*>(glfwGetWindowUserPointer(window));
    
    //camera movement
    if (input->getUp()) {
        angleY += 0.001f;
        if (angleY > glm::radians(89.0f)) angleY = glm::radians(89.0f);
        //this->cameraPos += speed * this->F;
    }
    if (input->getDown()) {
        angleY -= 0.001f;
        if (angleY < glm::radians(-89.0f)) angleY = glm::radians(-89.0f);
    }
        //this->cameraPos -= speed * this->F;
    if (input->getLeft()) {
        angleX += 0.001f;
        //this->cameraPos -= glm::normalize(glm::cross(this->F, this->U)) * speed;
    }
    if (input->getRight())
        angleX -= 0.001f;
        //this->cameraPos += glm::normalize(glm::cross(this->F, this->U)) * speed;
    //if (ascend)
    //    this->cameraPos += speed * this->U;
    //if (descend)
    //    this->cameraPos -= speed * this->U;
    //if (this->cameraPos.y > 5) {
    //    this->cameraPos.y = 5;
    //}


    //Rotate Camera
    this->cameraPos.x = radius * std::sin(angleX) * std::cos(angleY);
    this->cameraPos.y = radius * std::sin(angleY);
    this->cameraPos.z = radius * std::cos(angleY) * std::cos(angleX);

    //Target
    this->F = glm::normalize(glm::vec3(0, 0, 0) - this->cameraPos);

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

void PerspectiveCamera::update(GLFWwindow* window) {
    this->updateCameraPos(window);
    //this->updateCameraOrientation();
    this->updateViewMatrix();
}

//Perform Camera

void PerspectiveCamera::performCamera(GLuint shaderProg, GLFWwindow* window) {
    this->update(window);
    this->render(shaderProg);
}
