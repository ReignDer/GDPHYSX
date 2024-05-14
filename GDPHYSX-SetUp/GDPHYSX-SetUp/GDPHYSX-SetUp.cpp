// GDPHYSX-SetUp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "P6/MyVector.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define PI 3.14159


class Shader {
private:

    //Shaders
    GLuint vertexShader;
    GLuint fragShader;
    GLuint shaderProg;
public:
    //read vertex and frag shader file
    Shader(const char* v, const char* f) {
        this->compileShaders(v, f);
    }


    //Compile vertex and frag shaders into one
    void compileShaders(const char* v, const char* f) {
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

    GLuint getShaderProg() {
        return this->shaderProg;
    }

};
class Model3D {

    //Fields for Model
private:

    int img_width, //Width of the texture
        img_height, //Height of the texture
        colorChannels; //Number of color channels
    unsigned char* tex_bytes; // Tex_bytes

    int img_width2, //Width of the texture
        img_height2, //Height of the texture
        colorChannels2; //Number of color channels
    unsigned char* normal_bytes; // Normal_Tex_bytes

    //Shaders
    GLuint texture;
    GLuint norm_tex;
    GLuint vertexShader;
    GLuint fragShader;
    GLuint shaderProg;

    //Texture Address
    GLuint tex0Address;
    GLuint tex1Address;

    //Obj file attributes
    std::string path;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;

    tinyobj::attrib_t attributes;
    bool success;

    //Vertex Data
    std::vector<GLuint> mesh_indices;
    std::vector<GLfloat> fullVertexData;

    //Normal Mapping
    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> bitangents;

    //VertexArrayObject and VertexBufferObject
    GLuint VAO, VBO;

    //Matrices
    glm::mat4 identity_matrix4 = glm::mat4(1.0f);
    glm::mat4 transformation_matrix;

    //Position of the Model
    glm::vec3 modelPosition;

public:
    //Constructor & Destructor
    Model3D(std::string obj) {
        //Obj
        this->path = obj.c_str();

        this->success = tinyobj::LoadObj(
            &this->attributes,
            &this->shapes,
            &this->material,
            &this->warning,
            &this->error,
            this->path.c_str()
        );

    }
    Model3D(std::string image, std::string obj) {
        //Texture
        stbi_set_flip_vertically_on_load(true);

        this->tex_bytes =
            stbi_load(image.c_str(), //Texture path
                &this->img_width, //Fills out the width
                &this->img_height, //Fills out the height
                &this->colorChannels, //Fills out the color channels
                0);

        //Obj
        this->path = obj.c_str();

        this->success = tinyobj::LoadObj(
            &this->attributes,
            &this->shapes,
            &this->material,
            &this->warning,
            &this->error,
            this->path.c_str()
        );
    }

    Model3D(std::string image, std::string normal_img, std::string obj) {
        //Texture
        stbi_set_flip_vertically_on_load(true);

        this->tex_bytes =
            stbi_load(image.c_str(), //Texture path
                &this->img_width, //Fills out the width
                &this->img_height, //Fills out the height
                &this->colorChannels, //Fills out the color channels
                0);

        //Obj
        this->path = obj.c_str();

        this->success = tinyobj::LoadObj(
            &this->attributes,
            &this->shapes,
            &this->material,
            &this->warning,
            &this->error,
            this->path.c_str()
        );

        //Normal Texture
        stbi_set_flip_vertically_on_load(true);

        this->normal_bytes =
            stbi_load(normal_img.c_str(), //Texture path
                &this->img_width2, //Fills out the width
                &this->img_height2, //Fills out the height
                &this->colorChannels2, //Fills out the color channels
                0);

    }

    //Delete Vertex Object
    ~Model3D() {
        glDeleteVertexArrays(1, &this->VAO);
        glDeleteBuffers(1, &this->VBO);
    }

    //Methods
public:
    //read vertex and frag shader file
    void setShaders(GLuint shaderProg) {
        this->shaderProg = shaderProg;
    }

private:
    //Generate textures
    void createTexture() {
        //Generate reference
        glGenTextures(1, &this->texture);
        //Set the current texture we're
        //working
        glActiveTexture(GL_TEXTURE0);

        glBindTexture(GL_TEXTURE_2D, this->texture);


        //Assign the loaded teexture
        //to the OpenGL reference
        if (this->colorChannels == 3) {
            glTexImage2D(GL_TEXTURE_2D,
                0, //Texture 0
                GL_RGB, // Target color format of the texture
                this->img_width, // Texture width
                this->img_height,// Texture height
                0,
                GL_RGB,    //Color format of the texturue
                GL_UNSIGNED_BYTE, //Data type of texture
                this->tex_bytes); // Loaded texture in bytes
        }
        else {
            glTexImage2D(GL_TEXTURE_2D,
                0, //Texture 0
                GL_RGBA, // Target color format of the texture
                this->img_width, // Texture width
                this->img_height,// Texture height
                0,
                GL_RGBA,    //Color format of the texturue
                GL_UNSIGNED_BYTE, //Data type of texture
                this->tex_bytes); // Loaded texture in bytes
        }

        //Generate thhe mipmaps to the current texture
        glGenerateMipmap(GL_TEXTURE_2D);

        //Free uo the loaded bytes
        stbi_image_free(this->tex_bytes);
    }

    //set the Vertex and texture data of the object
    void setVertAndTex() {
        for (int i = 0; i < this->shapes[0].mesh.indices.size(); i++) {
            this->mesh_indices.push_back(
                this->shapes[0].mesh.indices[i].vertex_index
            );
        }

        for (int i = 0; i < this->shapes[0].mesh.indices.size(); i++) {

            //Assign the Index data for easy access
            tinyobj::index_t vData = this->shapes[0].mesh.indices[i];

            //Push the X position of the vertex
            this->fullVertexData.push_back(
                //Multiply the index by 3 to get the base offset
                this->attributes.vertices[(vData.vertex_index * 3)]
            );

            //Push the Y position of the vertex
            this->fullVertexData.push_back(
                //Add the base offset to 1 to get Y
                this->attributes.vertices[(vData.vertex_index * 3) + 1]
            );

            //Push the Z position of the vertex
            this->fullVertexData.push_back(
                //Add the base offset to 2 to get Z
                this->attributes.vertices[(vData.vertex_index * 3) + 2]
            );

            this->fullVertexData.push_back(
                //Add the base offset to 2 to get X
                this->attributes.normals[(vData.normal_index * 3)]
            );

            this->fullVertexData.push_back(
                //Add the base offset to 2 to get Y
                this->attributes.normals[(vData.normal_index * 3) + 1]
            );

            this->fullVertexData.push_back(
                //Add the base offset to 2 to get Z
                this->attributes.normals[(vData.normal_index * 3) + 2]
            );

            //Push the U of the Tex Coords
            this->fullVertexData.push_back(
                //Multiply the index by 3 to get the base offset
                this->attributes.texcoords[(vData.texcoord_index * 2)]
            );

            //Push the V of the Tex Coords
            this->fullVertexData.push_back(
                //Add the base offset to 1 to get V
                this->attributes.texcoords[(vData.texcoord_index * 2) + 1]
            );

        }
    }

public:
    //Createing the model
    void createModel() {
        //Call the neccessary functions to create model
        this->createTexture();
        this->setVertAndTex();

        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &this->VBO);

        //Generate VAO
        glGenVertexArrays(1, &this->VAO);

        //Generate VBO
        glGenBuffers(1, &this->VBO);


        //Bind VAO and VBO
        glBindVertexArray(this->VAO);

        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

        //Position
        glBufferData(
            GL_ARRAY_BUFFER,
            //Size of the whole array in bytes
            sizeof(GLfloat) * this->fullVertexData.size(),
            //Data of the array
            this->fullVertexData.data(),
            GL_DYNAMIC_DRAW
        );

        glVertexAttribPointer(
            0, //index 0 is the vertex position
            3, //Position is 3 floats (x,y,z)
            GL_FLOAT, // Data type of array
            GL_FALSE,

            //Our vertex data has 8 floats in it
            //(X,Y,Z,Normals,U,V)
            8 * sizeof(GLfloat),//size of the vertex data in bytes
            (void*)0
        );

        glEnableVertexAttribArray(0);

        //Since our UV starts at index 3
        //or the 4th of our index data

        //Normalize
        GLintptr normalPtr = 3 * sizeof(float);

        glVertexAttribPointer(
            1, //index 0 is the vertex position
            3, //Position is 3 floats (x,y,z)
            GL_FLOAT, // Data type of array
            GL_FALSE,

            //Our vertex data has 8 floats in it
            //(X,Y,Z,Normals,U,V)
            8 * sizeof(GLfloat),//size of the vertex data in bytes
            (void*)normalPtr
        );

        glEnableVertexAttribArray(1);


        //UV
        GLintptr uvPtr = 6 * sizeof(float);

        glVertexAttribPointer(
            2, //index 0 is the vertex position
            2, //Position is 3 floats (x,,z)
            GL_FLOAT, // Data type of array
            GL_FALSE,

            //Our vertex data has 8 floats in it
            //(X,Y,Z,Normals,U,V)
            8 * sizeof(GLfloat),//size of the vertex data in bytes
            (void*)uvPtr
        );
        glEnableVertexAttribArray(2);


        glBindBuffer(GL_ARRAY_BUFFER, 0);
        //Currently editing VBO = null

        //Currently editing VAO = VAO
        glBindVertexArray(0);
        //Currently editing VAO = null

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    //Set Object Position
    void updateTranslate(glm::vec3 position) {
        if (position.y > 0) {
            position.y = 0;
        }

        this->transformation_matrix =
            glm::translate(this->identity_matrix4,
                glm::vec3(position.x, position.y, position.z)
            );
        //Current Position of model
        this->modelPosition = glm::vec3(this->transformation_matrix[3]);
    }
    //Scale the Object
    void updateScale(float scale_x, float scale_y, float scale_z) {
        this->transformation_matrix =
            glm::scale(this->transformation_matrix,
                glm::vec3(scale_x, scale_y, scale_z)
            );
    }

    //Rotate the Object
    void updateRotation(float rotate_x, float rotate_y, float rotate_z) {
        //X Rotation
        this->transformation_matrix =
            glm::rotate(this->transformation_matrix,
                glm::radians(rotate_x),
                glm::normalize(glm::vec3(1.f, 0.f, 0.f))
            );
        //Y Rotation
        this->transformation_matrix =
            glm::rotate(this->transformation_matrix,
                glm::radians(rotate_y),
                glm::normalize(glm::vec3(0.f, 1.f, 0.f))
            );
        //Z Rotation
        this->transformation_matrix =
            glm::rotate(this->transformation_matrix,
                glm::radians(rotate_z),
                glm::normalize(glm::vec3(0.f, 0.f, 1.f))
            );
    }

    //Revolve object around a center
   /* void updateRevolution(float revolve_x, float revolve_y, float revolve_z, float rotate_x,
        float rotate_y, float rotate_z, PointLight* lightPos);*/

        //Updating Transformation matrix
    void update() {

        unsigned int transformLoc = glGetUniformLocation(this->shaderProg, "transform");

        glUniformMatrix4fv(transformLoc,
            1,
            GL_FALSE,
            glm::value_ptr(this->transformation_matrix));
    }
    //Render Texture with light
    void renderTexture();

    //Render the Complete object
    void perform() {
        //Update object
        this->update();
        //Render Texture
        this->renderTexture();


        glBindVertexArray(this->VAO);

        //Rendering the model
        glDrawArrays(GL_TRIANGLES, 0, this->fullVertexData.size() / 8);
    }

    //Getters
    GLuint getShaderProg() {
        return this->shaderProg;
    }

    glm::vec3 getModelPos() {
        return this->modelPosition;
    }
};
class MyCamera {
    //Camera Fields
protected:
    //Matrix for the projection
    glm::mat4 projectionMatrix;

    //The camera's poistion
    glm::vec3 cameraPos;
    glm::mat4 cameraPositionMatrix;

    //The camera's eye
    glm::vec3 WorldUp;
    glm::vec3 Center;
    glm::vec3 F;
    glm::vec3 R;
    glm::vec3 U;

    //The camera's orientation
    glm::mat4 cameraOrientation;

    //The View of the Camera
    glm::mat4 viewMatrix;

    //Window height and width
    float window_height;
    float window_width;

    //Default Constructor
public:
    MyCamera() {}


public:
    //Pure virtual function to implement projection type
    virtual void createProjection() = 0;
private:
    //Create the initial camera position
    void createCameraPos() {
        this->cameraPos = glm::vec3(0.f, 3.f, 10.f);

        //Construct the Position Matrix

        this->cameraPositionMatrix =
            glm::translate(glm::mat4(1.0f), //Intialize it as an Identity Matrix
                this->cameraPos * -1.f); //Multiply to -1 since we need -P

        //World's Up Direction
        //Normally just 1 in Y
        this->WorldUp = glm::vec3(0, 1.f, 0);

        //Camera's Center
        this->Center = glm::vec3(0.f, 1.f, 0.f);

        //Get Forward
        this->F = glm::vec3(this->Center - this->cameraPos);

        //Normalize the Forward
        this->F = glm::normalize(this->F);

        this->R = glm::normalize(
            //R x F
            glm::cross(this->F, WorldUp)
        );

        this->U = glm::normalize(
            //R x F
            glm::cross(this->R, this->F)
        );
    }

    //Create the initial camera orientation
    void createCameraOrientation() {
        //Construct the Camera Orientation Matrix
        this->cameraOrientation = glm::mat4(1.f);

        //Manually assign the Matrix
        //Matrix[Column][Row]
        this->cameraOrientation[0][0] = R.x;
        this->cameraOrientation[1][0] = R.y;
        this->cameraOrientation[2][0] = R.z;

        this->cameraOrientation[0][1] = U.x;
        this->cameraOrientation[1][1] = U.y;
        this->cameraOrientation[2][1] = U.z;

        this->cameraOrientation[0][2] = -F.x;
        this->cameraOrientation[1][2] = -F.y;
        this->cameraOrientation[2][2] = -F.z;
    }

    //Create the initial camera view
    void createCameraView() {
        //Camera View Matrix
        this->viewMatrix = cameraOrientation * cameraPositionMatrix;
    }
public:
    //Create the Camera
    void createCamera() {
        this->createProjection();
        this->createCameraPos();
        this->createCameraOrientation();
        this->createCameraView();
    }
public:
    //Render the camera
    void render(GLuint shaderProg) {

        unsigned int projectionLoc = glGetUniformLocation(shaderProg, "projection");

        glUniformMatrix4fv(projectionLoc,
            1,
            GL_FALSE,
            glm::value_ptr(this->projectionMatrix));

        unsigned int viewLoc = glGetUniformLocation(shaderProg, "view");

        glUniformMatrix4fv(viewLoc,
            1,
            GL_FALSE,
            glm::value_ptr(this->viewMatrix)); //View Matrix
    }
    //Virtual function for children to edit
    virtual void perform(GLuint shaderProg) {
        this->render(shaderProg);
    }

    //Getters
    glm::vec3 getCameraPos() {
        return this->cameraPos;
    }

    glm::mat4 getViewMatrix() {
        return this->viewMatrix;
    }

    glm::mat4 getProjectionMatrix() {
        return this->projectionMatrix;
    }
    void setCameraPosition(glm::vec3 modelPos) {
        this->cameraPos = modelPos;
    }
};
class OrthoCamera :
    public MyCamera {
    //Constructor
public:
    OrthoCamera(float window_height, float window_width) : MyCamera() {
        this->window_height = window_height;
        this->window_width = window_width;
    }

public:
    //Create Orthographic Projection
    void createProjection() {
        this->projectionMatrix = glm::ortho(-10.0f, //Left
            10.0f, //Right
            -10.0f, //Bottom
            10.0f, //Top
            -0.1f, //Z-Near
            100.f); //Z-Far
    }
private:
    //New camera position
    void updateCameraPos() {
        this->cameraPos = glm::vec3(0.f, 20.f, 0.f);

        //float speed = 0.005f;
        //if(upDirection)
        //    this->cameraPos += speed * this->U;
        //if(downDirection)
        //    this->cameraPos -= speed * this->U;
        //if(rightDirection)
        //    this->cameraPos += glm::normalize(glm::cross(this->F, this->U)) * speed;
        //if(leftDirection)
        //    this->cameraPos -= glm::normalize(glm::cross(this->F, this->U)) * speed;

        //Construct the Position Matrix
        this->cameraPositionMatrix =
            glm::translate(glm::mat4(1.0f), //Intialize it as an Identity Matrix
                this->cameraPos * -1.f); //Multiply to -1 since we need -P

        //World's Up Direction
        this->WorldUp = glm::vec3(0.f, 0.f, 1.f);

        //Camera's Center
        this->Center = glm::vec3(0.f, 1.f, 0.f);

        //Get Forward
        this->F = glm::vec3(this->Center - this->cameraPos);

        //Normalize the Forward
        this->F = glm::normalize(this->F);

        this->R = glm::normalize(
            //R x F
            glm::cross(this->F, WorldUp)
        );

        this->U = glm::normalize(
            //R x F
            glm::cross(this->R, this->F)
        );
    }
    //Update view withh lookAt function
    //Source::learnopengl.com/Getting-started/Camera
    void updateViewMatrix() {
        //Using the lookAt function for easy calculation of camera orientation and camera position
        this->viewMatrix = glm::lookAt(this->cameraPos, this->cameraPos + this->F, this->U);
    }

    //Update function that performs all updates
    void update() {
        this->updateCameraPos();
        this->updateViewMatrix();
    }

public:
    //Perfrom camera
    void performCamera(GLuint shaderProg) {
        this->update();
        this->render(shaderProg);
    }

};


void drawCircle(float centerX, float centerY, float radius) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float theta = 2.f * PI * (float)i / 360.f;
        float x = radius * cos(theta);
        float y = radius * sin(theta);
        glVertex2f(x + centerX, y + centerY);
    }
    glEnd();
}

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
    window = glfwCreateWindow(600, 600, "Mathieu Pobre", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    float window_width = 600;
    float window_height = 600;

    MyCamera* cameraOrtho = new OrthoCamera(window_height, window_width);
    OrthoCamera* pCameraOrtho = (OrthoCamera*)cameraOrtho;

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();


    Shader shader(v, f);

    Model3D object("3D/white.jpg","3D/wooden_sphere.obj");
    object.setShaders(shader.getShaderProg());
    object.createModel();
    cameraOrtho->createCamera();

    P6::MyVector position(10, 0, 0);
    position += P6::MyVector(-10, 0, 0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDepthMask(GL_FALSE);

        glDepthFunc(GL_LEQUAL);

        glUseProgram(shader.getShaderProg());

       

        pCameraOrtho->performCamera(shader.getShaderProg());
        position = position - P6::MyVector(-0.001, 0, 0);
        object.updateTranslate((glm::vec3)position);
        object.updateScale(0.5f, 0.5f, 0.5f);
        object.perform();

        //glColor3f(1.f,1.f,0.f);
        //drawCircle(0.f,0.f,0.5f);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

//Render Texutre Method
void Model3D::renderTexture() {
    glActiveTexture(GL_TEXTURE0);

    this->tex0Address = glGetUniformLocation(this->shaderProg, "tex0");

    glBindTexture(GL_TEXTURE_2D, this->texture);

    glUniform1i(this->tex0Address, 0);

}