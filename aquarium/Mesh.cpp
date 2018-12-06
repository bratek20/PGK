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
GLuint Mesh::reverseNormalId;
GLuint Mesh::isInstancedId;

GLuint Mesh::playerPositionId;
GLuint Mesh::lightsNumId;
GLuint Mesh::lightPosId;
GLuint Mesh::lightPowerId;
GLuint Mesh::lightColorId;
GLuint Mesh::lightCoefficientsId;

GLuint Mesh::programId;
GLuint Mesh::vertexArrayIdx;
GLuint Mesh::vertexBufferIdx;
GLuint Mesh::vertexNormalsBufferIdx;
GLuint Mesh::instancedVertexTransIdx;
GLuint Mesh::instancedVertexScaleIdx;
GLuint Mesh::instancedVertexColorIdx;

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
    reverseNormalId = glGetUniformLocation(programId, "ReverseNormal");
    isInstancedId = glGetUniformLocation(programId, "IsInstanced");

    playerPositionId = glGetUniformLocation(programId, "PlayerPosition_worldspace");
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

    glGenBuffers(1, &instancedVertexTransIdx);
    glGenBuffers(1, &instancedVertexScaleIdx);
    glGenBuffers(1, &instancedVertexColorIdx);

    setProjectionMat(glm::mat4(1.0f));
    setViewMat(glm::mat4(1.0f));
}

void Mesh::clear(){
    glDisableVertexAttribArray(0);
    glDeleteBuffers(1, &instancedVertexTransIdx);
    glDeleteBuffers(1, &instancedVertexScaleIdx);
	glDeleteBuffers(1, &instancedVertexColorIdx);
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

void Mesh::applyPlayerPosition(glm::vec3 worldPlayerPos){
    glUniform3f(playerPositionId, worldPlayerPos.x, worldPlayerPos.y, worldPlayerPos.z); 
}

void Mesh::render(const glm::mat4& worldMat){
    applyCommonUniforms(worldMat);
    glUniform1i(reverseNormalId, hasNormalsReversed ? 1 : 0);
    glUniform1i(isInstancedId, 0);
    color.apply(meshColorId);

    applyShape(shape);
    glDrawArrays(shape.type, 0, shape.size);
}

void Mesh::renderInstanced(const glm::mat4& worldMat, vector<MeshPtr> meshes, vector<glm::vec3> translations, vector<glm::vec3> scales){
    applyCommonUniforms(worldMat);
    glUniform1i(isInstancedId, 1);
    glUniform1i(reverseNormalId, meshes[0]->hasNormalsReversed ? 1 : 0);
    
    auto shape = meshes[0]->shape;
    applyShape(shape);

    GLfloat instancedTrans[meshes.size() * 3];
    GLfloat instancedScale[meshes.size() * 3];
    GLfloat instancedColor[meshes.size() * 3];

    for(int i=0;i<meshes.size();i++){
        instancedTrans[3*i] = translations[i].x;
        instancedTrans[3*i+1] = translations[i].y;
        instancedTrans[3*i+2] = translations[i].z;

        instancedScale[3*i] = scales[i].x;
        instancedScale[3*i+1] = scales[i].y;
        instancedScale[3*i+2] = scales[i].z;

        instancedColor[3*i] = meshes[i]->color.r;
        instancedColor[3*i+1] = meshes[i]->color.g;
        instancedColor[3*i+2] = meshes[i]->color.b;
    }


	glBindBuffer(GL_ARRAY_BUFFER, instancedVertexTransIdx);
	glBufferData(GL_ARRAY_BUFFER, sizeof(instancedTrans), instancedTrans, GL_STREAM_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, instancedVertexScaleIdx);
	glBufferData(GL_ARRAY_BUFFER, sizeof(instancedScale), instancedScale, GL_STREAM_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, instancedVertexColorIdx);
	glBufferData(GL_ARRAY_BUFFER, sizeof(instancedColor), instancedColor, GL_STREAM_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, instancedVertexTransIdx);
    glEnableVertexAttribArray(2);
	glVertexAttribPointer(
        2,                               
        3,                                       
        GL_FLOAT,                        
        GL_FALSE,                        
        0,                               
        (void*)0                      
    );

    glBindBuffer(GL_ARRAY_BUFFER, instancedVertexScaleIdx);
    glEnableVertexAttribArray(3);
	glVertexAttribPointer(
        3,                               
        3,                                       
        GL_FLOAT,                        
        GL_FALSE,                        
        0,                               
        (void*)0                      
    );

    glBindBuffer(GL_ARRAY_BUFFER, instancedVertexColorIdx);
    glEnableVertexAttribArray(4);
	glVertexAttribPointer(
        4,                               
        3,                                       
        GL_FLOAT,                        
        GL_FALSE,                        
        0,                               
        (void*)0                      
    );
 
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1); 
    glVertexAttribDivisor(4, 1);   
    
    glDrawArraysInstanced(shape.type,  0, shape.size, meshes.size());
}

void Mesh::applyCommonUniforms(const glm::mat4& worldMat){
    glm::mat4 MVP = projectionMat * viewMat * worldMat;
    glUniformMatrix4fv(worldMatId, 1, GL_FALSE, &worldMat[0][0]);
    glUniformMatrix4fv(MVPId, 1, GL_FALSE, &MVP[0][0]);
}

void Mesh::applyShape(const Shape& shape){
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
        globalCoords.push_back(static_cast<glm::vec3>(worldMat * glm::vec4(localCoord, 1.0f)));
    }
    return globalCoords;
}

void Mesh::reverseNormals(){
    hasNormalsReversed = true;
}

Color Mesh::getColor() const{
    return color;
}