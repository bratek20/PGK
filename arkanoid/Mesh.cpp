#include "Mesh.h"

#include <common/shader.hpp>

GLuint Mesh::worldMatId;
GLuint Mesh::programId;
GLuint Mesh::vertexArrayIdx;
GLuint Mesh::vertexBufferIdx;
GLuint Mesh::colorBufferIdx;

constexpr float sqrt3 = 1.73205f;
const GLfloat Mesh::bufferData[3 * 2] = {
    -1.0f, -1.0f + sqrt3 / 3,
    1.0f, -1.0f + sqrt3 / 3,
    0.0f, sqrt3 * 2 / 3
};

// constexpr Color WHITE = {1.0f, 1.0f, 1.0f};
// constexpr Color BLACK = {0.0f, 0.0f, 0.0f};
// constexpr Color GREY = {0.75f, 0.75f, 0.75f};
// constexpr Color BROWN = {1.0f, 0.9f, 0.8f};

// constexpr Color RED = {1.0f, 0.0f, 0.0f};
// constexpr Color GREEN = {0.0f, 1.0f, 0.0f};
// constexpr Color BLUE = {0.0f, 0.0f, 1.0f};
// constexpr Color YELLOW = {1.0f, 1.0f, 0.0f};
// constexpr Color MAGENTA = {1.0f, 0.0f, 1.0f};
// constexpr Color CYAN = {0.0f, 1.0f, 1.0f};

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

void Mesh::init()
{
    programId =  LoadShaders( "arkanoid.vs", "arkanoid.fs" ); 
    glUseProgram(programId);

	worldMatId = glGetUniformLocation(programId, "worldMat");
    
    glGenVertexArrays(1, &vertexArrayIdx);
	glBindVertexArray(vertexArrayIdx);

	glGenBuffers(1, &vertexBufferIdx);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferIdx);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bufferData), bufferData, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        2,
        GL_FLOAT,
        GL_FALSE,
        0,
        (void*)0
    );

    glGenBuffers(1, &colorBufferIdx);
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferIdx);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colorData), colorData, GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,                               
        3,                                       
        GL_FLOAT,                        
        GL_FALSE,                        
        9 * sizeof(GLfloat),                               
        (void*)0                      
    );
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
    glDrawArrays(GL_TRIANGLES, 0, 3);
}