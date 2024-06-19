#include "Input.h"

Input::Input()
{
	this->space = false;
	this->up = false;
	this->down = false;
	this->left = false;
	this->right = false;
    this->ortho = false;
    this->perspective = false;
}

void Input::Key_Callback(GLFWwindow* window, // the pointer to the window
    int key, // the keycode being pressed
    int scancode, // Physical position of the press on keyboard
    int action, // Either Press / Release
    int mods) //Which modifier keys is held down
{
    Input* input = static_cast<Input*>(glfwGetWindowUserPointer(window));

    if (!input) {
        std::cout << "Faile";
    }
    //Flags for player movement
    //Down
    switch (action)
    {
    case GLFW_PRESS:
        switch (key) {
        case GLFW_KEY_S:
            input->setDown(true);
            break;
        case GLFW_KEY_W:
            input->setUp(true);
            break;
        case GLFW_KEY_A:
            input->setLeft(true);
            break;
        case GLFW_KEY_D:
            input->setRight(true);
            break;
        case GLFW_KEY_SPACE:
            input->setSpace(true);
            break;
        case GLFW_KEY_1:
            input->setPerspective(false);
            input->setOrtho(true);
            break;
        case GLFW_KEY_2:
            input->setOrtho(false);
            input->setPerspective(true);
            break;
        }
        break;
    case GLFW_RELEASE:
        switch(key){
        case GLFW_KEY_S:
            input->setDown(false);
            break;
        case GLFW_KEY_W:
            input->setUp(false);
            break;
        case GLFW_KEY_A:
            input->setLeft(false);
            break;
        case GLFW_KEY_D:
            input->setRight(false);
            break;
        case GLFW_KEY_SPACE:
            input->setSpace(false);
            break;
        }
        break;
    }
}

bool Input::getSpace()
{
	return this->space;
}

void Input::setSpace(bool pressed)
{
	this->space = pressed;
}

bool Input::getUp()
{
	return this->up;
}

void Input::setUp(bool pressed)
{
    this->up = pressed;
}


bool Input::getDown()
{
	return this->down;
}

void Input::setDown(bool pressed)
{
    this->down = pressed;
}


bool Input::getLeft()
{
	return this->left;
}

void Input::setLeft(bool pressed)
{
    this->left = pressed;
}


bool Input::getRight()
{
	return this->right;
}

void Input::setRight(bool pressed)
{
    this->right = pressed;
}

bool Input::getOrtho()
{
    return this->ortho;
}

void Input::setOrtho(bool pressed)
{
    this->ortho = pressed;
}

bool Input::getPerspective()
{
    return this->perspective;
}

void Input::setPerspective(bool pressed)
{
    this->perspective = pressed;
}

