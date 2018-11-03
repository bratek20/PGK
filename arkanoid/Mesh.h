#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <memory>
#include <vector>

class Mesh;
using MeshPtr = std::shared_ptr<Mesh>;

class Mesh {
    static GLuint worldMatId;
    static GLuint programId;
    static GLuint vertexArrayIdx;
    static GLuint vertexBufferIdx;
    static GLuint colorBufferIdx;
    static const GLfloat vertexData[6 * 2];
    static const GLfloat colorData[9 * 3];

    int shapeOff;
    int colorOff;

public:
    using Shape = int;
    static const Shape EQUILATERAL;
    static const Shape RIGHT;

    using Color = int;
    static const Color RED;
    static const Color GREEN;
    static const Color BLUE;
    
    static MeshPtr create(Shape shape, Color color);

    static void init(); 
    static void clear();

    void render(const glm::mat3& worldMat);
    std::vector<glm::vec2> getLocalCoords() const;
    std::vector<glm::vec2> getWorldCoords(const glm::mat3& worldMat) const;

private:
    Mesh(){}
};

#endif