#ifndef MAP_H
#define MAP_H

#include "MapSegment.h"

#include <vector>
#include <string>

class Map {
    glm::vec2 center;
    glm::vec2 translate;
    float scale;
    std::vector<std::vector<MapSegmentPtr>> segments;

public:   
    Map() {}
    Map(const std::string& dataPath);
    Map(const std::string& dataPath, int wBeg, int wEnd, int lBeg, int lEnd);
    
    void render();
};

#endif