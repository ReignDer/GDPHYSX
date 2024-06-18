#include  "Model3D.h"

//Constructor & Destructor
Model3D::Model3D(std::string obj) {
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
Model3D::Model3D(std::string image, std::string obj) {
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

Model3D::Model3D(std::string image, std::string normal_img, std::string obj) {
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
Model3D::~Model3D() {
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);
}


//read vertex and frag shader file
void Model3D::setShaders(GLuint shaderProg) {
    this->shaderProg = shaderProg;
}


    //Generate textures
    void Model3D::createTexture() {
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
    void Model3D::setVertAndTex() {
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


    //Createing the model
    void Model3D::createModel() {
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
    void Model3D::updateTranslate(glm::vec3 position) {


        this->transformation_matrix =
            glm::translate(this->identity_matrix4,
                glm::vec3(position.x, position.y, position.z)
            );
        //Current Position of model
        this->modelPosition = glm::vec3(this->transformation_matrix[3]);
    }
    //Scale the Object
    void Model3D::updateScale(glm::vec3 scale) {
        this->transformation_matrix =
            glm::scale(this->transformation_matrix,
                glm::vec3(scale.x, scale.y, scale.z)
            );
    }

    //Rotate the Object
    void Model3D::updateRotation(float rotate_x, float rotate_y, float rotate_z) {
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
    void Model3D::update() {

        unsigned int transformLoc = glGetUniformLocation(this->shaderProg, "transform");

        glUniformMatrix4fv(transformLoc,
            1,
            GL_FALSE,
            glm::value_ptr(this->transformation_matrix));
    }
    //Render Texutre Method
    void Model3D::renderTexture() {
        glActiveTexture(GL_TEXTURE0);

        this->tex0Address = glGetUniformLocation(this->shaderProg, "tex0");

        glBindTexture(GL_TEXTURE_2D, this->texture);

        this->colorAddress = glGetUniformLocation(this->shaderProg, "color");
        glUniform3fv(this->colorAddress, 1, glm::value_ptr(this->color));

        glUniform1i(this->tex0Address, 0);

    }

    //Render the Complete object
    void Model3D::perform() {
        //Update object
        this->update();
        //Render Texture
        this->renderTexture();


        glBindVertexArray(this->VAO);

        //Rendering the model
        glDrawArrays(GL_TRIANGLES, 0, this->fullVertexData.size() / 8);
    }

    //Getters
    GLuint Model3D::getShaderProg() {
        return this->shaderProg;
    }

    glm::vec3 Model3D::getModelPos() {
        return this->modelPosition;
    }
    glm::vec3 Model3D::getColor()
    {
        return this->color;
    }
    //Setter
    void Model3D::setColor(glm::vec3 color) {
        this->color = color;
    }
