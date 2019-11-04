#pragma once

#include <iostream>

#include "Cloth.h"
#include "Program.h"
#include "stb_image.h"

struct Camera
{
    const float speed = 0.3f;
    glm::vec3 pos = glm::vec3(0.0f, 4.0f, 12.0f);
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
};
Camera cam;

struct Light
{
    glm::vec3 pos = glm::vec3(-5.0f, 7.0f, 6.0f);
    glm::vec3 color = glm::vec3(0.8f, 0.8f, 1.0f);
};
Light lamp;

struct ClothRender
{
    const Cloth* cloth;
    int nodeCount; // Number of nodes in faces
    
    glm::vec3 *vboPos; // Position
    glm::vec2 *vboTex; // Texture
    glm::vec3 *vboNor; // Normal

    GLuint vaoID;
    GLuint vboIDs[3];
    GLuint texID;
    GLuint programID;
//    GLint uniProjMatrix;
    
    GLint aPtrPos;
    GLint aPtrTex;
    GLint aPtrNor;
    
    ClothRender(Cloth* cloth)
    {
        nodeCount = (int)(cloth->faces.size());
        if (nodeCount <= 0) {
            std::cout << "ERROR::ClothRender : No node exists." << std::endl;
            exit(-1);
        }
        
        this->cloth = cloth;
        
        vboPos = new glm::vec3[nodeCount];
        vboTex = new glm::vec2[nodeCount];
        vboNor = new glm::vec3[nodeCount];
        
        // The texture coord will only be set here
        for (int i = 0; i < nodeCount; i ++) {
            Node* n = cloth->faces[i];
            vboPos[i] = glm::vec3(n->position.x, n->position.y, n->position.z);
            vboTex[i] = glm::vec2(n->texCoord.x, n->texCoord.y);
            vboNor[i] = glm::vec3(n->normal.x, n->normal.y, n->normal.z);
        }
        
        /** Build render program **/
        Program program("Shaders/ClothVS.glsl", "Shaders/ClothFS.glsl");
        programID = program.ID;
        std::cout << "Cloth Program ID: " << programID << std::endl;

        // Generate ID of VAO and VBOs
        glGenVertexArrays(1, &vaoID);
        glGenBuffers(3, vboIDs);
        
        // Attribute pointers of VAO
        aPtrPos = 0;
        aPtrTex = 1;
        aPtrNor = 2;
        // Bind VAO
        glBindVertexArray(vaoID);
        
        // Position buffer
        glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]);
        glVertexAttribPointer(aPtrPos, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glBufferData(GL_ARRAY_BUFFER, nodeCount*sizeof(glm::vec3), vboPos, GL_DYNAMIC_DRAW);
        // Texture buffer
        glBindBuffer(GL_ARRAY_BUFFER, vboIDs[1]);
        glVertexAttribPointer(aPtrTex, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glBufferData(GL_ARRAY_BUFFER, nodeCount*sizeof(glm::vec2), vboTex, GL_DYNAMIC_DRAW);
        // Normal buffer
        glBindBuffer(GL_ARRAY_BUFFER, vboIDs[2]);
        glVertexAttribPointer(aPtrNor, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glBufferData(GL_ARRAY_BUFFER, nodeCount*sizeof(glm::vec3), vboNor, GL_DYNAMIC_DRAW);
        
        // Enable it's attribute pointers since they were set well
        glEnableVertexAttribArray(aPtrPos);
        glEnableVertexAttribArray(aPtrTex);
        glEnableVertexAttribArray(aPtrNor);
        
        /** Load texture **/
        // Assign texture ID and gengeration
        glGenTextures(1, &texID);
        glBindTexture(GL_TEXTURE_2D, texID);
        // Set the texture wrapping parameters (for 2D tex: S, T)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // Set texture filtering parameters (Minify, Magnify)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        /** Load image and configure texture **/
        stbi_set_flip_vertically_on_load(true);
        int texW, texH, colorChannels; // After loading the image, stb_image will fill them
        unsigned char *data = stbi_load("Images/texture1.jpg", &texW, &texH, &colorChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texW, texH, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            // Automatically generate all the required mipmaps for the currently bound texture.
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            cout << "Failed to load texture" << endl;
        }
        // Always free image memory
        stbi_image_free(data);
        
        /** Set uniform **/
        glUseProgram(programID); // Active shader before set uniform
        // Set texture sampler
        glUniform1i(glGetUniformLocation(programID, "uniTex"), 0);
        
        /** Projection matrix : The frustum that camera observes **/
        // Since projection matrix rarely changes, set it outside the rendering loop for only onec time
        glm::mat4 uniProjMatrix = glm::mat4(1.0f);
        float frustumRatio = 1.0; // TODO: the ratio of width and height of camera?
        uniProjMatrix = glm::perspective(glm::radians(45.0f), frustumRatio, 0.1f, 100.0f);
        glUniformMatrix4fv(glGetUniformLocation(programID, "uniProjMatrix"), 1, GL_FALSE, &uniProjMatrix[0][0]);
        
        /** Model Matrix : Put cloth into the world **/
        glm::mat4 uniModelMatrix = glm::mat4(1.0f);
        uniModelMatrix = glm::translate(uniModelMatrix, glm::vec3(-cloth->width/2, cloth->height, -2.0));
        glUniformMatrix4fv(glGetUniformLocation(programID, "uniModelMatrix"), 1, GL_FALSE, &uniModelMatrix[0][0]);
        
        /** Light **/
        glUniform3fv(glGetUniformLocation(programID, "uniLightPos"), 1, &(lamp.pos[0]));
        glUniform3fv(glGetUniformLocation(programID, "uniLightColor"), 1, &(lamp.color[0]));

        // Cleanup
        glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbined VBO
        glBindVertexArray(0); // Unbined VAO
    }
    
    void destroy()
    {
        delete [] vboPos;
        delete [] vboTex;
        delete [] vboNor;
        
        if (vaoID)
        {
            glDeleteVertexArrays(1, &vaoID);
            glDeleteBuffers(3, vboIDs);
            vaoID = 0;
        }
        if (programID)
        {
            glDeleteProgram(programID);
            programID = 0;
        }
    }
    
    void flush()
    {
        // Update all the positions of nodes
        for (int i = 0; i < nodeCount; i ++) { // Tex coordinate dose not change
            Node* n = cloth->faces[i];
            vboPos[i] = glm::vec3(n->position.x, n->position.y, n->position.z);
            vboNor[i] = glm::vec3(n->normal.x, n->normal.y, n->normal.z);
        }
        
        glUseProgram(programID);
        
        glBindVertexArray(vaoID);
        
        glBindBuffer(GL_ARRAY_BUFFER, vboIDs[0]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, nodeCount*sizeof(glm::vec3), vboPos);
        glBindBuffer(GL_ARRAY_BUFFER, vboIDs[1]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, nodeCount* sizeof(glm::vec2), vboTex);
        glBindBuffer(GL_ARRAY_BUFFER, vboIDs[2]);
        glBufferSubData(GL_ARRAY_BUFFER, 0, nodeCount* sizeof(glm::vec3), vboNor);
        
        /** Bind texture **/
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texID);
        
        /** View Matrix : The camera **/
        glm::mat4 uniViewMatrix = glm::mat4(1.0f);
        uniViewMatrix = glm::lookAt(cam.pos, cam.pos + cam.front, cam.up);
        glUniformMatrix4fv(glGetUniformLocation(programID, "uniViewMatrix"), 1, GL_FALSE, &uniViewMatrix[0][0]);
        
        glEnable(GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        /** Draw **/
        glDrawArrays(GL_TRIANGLES, 0, nodeCount);
        
        // End flushing
        glDisable(GL_BLEND);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glUseProgram(0);
    }
};
