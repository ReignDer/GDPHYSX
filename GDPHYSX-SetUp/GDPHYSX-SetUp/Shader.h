#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Shader
{
private:
	
	    //Shaders
	    GLuint vertexShader;
	    GLuint fragShader;
	    GLuint shaderProg;
	public:
	    //read vertex and frag shader file
		Shader(const char* v, const char* f);
	
	
	    //Compile vertex and frag shaders into one
		void compileShaders(const char* v, const char* f);
	
		GLuint getShaderProg();
	
};

