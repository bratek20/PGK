#ifndef MAP_H
#define MAP_H

#include "MapSegment.h"
#include "Camera.h"

#include <unordered_map>
#include <string>

class Map {
    CameraPtr cam;
    std::unordered_map<std::string, MapSegmentPtr> segments;
    std::string dataPath;
    bool lazyLoad;
    float cosY;

public:   
    Map() {}
    Map(const std::string& dataPath);
    Map(const std::string& dataPath, int wStart, int lStart);
    Map(const std::string& dataPath, int wBeg, int wEnd, int lBeg, int lEnd);

    void render(bool is2D);

private:
    void initCam(glm::vec2 center);
    glm::vec2 calcDataCenter();
    std::string getKey(std::pair<int,int> coords) const;
    MapSegmentPtr createSegment(int w, int l);
    MapSegmentPtr createEmptySegment(int w, int l);
};

#endif