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
#include "RenderLine.h"


#include "P6/MyVector.h"
#include "P6/P6Particles.h"
#include "P6/PhysicsWorld.h"
#include "P6/RenderParticle.h"
#include "P6/DragForceGenerator.h"
#include "P6/ParticleContact.h"
#include "P6/ContactResolver.h"
#include "P6/Links/ParticleLink.h"
#include "P6/Links/Rod.h"
#include "P6/Links/Cable.h"
#include "P6/Spring/AnchoredSpring.h"
#include "P6/Spring/ParticleSpring.h"
#include "P6/Spring/Bungee.h"



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
    window = glfwCreateWindow(800, 800, "Group4/ReignEngine", NULL, NULL);
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

    /*----------------Instantiate shaders and create the model---------------*/
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
    std::list<RenderLine*> RenderLines;

    //Requirements for pendelum
    int cableLength;
    float particleGap;
    float particleRadius;
    float gravityStrength;
    float xForce;
    float yForce;
    float zForce;

    std::cout << "Cable Length: ";
    std::cin >> cableLength;
    std::cout << "Particle Gap: ";
    std::cin >> particleGap;
    std::cout << "Particle Radius: ";
    std::cin >> particleRadius;
    std::cout << "Gravity Strength: ";
    std::cin >> gravityStrength;
    std::cout << "ApplyForce"<<'\n';
    std::cout << "x: ";
    std::cin >> xForce;
    std::cout << "y: ";
    std::cin >> yForce;
    std::cout << "z: ";
    std::cin >> zForce;

    //Set radius of Particle
    P6::MyVector radius = P6::MyVector(particleRadius, particleRadius, particleRadius);
    pWorld.SetGravity(gravityStrength); //Set Gravity


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


    //Create Anchor using Particle Link
    P6::P6Particles p1 = P6::P6Particles();
    p1.Position = P6::MyVector(2.f * (- particleGap - particleRadius), 100, 0);//Gap from Center and Adjacent particle
    p1.radius = particleRadius;
    p1.mass = 50.f;
    pWorld.AddParticle(&p1);

    //Anchor
    P6::P6Particles p2 = P6::P6Particles();
    p2.Position = P6::MyVector( 2.f*(- particleGap - particleRadius), 101, 0); //Gap from Center and Adjacent particle
    p2.radius = 0.f;
    p2.mass = 1;
    p2.hasGravity = false; // not affected by gravity
    p2.movable = false; //Immovable
    pWorld.AddParticle(&p2);

    //Particle
    P6::P6Particles p3 = P6::P6Particles();
    p3.Position = P6::MyVector(-particleGap - particleRadius, 100, 0);//Gap from Center and Adjacent particle
    p3.radius = particleRadius;
    p3.mass = 50.f;
    pWorld.AddParticle(&p3);

    //Anchor
    P6::P6Particles p4 = P6::P6Particles();
    p4.Position = P6::MyVector(-particleGap - particleRadius, 101, 0);//Gap from Center and Adjacent particle
    p4.radius = 0.f;
    p4.mass = 1;
    p4.hasGravity = false;// not affected by gravity
    p4.movable = false;//Immovable
    pWorld.AddParticle(&p4);

    //Particle
    P6::P6Particles p5 = P6::P6Particles();
    p5.Position = P6::MyVector(0, 100, 0); //Center
    p5.radius = particleRadius;
    p5.mass = 50.f;
    pWorld.AddParticle(&p5);

    //Anchor
    P6::P6Particles p6 = P6::P6Particles();
    p6.Position = P6::MyVector(0, 101, 0);
    p6.radius = 0.f;
    p6.mass = 1;
    p6.hasGravity = false;// not affected by gravity
    p6.movable = false;//Immovable
    pWorld.AddParticle(&p6);

    //Particle
    P6::P6Particles p7 = P6::P6Particles();
    p7.Position = P6::MyVector(particleGap + particleRadius, 100, 0); //Center
    p7.radius = particleRadius;
    p7.mass = 50.f;
    pWorld.AddParticle(&p7);

    //Anchor
    P6::P6Particles p8 = P6::P6Particles();
    p8.Position = P6::MyVector(particleGap + particleRadius, 101, 0); //Gap from Center and Adjacent particle
    p8.radius = 0.f;
    p8.mass = 1;
    p8.hasGravity = false;// not affected by gravity
    p8.movable = false;//Immovable
    pWorld.AddParticle(&p8);

    //Particle
    P6::P6Particles p9 = P6::P6Particles();
    p9.Position = P6::MyVector(2 * (particleGap + particleRadius), 100, 0); //Gap from Center and Adjacent particle
    p9.radius = particleRadius;
    p9.mass = 50.f;
    pWorld.AddParticle(&p9);

    //Anchor
    P6::P6Particles p10 = P6::P6Particles();
    p10.Position = P6::MyVector(2 * (particleGap + particleRadius), 101, 0); //Gap from Center and Adjacent particle
    p10.radius = 0.f;
    p10.mass = 1;
    p10.hasGravity = false;// not affected by gravity
    p10.movable = false;//Immovable
    pWorld.AddParticle(&p10);

    /*-----------Create Cable-----------*/
    P6::Cable* c1 = new P6::Cable();
    c1->particles[0] = &p2;
    c1->particles[1] = &p1;
    c1->length = cableLength;

    pWorld.Links.push_back(c1);

    P6::Cable* c2 = new P6::Cable();
    c2->particles[0] = &p4;
    c2->particles[1] = &p3;
    c2->length = cableLength;

    pWorld.Links.push_back(c2);

    P6::Cable* c3 = new P6::Cable();
    c3->particles[0] = &p6;
    c3->particles[1] = &p5;
    c3->length = cableLength;

    pWorld.Links.push_back(c3);

    P6::Cable* c4 = new P6::Cable();
    c4->particles[0] = &p8;
    c4->particles[1] = &p7;
    c4->length = cableLength;

    pWorld.Links.push_back(c4);

    P6::Cable* c5 = new P6::Cable();
    c5->particles[0] = &p10;
    c5->particles[1] = &p9;
    c5->length = cableLength;

    pWorld.Links.push_back(c5);

    /*-------------Render Lines-----------------*/

    RenderLine* line1 = new RenderLine(&p4, &p3);
    line1->setShaders(shader.getShaderProg());
    line1->setColor(glm::vec3(1,1,1));
    RenderLines.push_back(line1);

    RenderLine* line2 = new RenderLine(&p2, &p1);
    line2->setShaders(shader.getShaderProg());
    line2->setColor(glm::vec3(1, 1, 1));
    RenderLines.push_back(line2);

    RenderLine* line3 = new RenderLine(&p6, &p5);
    line3->setShaders(shader.getShaderProg());
    line3->setColor(glm::vec3(1, 1, 1));
    RenderLines.push_back(line3);

    RenderLine* line4 = new RenderLine(&p8, &p7);
    line4->setShaders(shader.getShaderProg());
    line4->setColor(glm::vec3(1, 1, 1));
    RenderLines.push_back(line4);

    RenderLine* line5 = new RenderLine(&p10, &p9);
    line5->setShaders(shader.getShaderProg());
    line5->setColor(glm::vec3(1, 1, 1));
    RenderLines.push_back(line5);


    /*-------------------Render Particle--------------------------------*/

    RenderParticle* rp1 = new RenderParticle(&p1, &object, P6::MyVector(0.4,0,0), radius);
    RenderParticles.push_back(rp1);
    RenderParticle* rp2 = new RenderParticle(&p3, &object, P6::MyVector(0, 0, 0.4), radius);
    RenderParticles.push_back(rp2);
    RenderParticle* rp3 = new RenderParticle(&p5, &object, P6::MyVector(0, 0.4, 0.4), radius);
    RenderParticles.push_back(rp3);
    RenderParticle* rp4 = new RenderParticle(&p7, &object, P6::MyVector(0.4, 0, 0), radius);
    RenderParticles.push_back(rp4);
    RenderParticle* rp5 = new RenderParticle(&p9, &object, P6::MyVector(0, 0, 0.4), radius);
    RenderParticles.push_back(rp5);



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
        
        //Press Space to move Left most Particle
        if (input.getSpace()) {
            input.setSpace(false);
            p1.AddForce(P6::MyVector(xForce, yForce, zForce));
        }

        //Rendering Particles
        for (std::list<RenderParticle*>::iterator i = RenderParticles.begin();
            i != RenderParticles.end();
            i++
            ) {
            (*i)->Draw();
        }

        //Rendering Lines
        for (std::list<RenderLine*>::iterator i = RenderLines.begin();
            i != RenderLines.end();
            i++
            ) {
            (*i)->draw();
        }
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
