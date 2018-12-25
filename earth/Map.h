#ifndef MAP_H
#define MAP_H

#include "MapSegment.h"
#include "Camera.h"

#include <vector>
#include <string>

class Map {
    CameraPtr cam;
    std::vector<std::vector<MapSegmentPtr>> segments;

public:   
    Map() {}
    Map(const std::string& dataPath);
    Map(const std::string& dataPath, int wBeg, int wEnd, int lBeg, int lEnd);
    
    void render();
};

#endif