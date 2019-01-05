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

public:    
    static MapSegmentPtr create(const std::string& fileName, int w, int l);
    ~MapSegment();

    static void init(); 
    static void clear();

    static unsigned getIndexSize(int LOD);

    void render(glm::vec2 translate, float scale, int LOD, bool marked);
    void render(glm::mat4 VPMat, float radius, int LOD);

private:
    MapSegment(const std::vector<short>& heights, int w, int l);
    static void addIndexBuffer(int shift);
    static int lodToIdx(int lod);

    void commonRender(CommonProgram& prog, int LOD, bool marked);
};

#endif