// GDPHYSX-SetUp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <chrono>


#include <iostream>
#include <string>

#define TINYOBJLOADER_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include "Model3D.h"
#include "Shader.h"
#include "MyCamera.h"
#include "OrthoCamera.h"
#include "PerspectiveCamera.h"


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
    window = glfwCreateWindow(800, 800, "PC01 Mathieu Marc I. Pobre", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    float window_width = 800;
    float window_height = 800;

    std::srand((unsigned)time(NULL));

    MyCamera* cameraOrtho = new OrthoCamera(window_height, window_width);
    OrthoCamera* pCameraOrtho = (OrthoCamera*)cameraOrtho;

    MyCamera* cameraPerspective = new PerspectiveCamera(window_height, window_width);
    PerspectiveCamera* pCameraPespective = (PerspectiveCamera*)cameraPerspective;

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();


    Shader shader(v, f);

    Model3D object("3D/white.jpg", "3D/sphere.obj");
    object.setShaders(shader.getShaderProg());
    object.createModel();

    Model3D object2 = object;

    Model3D object3 = object;

    Model3D object4 = object;
    

    cameraOrtho->createCamera();
    cameraPerspective->createCamera();

    P6::PhysicsWorld pWorld = P6::PhysicsWorld();
    //P6::P6Particles particle = P6::P6Particles();
    //P6::P6Particles particle2 = P6::P6Particles();
    //P6::P6Particles particle3 = P6::P6Particles();
    //P6::P6Particles particle4 = P6::P6Particles();
    std::list<RenderParticle*> RenderParticles;

    int sparkAmount = 0;
    std::cout << "Enter Spark Amount: ";
    std::cin >> sparkAmount;

    using clock = std::chrono::high_resolution_clock;
    auto start_time = clock::now();


    auto curr_time = clock::now();
    auto prev_time = curr_time;

    std::chrono::nanoseconds curr_ns(0);
    

    for (int i = 0; i < sparkAmount; i++) {
        //Create New Particle
        P6::P6Particles* newParticle = new P6::P6Particles();
        //Set Position to the bottom
        newParticle->Position = P6::MyVector(0, -400, 0);
        //Set mass and lifespan from 1-10;
        newParticle->mass = 1.0f;
        newParticle->lifespan = 1 + ((rand() % 10));
        newParticle->lifeRemaining = newParticle->lifespan;
        //Add force to the sparks to create a fountain firework
        P6::MyVector randomForce = P6::MyVector(rand() % 2000 - 1000, rand() % 3000 + 5000, 0);
        newParticle->AddForce(randomForce);
        //Add particles to the Physics World
        pWorld.AddParticle(newParticle);

        /*---------------Rendering particle----------------*/
        
        //Randomize Color and Radius 
        P6::MyVector randomColor = P6::MyVector((float)rand() / RAND_MAX,
            (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
        int randomRadius = 2 + (rand() % 10);
        P6::MyVector randomScale = P6::MyVector(randomRadius, randomRadius, randomRadius);
        RenderParticle* rp = new RenderParticle(newParticle, &object, randomColor,randomScale);
        RenderParticles.push_back(rp);
    }

    //particle.Position = P6::MyVector(-350, 0, 0); //top Left
    //particle.mass = 1.f; //KG
    //particle.AddForce(P6::MyVector(1000, 0, 0));

    //pWorld.AddParticle(&particle);

    //particle2.Position = P6::MyVector(-350, 0, 0); //Top right
    //particle2.mass = 1.f;
    //particle2.AddForce(P6::MyVector(1000, 0, 0));

    //pWorld.AddParticle(&particle2);
    //P6::DragForceGenerator drag = P6::DragForceGenerator(0.14, 0.1);
    //pWorld.forceRegistry.Add(&particle2, &drag);


    //particle3.Position = P6::MyVector(-350, -300, -350); //bottom right
    //particle3.mass = 1.f;
    //particle3.AddForce(P6::MyVector(1000, 0, 0));

    //pWorld.AddParticle(&particle3);


    //particle4.Position = P6::MyVector(350, -150, -350); //bottom left
    //particle4.mass = 1.f;
    //particle4.AddForce(P6::MyVector(1000, 0, 0));

    //pWorld.AddParticle(&particle4);

    //RenderParticle rp1 = RenderParticle(&particle, &object, P6::MyVector(0.7f, 0.f, 0.f));
    //RenderParticles.push_back(&rp1);
    //RenderParticle rp2 = RenderParticle(&particle2, &object2, P6::MyVector(0.f, 0.7f, 0.f));
    //RenderParticles.push_back(&rp2);
    //RenderParticle rp3 = RenderParticle(&particle3, &object3, P6::MyVector(0.f, 0.f, 0.7f));
    //RenderParticles.push_back(&rp3);
    //RenderParticle rp4 = RenderParticle(&particle4, &object4, P6::MyVector(0.7f, 0.7f, 0.f));
    //RenderParticles.push_back(&rp4);

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



        pCameraOrtho->performCamera(shader.getShaderProg());

        for (std::list<RenderParticle*>::iterator i = RenderParticles.begin();
            i != RenderParticles.end();
            i++
            ) {
            (*i)->Draw();
        }


        //if (particle.Position.x == 0 && particle2.Position.x == 0 && particle3.Position.x == 0 && particle4.Position.x == 0) {
        //    std::cout << "Blue: 1st" << '\n';
        //    std::cout << "Mag. of Velocity: " << floor(particle3Magnitude * 100 + 0.5) / 100 << "m/s" << '\n';
        //    std::cout << "Average Velocity: (" << -1 * floor(particle3AvgVelocity.x * 100 + 0.5) / 100 << ", " <<
        //        floor(particle3AvgVelocity.z * 100 + 0.5) / 100 << ", " << floor(particle3AvgVelocity.y * 100 + 0.5) / 100 << ")m/s" << '\n';
        //    std::cout << floor((particle3Time.count() / 1000.f) * 100 + 0.5) / 100 << "secs" << '\n' << '\n';

        //    std::cout << "Yellow: 2nd" << '\n';
        //    std::cout << "Mag. of Velocity: " << floor(particle4Magnitude * 100 + 0.5) / 100 << "m/s" << '\n';
        //    std::cout << "Average Velocity: (" << -1 * floor(particle4AvgVelocity.x * 100 + 0.5) / 100 << ", " <<
        //        floor(particle4AvgVelocity.z * 100 + 0.5) / 100 << ", " << floor(particle4AvgVelocity.y * 100 + 0.5) / 100 << ")m/s" << '\n';
        //    std::cout << floor((particle4Time.count() / 1000.f) * 100 + 0.5) / 100 << "secs" << '\n' << '\n';
        //    
        //    std::cout << "Red: 3rd" << '\n';
        //    std::cout << "Mag. of Velocity: " << floor(particleMagnitude * 100 + 0.5)/100 <<"m/s"<<'\n';
        //    std::cout << "Average Velocity: (" << -1 * floor(particleAvgVelocity.x * 100 + 0.5)/100<<", "<<
        //        floor(particleAvgVelocity.z * 100 + 0.5) /100 << ", " << floor(particleAvgVelocity.y * 100 + 0.5)/100 << ")m/s"<<'\n';
        //    std::cout << floor((particleTime.count() / 1000.f) * 100+ 0.5) / 100 << "secs" << '\n' << '\n';

        //    std::cout << "Green: 4th" << '\n';
        //    std::cout << "Mag. of Velocity: " << floor(particle2Magnitude * 100 + 0.5) / 100 << "m/s" << '\n';
        //    std::cout << "Average Velocity: (" << -1 * floor(particle2AvgVelocity.x * 100 + 0.5) / 100 << ", " <<
        //        floor(particle2AvgVelocity.z * 100 + 0.5) / 100 << ", " << floor(particle2AvgVelocity.y * 100 + 0.5) / 100 << ")m/s" << '\n';
        //    std::cout << floor((particle2Time.count() / 1000.f) * 100 + 0.5) / 100 << "secs" << '\n' << '\n';


        //    end = true;
        //}

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
