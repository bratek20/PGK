#include "Mesh.h"
#include "SphereGenerator.h"
#include "Light.h"

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

CommonProgram Mesh::program;
GLuint Mesh::vertexArrayIdx;
GLuint Mesh::vertexBufferIdx;
GLuint Mesh::vertexNormalsBufferIdx;
GLuint Mesh::instancedVertexTransIdx;
GLuint Mesh::instancedVertexScaleIdx;
GLuint Mesh::instancedVertexColorIdx;

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

MeshPtr Mesh::create(Shape shape, Color color){
    auto mesh = MeshPtr(new Mesh());
    mesh->shape = shape;
    mesh->color = color;
    return mesh;
}

void Mesh::init(){
    glEnable(GL_DEPTH_TEST);

    auto sphereMesh = SphereGenerator::generate(30, 30, 0.5f);
    SPHERE = {108, sphereMesh.size(), GL_TRIANGLES};
    vertexData.insert(vertexData.end(), sphereMesh.begin(), sphereMesh.end());
    vertexNormalsData.insert(vertexNormalsData.end(), sphereMesh.begin(), sphereMesh.end());

    program =  CommonProgram( "aquarium.vs", "aquarium.fs" );

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

    /*
    	GLuint Texture = loadDDS("uvmap.DDS");

	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");

	// Read our .obj file
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	bool res = loadOBJ("suzanne.obj", vertices, uvs, normals);

	// Load it into a VBO

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	GLuint normalbuffer;
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
    */
}

void Mesh::clear(){
    glDisableVertexAttribArray(0);
    glDeleteBuffers(1, &instancedVertexTransIdx);
    glDeleteBuffers(1, &instancedVertexScaleIdx);
	glDeleteBuffers(1, &instancedVertexColorIdx);
    glDeleteBuffers(1, &vertexNormalsBufferIdx);
	glDeleteBuffers(1, &vertexBufferIdx);
	program.clear();
    glDeleteVertexArrays(1, &vertexArrayIdx);
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

    applyShape(shape);
    glDrawArrays(shape.type, 0, shape.size);
}

void Mesh::renderInstanced(const glm::mat4& worldMat, vector<MeshPtr> meshes, vector<glm::vec3> translations, vector<glm::vec3> scales){
    applyCommonUniforms(worldMat);
    program.applyInstanced(true);
    program.applyNormalsReversed(meshes[0]->hasNormalsReversed);

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
    program.applyWorldMat(worldMat);
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
