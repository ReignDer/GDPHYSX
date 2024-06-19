// GDPHYSX-SetUp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <chrono>


#include <iostream>
#include <string>
#include <random>

#define TINYOBJLOADER_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include "Model3D.h"
#include "Shader.h"
#include "MyCamera.h"
#include "OrthoCamera.h"
#include "PerspectiveCamera.h"
#include "Input.h"


#include "P6/MyVector.h"
#include "P6/P6Particles.h"
#include "P6/PhysicsWorld.h"
#include "P6/RenderParticle.h"
#include "P6/DragForceGenerator.h"


#define PI 3.14159

using namespace std::chrono_literals;

constexpr std::chrono::nanoseconds timestep(16ms);

//void Key_Callback(GLFWwindow* window, // the pointer to the window
//    int key, // the keycode being pressed
//    int scancode, // Physical position of the press on keyboard
//    int action, // Either Press / Release
//    int mods) //Which modifier keys is held down
//{
//    //Flags for player movement
//    //Down
//    if (key == GLFW_KEY_S) {
//        if (action == GLFW_PRESS)
//            down = true;
//        else if (action == GLFW_RELEASE)
//            down = false;
//    }
//    //Up
//    if (key == GLFW_KEY_W) {
//        if (action == GLFW_PRESS)
//            up = true;
//        else if (action == GLFW_RELEASE)
//            up = false;
//    }
//
//    //Left
//    if (key == GLFW_KEY_A) {
//        if (action == GLFW_PRESS)
//            left = true;
//        else if (action == GLFW_RELEASE)
//            left = false;
//    }
//
//    //Right
//    if (key == GLFW_KEY_D) {
//        if (action == GLFW_PRESS)
//            right = true;
//        else if (action == GLFW_RELEASE)
//            right = false;
//    }
//
//    //Flags for spawning objects
//    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
//        spawn = true;
//    else if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
//        spawn = false;
//
//    if (key == GLFW_KEY_ESCAPE)
//        escape = true;
//
//}
////Call Mouse
////Source::learnopengl.com/Getting-started/Camera
//void mouse_callback(GLFWwindow* window, double xpos, double ypos)
//{
//    if (firstMouse)
//    {
//        lastX = xpos;
//        lastY = ypos;
//        firstMouse = false;
//    }
//
//    float xoffset = xpos - lastX;
//    float yoffset = lastY - ypos;
//    lastX = xpos;
//    lastY = ypos;
//
//    float sensitivity = 0.1f;
//    xoffset *= sensitivity;
//    yoffset *= sensitivity;
//
//    yaw += xoffset;
//    pitch += yoffset;
//
//    if (pitch > 89.0f)
//        pitch = 89.0f;
//    if (pitch < -89.0f)
//        pitch = -89.0f;
//}
int main(void)
{

    //Load the shader file into a string stream
    std::fstream vertSrc("Shaders/Sample.vert");
    std::stringstream vertBuff;

    //Add the file stream to the string stream
    vertBuff << vertSrc.rdbuf();

    //Convert the stream to a character array
    std::string vertS = vertBuff.str();
    const char* v = vertS.c_str();

    //Load the shader file into a string stream
    std::fstream fragSrc("Shaders/Sample.frag");
    std::stringstream fragBuff;

    //Add the file stream to the string stream
    fragBuff << fragSrc.rdbuf();

    //Convert the stream to a character array
    std::string fragS = fragBuff.str();
    const char* f = fragS.c_str();

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 800, "PC01 Mathieu Marc I. Pobre", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    float window_width = 800;
    float window_height = 800;

    //Instantiate Camera
    MyCamera* cameraOrtho = new OrthoCamera(window_height, window_width);
    OrthoCamera* pCameraOrtho = (OrthoCamera*)cameraOrtho;

    MyCamera* cameraPerspective = new PerspectiveCamera(window_height, window_width);
    PerspectiveCamera* pCameraPerspective = (PerspectiveCamera*)cameraPerspective;

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();

    Input input;
    glfwSetWindowUserPointer(window, &input);
    //Keyboard and Mouse inputs
    glfwSetKeyCallback(window, Input::Key_Callback);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //glfwSetCursorPosCallback(window, mouse_callback);

    Shader shader(v, f);

    Model3D object("3D/white.jpg", "3D/sphere.obj");
    object.setShaders(shader.getShaderProg());
    object.createModel();

    Model3D object2 = object;

    Model3D object3 = object;

    Model3D object4 = object;
    
    //Initialize Camera
    cameraOrtho->createCamera();
    cameraPerspective->createCamera();

    P6::PhysicsWorld pWorld = P6::PhysicsWorld();
    std::list<RenderParticle*> RenderParticles;

    //Amount of Particles
    int sparkAmount = 0;
    std::cout << "Enter Spark Amount: ";
    std::cin >> sparkAmount;

    using clock = std::chrono::high_resolution_clock;
    auto start_time = clock::now();


    auto curr_time = clock::now();
    auto prev_time = curr_time;

    std::chrono::nanoseconds curr_ns(0);
    
    //Better Random Distribution
    std::random_device rd;
    std::uniform_real_distribution<float> dist(1.0f,10.0f);
    std::uniform_real_distribution<float> distForce(10.f, 50.f);

    std::uniform_real_distribution<float> distAngle(70.f, 110.f);
    std::uniform_real_distribution<float> distAngle_2(0.f, 360.f);



    for (int i = 0; i < sparkAmount; i++) {

        /*------------Create New Particle-----------------*/
        P6::P6Particles* newParticle = new P6::P6Particles();

        //Set Position to the bottom
        newParticle->Position = P6::MyVector(0, -400, 0);

        //Set mass and lifespan from 1-10;
        newParticle->mass = 0.01f;
        newParticle->lifespan = dist(rd);
        newParticle->lifeRemaining = newParticle->lifespan;

        float angle = distAngle(rd) * (PI / 180.f);
        float angle_2 = distAngle_2(rd) * (PI / 180.f);
        float velocity = distForce(rd);
        //Add force to the sparks to create a fountain firework
        P6::MyVector randomForce = P6::MyVector(velocity * std::cosf(angle) * std::cosf(angle_2), velocity * std::sinf(angle), velocity * std::cosf(angle) * std::sinf(angle_2));
        newParticle->AddForce(randomForce);
        //Add particles to the Physics World 
        pWorld.AddParticle(newParticle);

        /*---------------Rendering particle----------------*/
        
        //Randomize Color and Radius 
        P6::MyVector randomColor = P6::MyVector((float)std::rand() / RAND_MAX,
            (float)std::rand() / RAND_MAX, (float)std::rand() / RAND_MAX);
        int randomRadius = 2 + (std::rand() % 10);
        P6::MyVector randomScale = P6::MyVector(randomRadius, randomRadius, randomRadius);
        RenderParticle* rp = new RenderParticle(newParticle, &object, randomColor,randomScale);
        RenderParticles.push_back(rp);
    }

    bool end = false;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window) && !end)
    {
        curr_time = clock::now();

        auto dur = std::chrono::duration_cast<std::chrono::nanoseconds>(curr_time - prev_time);
        prev_time = curr_time;

        curr_ns += dur;

        if (curr_ns >= timestep) {
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(curr_ns);
            //std::cout << "MS: " << (float)ms.count() << '\n';

            curr_ns -= curr_ns;

            //std::cout << "P6 Update\n";
            pWorld.Update((float)ms.count() / 1000);

            //std::cout << "Normal Update\n";
        }

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDepthMask(GL_FALSE);

        glDepthFunc(GL_LEQUAL);

        glUseProgram(shader.getShaderProg());


        if (input.getPerspective() == true) {
            pCameraPerspective->performCamera(shader.getShaderProg());
            //pCameraOrtho->performCamera(shader.getShaderProg());

        }
        else {
            //pCameraPerspective->performCamera(shader.getShaderProg());

           pCameraOrtho->performCamera(shader.getShaderProg());
        }
   
        for (std::list<RenderParticle*>::iterator i = RenderParticles.begin();
            i != RenderParticles.end();
            i++
            ) {
            (*i)->Draw();
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
