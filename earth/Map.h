#ifndef MAP_H
#define MAP_H

#include "MapSegment.h"
#include "Camera.h"

#include <unordered_map>
#include <string>

class Map {
    CameraPtr cam;
    std::unordered_map<std::string, MapSegmentPtr> segments;

public:   
    Map() {}
    Map(const std::string& dataPath);
    Map(const std::string& dataPath, int wBeg, int wEnd, int lBeg, int lEnd);
    
    void render();

private:
    std::string getKey(std::pair<int,int> coords) const;
    int getLOD();
};

#endif