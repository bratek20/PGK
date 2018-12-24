#ifndef MAP_H
#define MAP_H

#include "MapSegment.h"

#include <vector>
#include <string>

class Map {
    std::vector<std::vector<MapSegmentPtr>> semgents;

public:   
    Map() {}
    Map(std::string dataPath);
    Map(std::string dataPath, int wBeg, int wEnd, int lBeg, int lEnd);
    
    void render();
};

#endif