#include "Mesh.h"

#include <common/shader.hpp>

GLuint Mesh::worldMatId;
GLuint Mesh::programId;
GLuint Mesh::vertexArrayIdx;
GLuint Mesh::vertexBufferIdx;
GLuint Mesh::colorBufferIdx;

const float Mesh::EQ_TRI_H = 1.73205f / 2;
const std::vector<GLfloat> Mesh::vertexData = {
    -0.5f, -EQ_TRI_H / 3,
    0.5f, -EQ_TRI_H / 3,
    0.0f, EQ_TRI_H * 2 / 3, 

    -0.5f, -0.5f,
    0.5f, -0.5f,
    -0.5f, 0.5f,

    -0.5f, -EQ_TRI_H,
    0.5f, -EQ_TRI_H,
    1.0f, 0.0f, 
    0.5f, EQ_TRI_H,
    -0.5f, EQ_TRI_H,
    -1.0f, 0.0f,
};
const Mesh::Shape Mesh::EQUILATERAL = {0, 3, GL_TRIANGLES};
const Mesh::Shape Mesh::RIGHT = {6, 3, GL_TRIANGLES};
const Mesh::Shape Mesh::HEXAGON = {12, 6, GL_TRIANGLE_FAN};
const Mesh::Shape Mesh::HEXAGON_LINES = {12, 6, GL_LINE_STRIP};

const std::vector<GLfloat> Mesh::colorData = { 
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,

    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,

    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f,

    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,

    1.0f, 0.5f, 0.4f,
    1.0f, 0.5f, 0.4f,
    1.0f, 0.9f, 0.8f,

    0.75f, 0.75f, 0.75f,
    0.75f, 0.75f, 0.75f,
    0.75f, 0.75f, 0.75f,

    0.5f, 1.0f, 0.0f,
    0.5f, 1.0f, 0.0f,
    0.9f, 1.0f, 0.0f,

    0.2f, 1.0f, 0.9f,
    0.2f, 1.0f, 0.9f,
    0.2f, 0.5f, 0.9f,

};
const Mesh::Color Mesh::RED = 0;
const Mesh::Color Mesh::GREEN = 9;
const Mesh::Color Mesh::BLUE = 18;
const Mesh::Color Mesh::RAINBOW_HEX = 27;
const Mesh::Color Mesh::WHITE_HEX = 45;
const Mesh::Color Mesh::BRICK = 63;
const Mesh::Color Mesh::GRAY = 72;
const Mesh::Color Mesh::BRICK2 = 81;
const Mesh::Color Mesh::BRICK3 = 90;
// constexpr Color WHITE = {1.0f, 1.0f, 1.0f};
// constexpr Color BLACK = {0.0f, 0.0f, 0.0f};
// constexpr Color GREY = {0.75f, 0.75f, 0.75f};
// constexpr Color BROWN = {1.0f, 0.9f, 0.8f};
// constexpr Color YELLOW = {1.0f, 1.0f, 0.0f};
// constexpr Color MAGENTA = {1.0f, 0.0f, 1.0f};
// constexpr Color CYAN = {0.0f, 1.0f, 1.0f};

MeshPtr Mesh::create(Shape shape, Color color){
    auto mesh = MeshPtr(new Mesh());
    mesh->shape = shape;
    mesh->colorOff = color;
    return mesh;
}

void Mesh::init()
{
    programId =  LoadShaders( "arkanoid.vs", "arkanoid.fs" ); 
    glUseProgram(programId);

	worldMatId = glGetUniformLocation(programId, "worldMat");
    
    glGenVertexArrays(1, &vertexArrayIdx);
	glBindVertexArray(vertexArrayIdx);

	glGenBuffers(1, &vertexBufferIdx);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferIdx);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertexData.size(), vertexData.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &colorBufferIdx);
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferIdx);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * colorData.size(), colorData.data(), GL_STATIC_DRAW);
}

void Mesh::clear()
{
    glDisableVertexAttribArray(1);
	glDeleteBuffers(1, &colorBufferIdx);
    glDisableVertexAttribArray(0);
	glDeleteBuffers(1, &vertexBufferIdx);
	glDeleteProgram(programId);
	glDeleteVertexArrays(1, &vertexArrayIdx);
}

void Mesh::render(const glm::mat3& worldMat){
    glUniformMatrix3fv(worldMatId, 1, GL_FALSE, &worldMat[0][0]);
    
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferIdx);
    glEnableVertexAttribArray(0);
	glVertexAttribPointer(
        0,                               
        2,                                       
        GL_FLOAT,                        
        GL_FALSE,                        
        0,                               
        (void*)(shape.off * sizeof(GLfloat))                      
    );

    glBindBuffer(GL_ARRAY_BUFFER, colorBufferIdx);
    glEnableVertexAttribArray(1);
	glVertexAttribPointer(
        1,                               
        3,                                       
        GL_FLOAT,                        
        GL_FALSE,                        
        0,                               
        (void*)(colorOff * sizeof(GLfloat))                      
    );
    glDrawArrays(shape.type, 0, shape.size);
}

std::vector<glm::vec2> Mesh::getLocalCoords() const{
    std::vector<glm::vec2> localCoords;
    for(int i=0;i<3;i++){
        localCoords.push_back(glm::vec2(vertexData[shape.off + 2*i], vertexData[shape.off + 2*i+1]));
    }
    return localCoords;
}

std::vector<glm::vec2> Mesh::getWorldCoords(const glm::mat3& worldMat) const{
    std::vector<glm::vec2> globalCoords;
    for(auto& localCoord : getLocalCoords()){
        globalCoords.push_back(worldMat * glm::vec3(localCoord, 1.0f));
    }
    return globalCoords;
}