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

class Light;
using LightPtr = std::shared_ptr<Light>;
using WeakLightPtr = std::weak_ptr<Light>;

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
    static GLuint reverseNormalId;
    static GLuint isInstancedId;

    static GLuint playerPositionId;
    static GLuint lightsNumId;
    static GLuint lightPosId;
    static GLuint lightPowerId;
    static GLuint lightColorId;
    static GLuint lightCoefficientsId;

    static GLuint programId;
    static GLuint vertexArrayIdx;
    static GLuint vertexBufferIdx;
    static GLuint vertexNormalsBufferIdx;
    static std::vector<GLfloat> vertexData;
    static std::vector<GLfloat> vertexNormalsData;

    static glm::mat4 projectionMat;
    static glm::mat4 viewMat;
    static std::vector<WeakLightPtr> lights;

    Shape shape;
    Color color;
    bool hasNormalsReversed = false;

public:
    static Shape CUBE;
    static Shape SPHERE;
    static const int MAX_LIGHTS;
    
    static MeshPtr create(Shape shape, Color color);

    static void init(); 
    static void clear();

    static void setProjectionMat(const glm::mat4& mat);
    static void setViewMat(const glm::mat4& mat);
    
    static void addLight(LightPtr light);
    static void applyLights();
    static void applyPlayerPosition(glm::vec3 worldPlayerPos);

    void render(const glm::mat4& worldMat);
    static void renderInstanced(const glm::mat4& worldMat, std::vector<MeshPtr> meshes, std::vector<glm::vec3> translations, std::vector<glm::vec3> scales);
    std::vector<glm::vec3> getLocalCoords() const;
    std::vector<glm::vec3> getWorldCoords(const glm::mat4& worldMat) const;
    
    void reverseNormals();
    Color getColor() const;

private:
    Mesh(){}

    static void applyCommonUniforms(const glm::mat4& worldMat);
    static void applyShape(const Shape& shape);
};

#endif