#include "Mesh.h"

#include <common/shader.hpp>

GLuint Mesh::worldMatId;
GLuint Mesh::programId;
GLuint Mesh::vertexArrayIdx;
GLuint Mesh::vertexBufferIdx;
GLuint Mesh::colorBufferIdx;

constexpr float sqrt3 = 1.73205f;
const GLfloat Mesh::vertexData[6 * 2] = {
    -1.0f, -1.0f + sqrt3 / 3,
    1.0f, -1.0f + sqrt3 / 3,
    0.0f, sqrt3 * 2 / 3, 

    -1.0f, -1.0f,
    1.0f, -1.0f,
    -1.0f, 1.0f
};
const Mesh::Shape Mesh::EQUILATERAL = 0;
const Mesh::Shape Mesh::RIGHT = 1;

const GLfloat Mesh::colorData[9 * 3] = { 
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,

    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
};
const Mesh::Color Mesh::RED = 0;
const Mesh::Color Mesh::GREEN = 1;
const Mesh::Color Mesh::BLUE = 2;
// constexpr Color WHITE = {1.0f, 1.0f, 1.0f};
// constexpr Color BLACK = {0.0f, 0.0f, 0.0f};
// constexpr Color GREY = {0.75f, 0.75f, 0.75f};
// constexpr Color BROWN = {1.0f, 0.9f, 0.8f};
// constexpr Color YELLOW = {1.0f, 1.0f, 0.0f};
// constexpr Color MAGENTA = {1.0f, 0.0f, 1.0f};
// constexpr Color CYAN = {0.0f, 1.0f, 1.0f};

MeshPtr Mesh::create(Shape shape, Color color){
    auto mesh = MeshPtr(new Mesh());
    mesh->shapeOff = shape;
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    glGenBuffers(1, &colorBufferIdx);
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferIdx);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colorData), colorData, GL_STATIC_DRAW);
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
        (void*)(6 * shapeOff * sizeof(GLfloat))                      
    );

    glBindBuffer(GL_ARRAY_BUFFER, colorBufferIdx);
    glEnableVertexAttribArray(1);
	glVertexAttribPointer(
        1,                               
        3,                                       
        GL_FLOAT,                        
        GL_FALSE,                        
        0,                               
        (void*)(9 * colorOff * sizeof(GLfloat))                      
    );
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

std::vector<glm::vec2> Mesh::getLocalCoords() const{
    std::vector<glm::vec2> localCoords;
    for(int i=0;i<3;i++){
        localCoords.push_back(glm::vec2(vertexData[6*shapeOff + 2*i], vertexData[6*shapeOff + 2*i+1]));
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