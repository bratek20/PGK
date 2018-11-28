#include "Mesh.h"

#include <common/shader.hpp>
#include <iostream>
using namespace std;

GLuint Mesh::MVPId;
GLuint Mesh::viewMatId;
GLuint Mesh::worldMatId;
GLuint Mesh::meshColorId;
GLuint Mesh::lightId;

GLuint Mesh::programId;
GLuint Mesh::vertexArrayIdx;
GLuint Mesh::vertexBufferIdx;
GLuint Mesh::vertexNormalsBufferIdx;

glm::mat4 Mesh::projectionMat;
glm::mat4 Mesh::viewMat;
glm::vec3 Mesh::globalLightPos;

const float Mesh::EQ_TRI_H = 1.73205f / 2;
std::vector<GLfloat> Mesh::vertexData = {
    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

    0.5f,  0.5f,  0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f,  0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f,  0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
};

std::vector<GLfloat> Mesh::vertexNormalsData = {
    0.0f,  0.0f, -1.0f,
    0.0f,  0.0f, -1.0f,
    0.0f,  0.0f, -1.0f,
    0.0f,  0.0f, -1.0f,
    -0.0f,  0.0f, -1.0f,
    0.0f,  0.0f, -1.0f,

    0.0f,  0.0f,  1.0f,
    0.0f,  0.0f,  1.0f,
    0.0f,  0.0f,  1.0f,
    0.0f,  0.0f,  1.0f,
    -0.0f,  0.0f,  1.0f,
    0.0f,  0.0f,  1.0f,

    -1.0f,  0.0f,  0.0f,
    -1.0f,  0.0f,  0.0f,
    -1.0f,  0.0f,  0.0f,
    -1.0f,  0.0f,  0.0f,
    -1.0f,  0.0f,  0.0f,
    -1.0f,  0.0f,  0.0f,

    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,

    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,

    -0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
    -0.0f,  1.0f,  0.0f,
    -0.0f,  1.0f,  0.0f
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

    MVPId = glGetUniformLocation(programId, "MVP");
    viewMatId = glGetUniformLocation(programId, "V");
	worldMatId = glGetUniformLocation(programId, "M");
    meshColorId = glGetUniformLocation(programId, "MeshColor");
    lightId = glGetUniformLocation(programId, "LightPosition_worldspace");

    glGenVertexArrays(1, &vertexArrayIdx);
	glBindVertexArray(vertexArrayIdx);

	glGenBuffers(1, &vertexBufferIdx);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferIdx);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertexData.size(), vertexData.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &vertexNormalsBufferIdx);
	glBindBuffer(GL_ARRAY_BUFFER, vertexNormalsBufferIdx);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertexData.size(), vertexData.data(), GL_STATIC_DRAW);

    setProjectionMat(glm::mat4(1.0f));
    setViewMat(glm::mat4(1.0f));
    setLightPosition({0,0,0});
}

void Mesh::clear(){
    glDisableVertexAttribArray(0);
    glDeleteBuffers(1, &vertexNormalsBufferIdx);
	glDeleteBuffers(1, &vertexBufferIdx);
	glDeleteProgram(programId);
	glDeleteVertexArrays(1, &vertexArrayIdx);
}

void Mesh::setProjectionMat(const glm::mat4& mat){
    projectionMat = mat;
}

void Mesh::setViewMat(const glm::mat4& mat){
    glUniformMatrix4fv(viewMatId, 1, GL_FALSE, &mat[0][0]);
    viewMat = mat;
}

void Mesh::setLightPosition(const glm::vec3& globalPos){
    glUniform3f(lightId, globalPos.x, globalPos.y, globalPos.z);
    globalLightPos = globalPos;
}

void Mesh::render(const glm::mat4& worldMat){
    glm::mat4 MVP = projectionMat * viewMat * worldMat;
    glUniformMatrix4fv(worldMatId, 1, GL_FALSE, &worldMat[0][0]);
    glUniformMatrix4fv(MVPId, 1, GL_FALSE, &MVP[0][0]);

    glUniform3f(meshColorId, color.r, color.g, color.b);

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
    glBindBuffer(GL_ARRAY_BUFFER, vertexNormalsBufferIdx);
    glEnableVertexAttribArray(1);
	glVertexAttribPointer(
        1,                               
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