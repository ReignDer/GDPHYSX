#include "RenderLine.h"

RenderLine::~RenderLine()
{
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);
    glDeleteShader(this->shaderProg);
}

void RenderLine::setUp(P6::MyVector p1, P6::MyVector p2)
{
    this->vertices = {
        p1.x,p1.y,p1.z,
        p2.x,p2.y,p2.z
    };

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);

    //Generate VAO
    glGenVertexArrays(1, &this->VAO);

    //Generate VBO
    glGenBuffers(1, &this->VBO);


    //Bind VAO and VBO
    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

    glBufferData(
        GL_ARRAY_BUFFER,
        //Size of the whole array in bytes
        sizeof(GLfloat) * this->vertices.size(),
        //Data of the array
        this->vertices.data(),
        GL_DYNAMIC_DRAW
    );

    glVertexAttribPointer(
        0, //index 0 is the vertex position
        3, //Position is 3 floats (x,y,z)
        GL_FLOAT, // Data type of array
        GL_FALSE,

        //Our vertex data has 3 floats in it
        //(X,Y,Z)
        3 * sizeof(GLfloat),//size of the vertex data in bytes
        (void*)0

    );

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //Currently editing VBO = null

    //Currently editing VAO = VAO
    glBindVertexArray(0);
    //Currently editing VAO = null

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}



void RenderLine::draw()
{
    if (p1) {
        this->setUp(p1->Position, p2->Position);
    }
    else
        this->setUp(anchorPoint,p2->Position);

    this->colorAddress = glGetUniformLocation(this->shaderProg, "color");
    glUniform3fv(this->colorAddress, 1, glm::value_ptr(this->color));
    unsigned int transformLoc = glGetUniformLocation(this->shaderProg, "transform");
    glUniformMatrix4fv(transformLoc,
        1,
        GL_FALSE,
        glm::value_ptr(this->identity_matrix4));

    glBindVertexArray(this->VAO);

    glDrawArrays(GL_LINES, 0, this->vertices.size() / 3);
}

void RenderLine::setShaders(GLuint shaderProg)
{
    this->shaderProg = shaderProg;
}

void RenderLine::setColor(glm::vec3(color))
{
    this->color = color;
}
