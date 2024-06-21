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
    window = glfwCreateWindow(800, 800, "ReignEngine", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    float window_width = 800;
    float window_height = 800;

    /*-----------------Instantiate Camera---------------------*/

    //Orthographic
    MyCamera* cameraOrtho = new OrthoCamera(window_height, window_width);
    OrthoCamera* pCameraOrtho = (OrthoCamera*)cameraOrtho;

    //Perspective
    MyCamera* cameraPerspective = new PerspectiveCamera(window_height, window_width);
    PerspectiveCamera* pCameraPerspective = (PerspectiveCamera*)cameraPerspective;

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();

    /*-----------------Instantiate inputs--------------------*/
    Input input;

    //Refers window to input
    glfwSetWindowUserPointer(window, &input);
    //Keyboard
    glfwSetKeyCallback(window, Input::Key_Callback);

    /*----------------Instantiate shares and create the model---------------*/
    Shader shader(v, f);

    //Texture, OBJ
    Model3D object("3D/white.jpg", "3D/sphere.obj");
    object.setShaders(shader.getShaderProg());
    object.createModel();
    
    /*----------------Initialize Camera------------------*/
    cameraOrtho->createCamera();
    cameraPerspective->createCamera();

    /*----------------Initialize Physics and particle list----------------*/
    P6::PhysicsWorld pWorld = P6::PhysicsWorld();
    std::list<RenderParticle*> RenderParticles;

    //Amount of Particles
    int sparkAmount = 0;
    std::cout << "Enter Spark Amount: ";
    std::cin >> sparkAmount;

    //Spawning particles one by one
    const float spawnInterval = 100.f; //In milliseconds
    int maxParticles = sparkAmount;
    int particlesSpawned = 0;

    //Clock for spawn
    using clock = std::chrono::steady_clock;
    auto lastSpawnTime = clock::now();


    //Clock time
    using clock = std::chrono::high_resolution_clock;
    auto start_time = clock::now();


    auto curr_time = clock::now();
    auto prev_time = curr_time;

    std::chrono::nanoseconds curr_ns(0);
    
    //Better Random Distribution
    std::random_device rd;
    std::uniform_real_distribution<float> dist(1.0f,10.0f); //LifeSpan Range from 1-10
    std::uniform_real_distribution<float> distForce(50.f, 100.f); //Force Range from 50-100

    //Controlling the trajectory of a particle
    //X and Y angle from 70 to 110
    std::uniform_real_distribution<float> distAngle(80.f, 110.f);
    //Z angle, full 360
    std::uniform_real_distribution<float> distAngle_2(0.f, 360.f);


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        //Start Clock for spawning
        auto currentTime = clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastSpawnTime);

        //Start Clock for frame updates
        curr_time = clock::now();

        auto dur = std::chrono::duration_cast<std::chrono::nanoseconds>(curr_time - prev_time);
        prev_time = curr_time;

        curr_ns += dur;

        //Update per frame
        if (curr_ns >= timestep) {
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(curr_ns);
            //std::cout << "MS: " << (float)ms.count() << '\n';

            curr_ns -= curr_ns;

            //std::cout << "P6 Update\n";

            //Press Space to Pause Physics World
            if(!input.getSpace())   
                pWorld.Update((float)ms.count() / 1000);

            //std::cout << "Normal Update\n";
        }

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDepthMask(GL_FALSE);

        glDepthFunc(GL_LEQUAL);

        glUseProgram(shader.getShaderProg());

        //Toggle between Perspective and Orthographic
        //Orthographic is the default view
        //Press 1 for Orthographic and Press 2 for Perspective
        if (input.getPerspective()) {
            pCameraPerspective->performCamera(shader.getShaderProg(), window);

        }
        else {

           pCameraOrtho->performCamera(shader.getShaderProg(), window);
        }
        
        //Spawn particles one by one and pause spawning when space is pressed
        if (particlesSpawned < maxParticles && elapsedTime.count() >= spawnInterval && !input.getSpace()) {

            /*------------Create New Particle-----------------*/
            P6::P6Particles* newParticle = new P6::P6Particles();

            //Set Position to the bottom
            newParticle->Position = P6::MyVector(0, -380, 0);

            //Set mass and randomize lifespan from 1-10 second/s;
            newParticle->mass = 0.01f;
            newParticle->lifespan = dist(rd);

            //Randomizing angle of trajectory and converting degrees to radians
            float angle = distAngle(rd) * (PI / 180.f);
            float angle_2 = distAngle_2(rd) * (PI / 180.f);
            //Randomize velocity of particle
            float velocity = distForce(rd);

            //Add force to the sparks to create a fountain firework
            P6::MyVector randomForce = P6::MyVector(velocity * std::cosf(angle) * std::cosf(angle_2),
               velocity * std::sinf(angle), velocity * std::cosf(angle) * std::sinf(angle_2));
            newParticle->AddForce(randomForce);

            //Add particles to the Physics World 
            pWorld.AddParticle(newParticle);

            /*---------------Rendering particle----------------*/

            //Randomize Color and Radius 
            //Randomization of Color is between 0 to 1
            P6::MyVector randomColor = P6::MyVector((float)std::rand() / RAND_MAX,
                (float)std::rand() / RAND_MAX, (float)std::rand() / RAND_MAX);

            //Randomize radius from 2-10
            int randomRadius = 2 + (std::rand() % 10);
            P6::MyVector randomScale = P6::MyVector(randomRadius, randomRadius, randomRadius);

            //Instantiate particle
            RenderParticle* rp = new RenderParticle(newParticle, &object, randomColor, randomScale);

            //Add rendered particle in the list
            RenderParticles.push_back(rp);

            //Increment particles spawned to keep in track
            particlesSpawned++;

            //Set lastSpawnTime to currentTime for the next particle to spawn
            lastSpawnTime = currentTime;
        }

        //Rendering Particles
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
