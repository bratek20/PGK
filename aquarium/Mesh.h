#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <memory>
#include <vector>

class Mesh;
using MeshPtr = std::shared_ptr<Mesh>;

class Mesh {
    struct Shape{
        int off;
        int size;
        int type;
    };

    static GLuint worldMatId;
    static GLuint programId;
    static GLuint vertexArrayIdx;
    static GLuint vertexBufferIdx;
    static GLuint colorBufferIdx;
    static const std::vector<GLfloat> vertexData;
    static const std::vector<GLfloat> colorData;

    Shape shape;
    int colorOff;

public:
    static const float EQ_TRI_H;

    static const Shape EQUILATERAL;
    static const Shape RIGHT;
    static const Shape HEXAGON;
    static const Shape HEXAGON_LINES;

    using Color = int;
    static const Color RED;
    static const Color GREEN;
    static const Color BLUE;
    static const Color RAINBOW_HEX;
    static const Color WHITE_HEX;
    static const Color BRICK;
    static const Color GRAY;
    static const Color BRICK2;
    static const Color BRICK3;

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