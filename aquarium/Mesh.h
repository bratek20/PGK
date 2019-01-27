#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <vector>

#include "Program3D.h"
#include "Shape.h"
#include "Color.h"

class Mesh;
using MeshPtr = std::shared_ptr<Mesh>;

class Light;
using LightPtr = std::shared_ptr<Light>;
using WeakLightPtr = std::weak_ptr<Light>;

class Mesh {   
    static Program3D program;
    static GLuint instancedVertexTransIdx;
    static GLuint instancedVertexScaleIdx;
    static GLuint instancedVertexColorIdx;

    ShapePtr shape;
    Color color;
    GLuint texture;
    GLuint renderType;
    bool hasNormalsReversed = false;

public:
    static MeshPtr create(ShapePtr shape, Color color, GLuint texture, GLuint renderType = GL_TRIANGLES);

    static void init(); 
    static void clear();

    static void setProjectionMat(const glm::mat4& mat);
    static void setViewMat(const glm::mat4& mat);
    
    static void addLight(LightPtr light);
    static void applyLights();
    static void applyPlayerPosition(glm::vec3 worldPlayerPos);

    void render(const glm::mat4& worldMat);
    static void renderInstanced(const glm::mat4& worldMat, std::vector<MeshPtr> meshes, std::vector<glm::vec3> translations, std::vector<glm::vec3> scales);
    const std::vector<glm::vec3>& getLocalCoords() const;
    std::vector<glm::vec3> getWorldCoords(const glm::mat4& worldMat) const;
    
    void reverseNormals();
    Color getColor() const;

private:
    Mesh(){}

    static void applyCommonUniforms(const glm::mat4& worldMat);
};

#endif