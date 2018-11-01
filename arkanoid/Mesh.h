#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <glm/glm.hpp>

class Mesh{
    static GLuint worldMatId;
    static GLuint programId;
    static GLuint vertexArrayIdx;
    static GLuint vertexBufferIdx;
    static GLuint colorBufferIdx;
    static const GLfloat bufferData[3 * 2];
    static const GLfloat colorData[9 * 3];
    
public:
    void static init(); 
    void static clear();

    void render(const glm::mat3& worldMat);
};

#endif