#include "Mesh.h"

#include <common/shader.hpp>
#include <iostream>
using namespace std;

GLuint Mesh::worldMatId;
GLuint Mesh::colorId;
GLuint Mesh::programId;
GLuint Mesh::vertexArrayIdx;
GLuint Mesh::vertexBufferIdx;

const float Mesh::EQ_TRI_H = 1.73205f / 2;
const std::vector<GLfloat> Mesh::vertexData = {
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
};

const Mesh::Shape Mesh::CUBE = { 0, 108, GL_TRIANGLES };

MeshPtr Mesh::create(Shape shape, Color color){
    auto mesh = MeshPtr(new Mesh());
    mesh->shape = shape;
    mesh->color = color;
    return mesh;
}

void Mesh::init()
{
    programId =  LoadShaders( "aquarium.vs", "aquarium.fs" ); 
    glUseProgram(programId);

	worldMatId = glGetUniformLocation(programId, "worldMat");
    colorId = glGetUniformLocation(programId, "color");

    glGenVertexArrays(1, &vertexArrayIdx);
	glBindVertexArray(vertexArrayIdx);

	glGenBuffers(1, &vertexBufferIdx);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferIdx);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertexData.size(), vertexData.data(), GL_STATIC_DRAW);
}

void Mesh::clear()
{
    glDisableVertexAttribArray(0);
	glDeleteBuffers(1, &vertexBufferIdx);
	glDeleteProgram(programId);
	glDeleteVertexArrays(1, &vertexArrayIdx);
}

void Mesh::render(const glm::mat4& worldMat){
    glUniformMatrix4fv(worldMatId, 1, GL_FALSE, &worldMat[0][0]);
    glUniform3f(colorId, color.r, color.g, color.b);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferIdx);
    glEnableVertexAttribArray(0);
	glVertexAttribPointer(
        0,                               
        3,                                       
        GL_FLOAT,                        
        GL_FALSE,                        
        0,                               
        (void*)(shape.off * sizeof(GLfloat))                      
    );
    glDrawArrays(shape.type, 0, shape.size);
}

std::vector<glm::vec3> Mesh::getLocalCoords() const{
    std::vector<glm::vec3> localCoords;
    for(int i=0;i<shape.size/3;i++){
        localCoords.push_back(glm::vec3(vertexData[shape.off + 3*i], vertexData[shape.off + 3*i+1],  vertexData[shape.off + 3*i+2]));
    }
    return localCoords;
}

std::vector<glm::vec3> Mesh::getWorldCoords(const glm::mat4& worldMat) const{
    std::vector<glm::vec3> globalCoords;
    for(auto& localCoord : getLocalCoords()){
        globalCoords.push_back(worldMat * glm::vec4(localCoord, 1.0f));
    }
    return globalCoords;
}