#include "Mesh.h"
#include "SphereGenerator.h"
#include "Light.h"
#include "Assets.h"

#include <common/shader.hpp>
#include <common/shader.hpp>
#include <common/texture.hpp>
//#include <common/controls.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>

#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <iostream>

using namespace std;

Program3D Mesh::program;
GLuint Mesh::instancedVertexTransIdx;
GLuint Mesh::instancedVertexScaleIdx;
GLuint Mesh::instancedVertexColorIdx;

MeshPtr Mesh::create(ShapePtr shape, Color color, GLuint texture, GLuint renderType){
    auto mesh = MeshPtr(new Mesh());
    mesh->shape = shape;
    mesh->color = color;
    mesh->texture = texture;
    mesh->renderType = renderType;
    return mesh;
}

void Mesh::init(){
    glEnable(GL_DEPTH_TEST);

    program = Program3D("Program3D.vs", "Program3D.fs");

    glGenBuffers(1, &instancedVertexTransIdx);
    glGenBuffers(1, &instancedVertexScaleIdx);
    glGenBuffers(1, &instancedVertexColorIdx);

    setProjectionMat(glm::mat4(1.0f));
    setViewMat(glm::mat4(1.0f));
}

void Mesh::clear(){
    glDeleteBuffers(1, &instancedVertexTransIdx);
    glDeleteBuffers(1, &instancedVertexScaleIdx);
	glDeleteBuffers(1, &instancedVertexColorIdx);
	program.clear();
}

void Mesh::setProjectionMat(const glm::mat4& mat){
    program.setProjectionMat(mat);
}

void Mesh::setViewMat(const glm::mat4& mat){
    program.setViewMat(mat);
}

void Mesh::addLight(LightPtr light){
    program.addLight(light);
}

void Mesh::applyLights(){
    program.applyLights();
}

void Mesh::applyPlayerPosition(glm::vec3 worldPlayerPos){
    program.applyPlayerPosition(worldPlayerPos);
}

void Mesh::render(const glm::mat4& worldMat){
    applyCommonUniforms(worldMat);
    program.applyInstanced(false);
    program.applyNormalsReversed(hasNormalsReversed);
    program.applyColor(color);
    program.applyTexture(texture);

    shape->apply();
    glDrawArrays(renderType, 0, shape->verticesNum());
}

void Mesh::renderInstanced(const glm::mat4& worldMat, vector<MeshPtr> meshes, vector<glm::vec3> translations, vector<glm::vec3> scales){
    applyCommonUniforms(worldMat);
    program.applyInstanced(true);

    auto mesh = meshes[0];
    auto shape = mesh->shape;
    shape->apply();
    program.applyNormalsReversed(mesh->hasNormalsReversed);
    program.applyTexture(mesh->texture);

    GLfloat instancedTrans[meshes.size() * 3];
    GLfloat instancedScale[meshes.size() * 3];
    GLfloat instancedColor[meshes.size() * 3];

    for(unsigned i=0;i<meshes.size();i++){
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
    glEnableVertexAttribArray(3);
	glVertexAttribPointer(
        3,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        (void*)0
    );

    glBindBuffer(GL_ARRAY_BUFFER, instancedVertexScaleIdx);
    glEnableVertexAttribArray(4);
	glVertexAttribPointer(
        4,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        (void*)0
    );

    glBindBuffer(GL_ARRAY_BUFFER, instancedVertexColorIdx);
    glEnableVertexAttribArray(5);
	glVertexAttribPointer(
        5,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        (void*)0
    );

    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);

    glDrawArraysInstanced(mesh->renderType,  0, shape->verticesNum(), meshes.size());
}

void Mesh::applyCommonUniforms(const glm::mat4& worldMat){
    program.applyWorldMat(worldMat);
}

const std::vector<glm::vec3>& Mesh::getLocalCoords() const{
    return shape->getVertices();
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
