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

#include "P6/MyVector.h"
#include "P6/P6Particles.h"
#include "P6/PhysicsWorld.h"
#include "P6/RenderParticle.h"

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
    window = glfwCreateWindow(700, 700, "PC01 Mathieu Marc I. Pobre", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    float window_width = 700;
    float window_height = 700;

    MyCamera* cameraOrtho = new OrthoCamera(window_height, window_width);
    OrthoCamera* pCameraOrtho = (OrthoCamera*)cameraOrtho;
    //pCameraOrtho->setProjection(500,500,500,500);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();


    Shader shader(v, f);

    Model3D object("3D/white.jpg", "3D/wooden_sphere.obj");
    object.setShaders(shader.getShaderProg());
    object.createModel();

    Model3D object2 = object;

    Model3D object3 = object;

    Model3D object4 = object;


    cameraOrtho->createCamera();

    P6::PhysicsWorld pWorld = P6::PhysicsWorld();
    P6::P6Particles particle = P6::P6Particles();
    P6::P6Particles particle2 = P6::P6Particles();
    P6::P6Particles particle3 = P6::P6Particles();
    P6::P6Particles particle4 = P6::P6Particles();


    using clock = std::chrono::high_resolution_clock;
    auto start_time = clock::now();


    auto curr_time = clock::now();
    auto prev_time = curr_time;
    std::chrono::milliseconds particleTime(0);
    std::chrono::milliseconds particle2Time(0);
    std::chrono::milliseconds particle3Time(0);
    std::chrono::milliseconds particle4Time(0);

    float particleMagnitude = 0;
    float particle2Magnitude = 0;
    float particle3Magnitude = 0;
    float particle4Magnitude = 0;

    P6::MyVector particleFinalV(0, 0, 0);
    P6::MyVector particle2FinalV(0, 0, 0);
    P6::MyVector particle3FinalV(0, 0, 0);
    P6::MyVector particle4FinalV(0, 0, 0);

    P6::MyVector particleAvgVelocity(0,0,0);
    P6::MyVector particle2AvgVelocity(0, 0, 0);
    P6::MyVector particle3AvgVelocity(0, 0, 0);
    P6::MyVector particle4AvgVelocity(0, 0, 0);

    bool recorded = false;
    bool recorded2 = false;
    bool recorded3 = false;
    bool recorded4 = false;

    std::chrono::nanoseconds curr_ns(0);

    std::list<RenderParticle*> RenderParticles;

    particle.Position = P6::MyVector(350, 201, 350); //top Left
    particle.mass = 1.f; //KG
    particle.AddForce(P6::MyVector(-8000, -8000, -8000));

    pWorld.AddParticle(&particle);

    particle2.Position = P6::MyVector(-350, 173, 350); //Top right
    particle2.Velocity = P6::MyVector(90, -90, -90);
    particle2.Acceleration = P6::MyVector(8, -8, -8);

    pWorld.AddParticle(&particle2);


    particle3.Position = P6::MyVector(-350, -300, -350); //bottom right
    particle3.Velocity = P6::MyVector(130, 130, 130);
    particle3.Acceleration = P6::MyVector(1, 1, 1);

    pWorld.AddParticle(&particle3);


    particle4.Position = P6::MyVector(350, -150, -350); //bottom left
    particle4.Velocity = P6::MyVector(-110, 110, 110);
    particle4.Acceleration = P6::MyVector(-3, 3, 3);

    pWorld.AddParticle(&particle4);

    RenderParticle rp1 = RenderParticle(&particle, &object, P6::MyVector(0.7f, 0.f, 0.f));
    RenderParticles.push_back(&rp1);
    RenderParticle rp2 = RenderParticle(&particle2, &object2, P6::MyVector(0.f, 0.7f, 0.f));
    RenderParticles.push_back(&rp2);
    RenderParticle rp3 = RenderParticle(&particle3, &object3, P6::MyVector(0.f, 0.f, 0.7f));
    RenderParticles.push_back(&rp3);
    RenderParticle rp4 = RenderParticle(&particle4, &object4, P6::MyVector(0.7f, 0.7f, 0.f));
    RenderParticles.push_back(&rp4);

    float z_Velocity = 0.f;
    float z_Velocity2 = 0.f;
    float z_Velocity3 = 0.f;
    float z_Velocity4 = 0.f;

    bool z = false;
    bool z2 = false;
    bool z3 = false;
    bool z4 = false;

    P6::MyVector position(10, 0, 0);

    position += P6::MyVector(-10, 0, 0);
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



            if (particle.Position.y <= 0) {
                particle.Position.y = 0;
                if (!z) {
                    z_Velocity = (particle.Position.y + -80) / 2;
                    z = true;
                }
 
                if (particle.Position.z <= 0) {
   
                        auto end_time = clock::now();
                        particleTime = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
                        particle.Velocity.y = z_Velocity;
                        particleMagnitude = particle.Velocity.magnitude();
                        particleFinalV = particle.Velocity + P6::MyVector(-80, -80, -80);
                        particleAvgVelocity = particleFinalV / 2;
                        particle.Destroy();

                }

            }

            if (particle2.Position.y <= 0) {
                particle2.Position.y = 0;
                if (!z2) {
                    z_Velocity2 = (particle2.Position.y + -90) / 2;
                    z2 = true;
                }
                if (particle2.Position.z <= 0) {

                        auto end_time = clock::now();
                        particle2Time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
                        particle2.Velocity.y = z_Velocity2;
                        particle2Magnitude = particle2.Velocity.magnitude();
                        particle2FinalV = particle2.Velocity + P6::MyVector(90, -90, -90);
                        particle2AvgVelocity = particle2FinalV / 2;
                        particle2.Destroy();

                    

                }


            }
            if (particle3.Position.y >= 0) {
                particle3.Position.y = 0;
                if (!z3) {
                    z_Velocity3 = (particle3.Position.y + 130) / 2;
                    z3 = true;
                }
                if (particle3.Position.z >= 0) {

                        auto end_time = clock::now();
                        particle3Time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
                        particle3.Velocity.y = z_Velocity3;
                        particle3Magnitude = particle3.Velocity.magnitude();
                        particle3FinalV = particle3.Velocity + P6::MyVector(130, 130, 130);
                        particle3AvgVelocity = particle3FinalV / 2;
                        particle3.Destroy();

   
                }


            }
            if (particle4.Position.y >= 0) {
                particle4.Position.y = 0;
                if (!z4) {
                    z_Velocity4 = (particle4.Position.y + 110) / 2;
                    z4 = true;
                }
                if (particle4.Position.z >= 0) {
                        auto end_time = clock::now();
                        particle4Time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
                        particle4.Velocity.y = z_Velocity4;
                        particle4Magnitude = particle4.Velocity.magnitude();
                        particle4FinalV = particle4.Velocity + P6::MyVector(-110, 110, 110);
                        particle4AvgVelocity = particle4FinalV / 2;
                        particle4.Destroy();
                    
 
                }


            }
        }
        //std::cout << "Normal Update\n";


        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDepthMask(GL_FALSE);

        glDepthFunc(GL_LEQUAL);

        glUseProgram(shader.getShaderProg());



        pCameraOrtho->performCamera(shader.getShaderProg());
        position = position - P6::MyVector(-0.001, 0, 0);

        for (std::list<RenderParticle*>::iterator i = RenderParticles.begin();
            i != RenderParticles.end();
            i++
            ) {
            (*i)->Draw();
        }


        if (particle.Position.x == 0 && particle2.Position.x == 0 && particle3.Position.x == 0 && particle4.Position.x == 0) {
            std::cout << "Blue: 1st" << '\n';
            std::cout << "Mag. of Velocity: " << floor(particle3Magnitude * 100 + 0.5) / 100 << "m/s" << '\n';
            std::cout << "Average Velocity: (" << -1 * floor(particle3AvgVelocity.x * 100 + 0.5) / 100 << ", " <<
                floor(particle3AvgVelocity.z * 100 + 0.5) / 100 << ", " << floor(particle3AvgVelocity.y * 100 + 0.5) / 100 << ")m/s" << '\n';
            std::cout << floor((particle3Time.count() / 1000.f) * 100 + 0.5) / 100 << "secs" << '\n' << '\n';

            std::cout << "Yellow: 2nd" << '\n';
            std::cout << "Mag. of Velocity: " << floor(particle4Magnitude * 100 + 0.5) / 100 << "m/s" << '\n';
            std::cout << "Average Velocity: (" << -1 * floor(particle4AvgVelocity.x * 100 + 0.5) / 100 << ", " <<
                floor(particle4AvgVelocity.z * 100 + 0.5) / 100 << ", " << floor(particle4AvgVelocity.y * 100 + 0.5) / 100 << ")m/s" << '\n';
            std::cout << floor((particle4Time.count() / 1000.f) * 100 + 0.5) / 100 << "secs" << '\n' << '\n';
            
            std::cout << "Red: 3rd" << '\n';
            std::cout << "Mag. of Velocity: " << floor(particleMagnitude * 100 + 0.5)/100 <<"m/s"<<'\n';
            std::cout << "Average Velocity: (" << -1 * floor(particleAvgVelocity.x * 100 + 0.5)/100<<", "<<
                floor(particleAvgVelocity.z * 100 + 0.5) /100 << ", " << floor(particleAvgVelocity.y * 100 + 0.5)/100 << ")m/s"<<'\n';
            std::cout << floor((particleTime.count() / 1000.f) * 100+ 0.5) / 100 << "secs" << '\n' << '\n';

            std::cout << "Green: 4th" << '\n';
            std::cout << "Mag. of Velocity: " << floor(particle2Magnitude * 100 + 0.5) / 100 << "m/s" << '\n';
            std::cout << "Average Velocity: (" << -1 * floor(particle2AvgVelocity.x * 100 + 0.5) / 100 << ", " <<
                floor(particle2AvgVelocity.z * 100 + 0.5) / 100 << ", " << floor(particle2AvgVelocity.y * 100 + 0.5) / 100 << ")m/s" << '\n';
            std::cout << floor((particle2Time.count() / 1000.f) * 100 + 0.5) / 100 << "secs" << '\n' << '\n';


            end = true;
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
