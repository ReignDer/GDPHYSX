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
    static void Key_Callback(GLFWwindow* window, // the pointer to the window
        int key, // the keycode being pressed
        int scancode, // Physical position of the press on keyboard
        int action, // Either Press / Release
        int mods);
	bool getSpace();
	void setSpace(bool pressed);
	bool getUp();
	void setUp(bool pressed);
	bool getDown();
	void setDown(bool pressed);
	bool getLeft();
	void setLeft(bool pressed);
	bool getRight();
	void setRight(bool pressed);
	bool getOrtho();
	void setOrtho(bool pressed);
	bool getPerspective();
	void setPerspective(bool pressed);

};

