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

    static GLuint MVPId;
    static GLuint viewMatId;
    static GLuint worldMatId;
    static GLuint meshColorId;
    static GLuint lightId;

    static GLuint programId;
    static GLuint vertexArrayIdx;
    static GLuint vertexBufferIdx;
    static GLuint vertexNormalsBufferIdx;
    static std::vector<GLfloat> vertexData;
    static std::vector<GLfloat> vertexNormalsData;

    static glm::mat4 projectionMat;
    static glm::mat4 viewMat;
    static glm::vec3 globalLightPos;

    Shape shape;
    Color color;

public:
    static Shape CUBE;
    static Shape SPHERE;

    static MeshPtr create(Shape shape, Color color);

    static void init(); 
    static void clear();

    static void setProjectionMat(const glm::mat4& mat);
    static void setViewMat(const glm::mat4& mat);
    static void setLightPosition(const glm::vec3& globalPos);

    void render(const glm::mat4& worldMat);
    std::vector<glm::vec3> getLocalCoords() const;
    std::vector<glm::vec3> getWorldCoords(const glm::mat4& worldMat) const;

private:
    Mesh(){}
};

#endif