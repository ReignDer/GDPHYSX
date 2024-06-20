#pragma once
#include <iostream>
#include <GLFW/glfw3.h>

class Input
{
private:
	bool space;
	bool up;
	bool down;
	bool left;
	bool right;
	bool ortho;
	bool perspective;
public:
	Input();
	//Process Inputs
    static void Key_Callback(GLFWwindow* window, // the pointer to the window
        int key, // the keycode being pressed
        int scancode, // Physical position of the press on keyboard
        int action, // Either Press / Release
        int mods);
	//Getters
	bool getSpace();
	bool getUp();
	bool getDown();
	bool getLeft();
	bool getRight();
	bool getOrtho();
	bool getPerspective();

	//Setters
	void setSpace(bool pressed);
	void setUp(bool pressed);
	void setDown(bool pressed);
	void setLeft(bool pressed);
	void setRight(bool pressed);
	void setOrtho(bool pressed);
	void setPerspective(bool pressed);

};

