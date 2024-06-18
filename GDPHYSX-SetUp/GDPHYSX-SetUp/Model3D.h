#pragma once 
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "tiny_obj_loader.h"

#include "stb_image.h"


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
    GLuint colorAddress;

    glm::vec3 color;

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
    Model3D(std::string obj);
    
    Model3D(std::string image, std::string obj);

    Model3D(std::string image, std::string normal_img, std::string obj);

    //Delete Vertex Object
    ~Model3D();

    //Methods
public:
    //read vertex and frag shader file
    void setShaders(GLuint shaderProg);

private:
    //Generate textures
    void createTexture();

    //set the Vertex and texture data of the object
    void setVertAndTex();

public:
    //Createing the model
    void createModel();

    //Set Object Position
    void updateTranslate(glm::vec3 position);
    //Scale the Object
    void updateScale(glm::vec3 scale);

    //Rotate the Object
    void updateRotation(float rotate_x, float rotate_y, float rotate_z);

    //Revolve object around a center
   /* void updateRevolution(float revolve_x, float revolve_y, float revolve_z, float rotate_x,
        float rotate_y, float rotate_z, PointLight* lightPos);*/

        //Updating Transformation matrix
    void update();
    //Render Texture with light
    void renderTexture();

    //Render the Complete object
    void perform();

    //Getters
    GLuint getShaderProg();

    glm::vec3 getModelPos();

    glm::vec3 getColor();

    //Setter
    void setColor(glm::vec3 color);
};