#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class MyCamera
{
	//Camera Fields
	protected:
	    //Matrix for the projection
	    glm::mat4 projectionMatrix;
	
	    //The camera's poistion
	    glm::vec3 cameraPos;
	    glm::mat4 cameraPositionMatrix;
	
	    //The camera's eye
	    glm::vec3 WorldUp;
	    glm::vec3 Center;
	    glm::vec3 F;
	    glm::vec3 R;
	    glm::vec3 U;
	
	    //The camera's orientation
	    glm::mat4 cameraOrientation;
	
	    //The View of the Camera
	    glm::mat4 viewMatrix;
	
	    //Window height and width
	    float window_height;
	    float window_width;

		//Perspective
		float angleX = 0.f;
		float angleY = 0.f;
	
	    //Default Constructor
	public:
		MyCamera();
	
	
	public:
	    //Pure virtual function to implement projection type
	    virtual void createProjection() = 0;
	private:
	    //Create the initial camera position
		void createCameraPos();
	
	    //Create the initial camera orientation
		void createCameraOrientation();
	
	    //Create the initial camera view
		void createCameraView();
	public:
	    //Create the Camera
		void createCamera();
	public:
	    //Render the camera
		void render(GLuint shaderProg);
	    //Virtual function for children to edit
		virtual void perform(GLuint shaderProg);
	
	    //Getters
		glm::vec3 getCameraPos();
	
		glm::mat4 getViewMatrix();
		glm::mat4 getProjectionMatrix();
		void setCameraPosition(glm::vec3 modelPos);
	};

