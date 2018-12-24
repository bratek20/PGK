#ifndef MAP_SEGMENT_H
#define MAP_SEGMENT_H

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
    glm::vec2 offset;
    GLuint vertexBufferIdx;
    static GLuint offsetId;
    static GLuint translateId;
    static GLuint scaleId;

    static GLuint programId;
    static GLuint vertexArrayIdx;
    static std::vector<GLuint> indexBufferIdx;

public:    
    static MapSegmentPtr create(const std::string& fileName, int w, int l);
    ~MapSegment();

    static void init(); 
    static void clear();

    void render(glm::vec2 translate, glm::vec2 scale);

private:
    MapSegment(const std::vector<short>& heights, int w, int l);
};

#endif