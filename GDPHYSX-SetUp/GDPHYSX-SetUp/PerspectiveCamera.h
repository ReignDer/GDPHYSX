#pragma once
#include "MyCamera.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Input.h"

#define PI 3.14159

class PerspectiveCamera : public MyCamera
{
private:
    //Direction vector
    //glm::vec3 direction;
public:
    //Constructor
    PerspectiveCamera(float window_height, float window_width);

public:
    //Create Perspective Projection
    void createProjection();

    //Not used
    void updateZfarView();

private:
    //Update camera's position
    //Source::learnopengl.com/Getting-started/Camera
    void updateCameraPos(GLFWwindow* window);


    //Update view with lookAt function
    //Source::learnopengl.com/Getting-started/Camera
    void updateViewMatrix();

    //Update function that performs all updates
    void update(GLFWwindow* window);
public:
    //Perform Camera
    void performCamera(GLuint shaderProg, GLFWwindow* window);

};

