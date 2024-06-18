#pragma once
#include "MyCamera.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class PerspectiveCamera : public MyCamera
{
private:
    //Direction vector
    //glm::vec3 direction;
public:
    //Constructor
    PerspectiveCamera(float window_height, float window_width);

public:
    //Create Perpective Projection
    void createProjection();

    void updateZfarView();
private:
    //Update camera's position
    //Source::learnopengl.com/Getting-started/Camera
    void updateCameraPos();

    ////Update camera's orientation
    ////Source::learnopengl.com/Getting-started/Camera
    //void updateCameraOrientation() {

    //    //Orientation of x,y,z
    //    this->direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    //    this->direction.y = sin(glm::radians(pitch));
    //    this->direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    //    //Normalize F
    //    this->F = glm::normalize(this->direction);


    //}

    //Update view withh lookAt function
    //Source::learnopengl.com/Getting-started/Camera
    void updateViewMatrix();

    //Update function that performs all updates
    void update();
public:
    //Perform Camera
    void performCamera(GLuint shaderProg);

};

