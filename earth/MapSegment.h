#ifndef MAP_SEGMENT_H
#define MAP_SEGMENT_H

#include "Program2D.h"
#include "Program3D.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <vector>

class MapSegment;
using MapSegmentPtr = std::shared_ptr<MapSegment>;

class MapSegment {
    static const GLuint PRIMITIVE_INDEX;
    static GLuint EMPTY_VERTEX_IDX;
    static GLuint offsetId;
    static GLuint translateId;
    static GLuint scaleId;
    static GLuint ratioId;
    static GLuint markedId;

    static Program2D prog2D;
    static Program3D prog3D;
    static GLuint vertexArrayIdx;
    static std::vector<GLuint> indexBufferIdx;
    static std::vector<GLuint> indexBufferSizes;

    glm::vec2 offset;
    GLuint vertexBufferIdx;
    bool empty;

public:    
    static MapSegmentPtr create(int w, int l);
    static MapSegmentPtr create(const std::string& fileName, int w, int l);
    ~MapSegment();

    static void init(); 
    static void clear();

    unsigned render(glm::vec2 translate, float scale, int LOD, float ratioMult);
    unsigned render(glm::mat4 VPMat, float radius, int LOD, float ratioMult, float heightScale);

private:
    MapSegment(const std::vector<short>& heights, int w, int l);
    static void addIndexBuffer(int shift);
    static int lodToIdx(int lod);

    unsigned commonRender(CommonProgram& prog, int LOD, float ratioMult);
};

#endif