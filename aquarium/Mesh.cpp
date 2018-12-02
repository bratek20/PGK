#include "Mesh.h"
#include "SphereGenerator.h"
#include "Light.h"

#include <common/shader.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <iostream>

using namespace std;

GLuint Mesh::MVPId;
GLuint Mesh::viewMatId;
GLuint Mesh::worldMatId;
GLuint Mesh::meshColorId;

GLuint Mesh::lightsNumId;
GLuint Mesh::lightPosId;
GLuint Mesh::lightPowerId;
GLuint Mesh::lightColorId;
GLuint Mesh::lightCoefficientsId;

GLuint Mesh::programId;
GLuint Mesh::vertexArrayIdx;
GLuint Mesh::vertexBufferIdx;
GLuint Mesh::vertexNormalsBufferIdx;

glm::mat4 Mesh::projectionMat;
glm::mat4 Mesh::viewMat;
std::vector<WeakLightPtr> Mesh::lights;

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

Mesh::Shape Mesh::CUBE = { 0, 108, GL_TRIANGLES };
Mesh::Shape Mesh::SPHERE;
const int Mesh::MAX_LIGHTS = 10;

MeshPtr Mesh::create(Shape shape, Color color){
    auto mesh = MeshPtr(new Mesh());
    mesh->shape = shape;
    mesh->color = color;
    return mesh;
}

void Mesh::init(){
    auto sphereMesh = SphereGenerator::generate(30, 30, 0.5f);
    SPHERE = {108, sphereMesh.size(), GL_TRIANGLES};
    vertexData.insert(vertexData.end(), sphereMesh.begin(), sphereMesh.end());
    vertexNormalsData.insert(vertexNormalsData.end(), sphereMesh.begin(), sphereMesh.end());

    programId =  LoadShaders( "aquarium.vs", "aquarium.fs" ); 
    glUseProgram(programId);

    MVPId = glGetUniformLocation(programId, "MVP");
    viewMatId = glGetUniformLocation(programId, "V");
	worldMatId = glGetUniformLocation(programId, "M");
    meshColorId = glGetUniformLocation(programId, "MeshColor");

    lightsNumId = glGetUniformLocation(programId, "LightsNum");
    lightPosId = glGetUniformLocation(programId, "LightPosition_worldspace");
    lightPowerId = glGetUniformLocation(programId, "LightPower");
    lightColorId = glGetUniformLocation(programId, "LightColor");
    lightCoefficientsId = glGetUniformLocation(programId, "LightDistanceCoefficients");

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

void Mesh::addLight(LightPtr light){
    lights.push_back(light);
   
}

void Mesh::applyLights(){
    lights.erase(
        std::remove_if(lights.begin(), lights.end(), [](WeakLightPtr l){return l.expired();}),
        lights.end());

    int size = min(static_cast<int>(lights.size()), MAX_LIGHTS);
    glUniform1i(lightsNumId, size); 

    glm::vec3 pos[size];
    float power[size];
    glm::vec3 color[size];
    glm::vec3 coefficient[size];
    for(int i=0;i<size;i++){
        auto light = lights[i].lock(); 
        pos[i] = light->getWorldPosition();
        power[i] = light->getPower();
        color[i] = static_cast<glm::vec3>(light->getColor());
        coefficient[i] = light->getCoefficients();
    }

    glUniform3fv(lightPosId, size, glm::value_ptr(pos[0]));
    glUniform1fv(lightPowerId, size, power);
    glUniform3fv(lightColorId, size, glm::value_ptr(color[0]));
    glUniform3fv(lightCoefficientsId, size, glm::value_ptr(coefficient[0]));
}

void Mesh::render(const glm::mat4& worldMat){
    glm::mat4 MVP = projectionMat * viewMat * worldMat;
    glUniformMatrix4fv(worldMatId, 1, GL_FALSE, &worldMat[0][0]);
    glUniformMatrix4fv(MVPId, 1, GL_FALSE, &MVP[0][0]);

    color.apply(meshColorId);

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

Color Mesh::getColor() const{
    return color;
}