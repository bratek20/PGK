#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <vector>

#include "Color.h"

class Mesh;
using MeshPtr = std::shared_ptr<Mesh>;

class Mesh {
    struct Shape{
        int off;
        int size;
        int type;
    };

    static GLuint worldMatId;
    static GLuint colorId;
    static GLuint programId;
    static GLuint vertexArrayIdx;
    static GLuint vertexBufferIdx;
    static const std::vector<GLfloat> vertexData;

    Shape shape;
    Color color;

public:
    static const float EQ_TRI_H;

    static const Shape CUBE;
    static const Shape SPHERE;

    static MeshPtr create(Shape shape, Color color);

    static void init(); 
    static void clear();

    void render(const glm::mat4& worldMat);
    std::vector<glm::vec3> getLocalCoords() const;
    std::vector<glm::vec3> getWorldCoords(const glm::mat4& worldMat) const;

private:
    Mesh(){}
};

#endif