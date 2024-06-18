#include "Shader.h"

//read vertex and frag shader file
Shader::Shader(const char* v, const char* f) {
	this->compileShaders(v, f);
}


//Compile vertex and frag shaders into one
void Shader::compileShaders(const char* v, const char* f) {
	//Create a Vertex Shader
	this->vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//Assign the source to the Vertex Shader
	glShaderSource(this->vertexShader, 1, &v, NULL);

	//Compile the Vertex Shader
	glCompileShader(this->vertexShader);

	//Create a Fragment Shader
	this->fragShader = glCreateShader(GL_FRAGMENT_SHADER);

	//Assign the source to the Fragment Shader
	glShaderSource(this->fragShader, 1, &f, NULL);

	//Compile the Fragment Shader
	glCompileShader(this->fragShader);

	//Create the Shader Program
	this->shaderProg = glCreateProgram();
	//Attach the compiled Vertex Shader
	glAttachShader(this->shaderProg, this->vertexShader);
	//Attach the compiled Fragment Shader
	glAttachShader(this->shaderProg, this->fragShader);

	//Finalize the compilation process
	glLinkProgram(this->shaderProg);

	glDeleteShader(this->vertexShader);
	glDeleteShader(this->fragShader);
}

GLuint Shader::getShaderProg() {
	return this->shaderProg;
}
