#include "Map.h"
#include "Globals.h"

#include <iostream>
#include <experimental/filesystem>
#include <regex>

using namespace std;
namespace fs = std::experimental::filesystem::v1;

Map::Map(const string& dataPath) : dataPath(dataPath), lazyLoad(true){
    initCam(calcDataCenter());
}

Map::Map(const std::string& dataPath, int wStart, int lStart) : dataPath(dataPath), lazyLoad(true){
    initCam({lStart, wStart});
}

Map::Map(const string& dataPath, int wBeg, int wEnd, int lBeg, int lEnd) : dataPath(dataPath), lazyLoad(false){
    for(int w = wBeg; w < wEnd; w++){
        for(int l = lBeg; l < lEnd; l++){
            segments[getKey({l, w})] = createSegment(w, l);
        }
    }
    initCam(glm::vec2((lBeg + lEnd)/2, (wBeg + wEnd)/2));
}

void Map::initCam(glm::vec2 center){
    cam = Camera::create(center);
    cosY = cos(glm::radians(center.y));
}
    
void Map::render(bool is2D){
    cam->update(is2D);
    auto center = cam->getCenterSegment();
    auto size = cam->getViewSize(cosY);
    int LOD = Globals::getLOD();
    
    int bx = -size.first/2;
    int ex = size.first/2;
    int by = -size.second/2;
    int ey = size.second/2;
    if(!is2D){
        bx = by = -3;
        ex = ey = 3;
    }

    for(int x = bx; x <= ex; x++){
        for(int y = by; y <= ey; y++){
            int l = x + center.first;
            int w = y + center.second;
            string key = getKey({l, w});
            MapSegmentPtr seg = segments[key];
            if(seg == nullptr){
                if(lazyLoad){
                    seg = segments[key] = createSegment(w, l);
                }
                else{
                    seg = segments[key] = createEmptySegment(w, l);
                }
            }

            if(is2D){
                Globals::renderedVertices += seg->render(-cam->getPos2D() * cam->getZoom(), cam->getZoom(), LOD, cosY);            }
            else{
                Globals::renderedVertices += seg->render(cam->getVPMat(), Camera::RADIUS, LOD, cosY);
            }
        }  
    }
}

glm::vec2 Map::calcDataCenter(){
    int lSum = 0;
    int wSum = 0;
    int cnt = 0;
    std::regex rgx(".*([NS])(\\d+)([EW])(\\d+).*");
    
    for (const auto& entry : fs::directory_iterator(dataPath)){
        string path = entry.path();
        std::smatch matches;
        if(std::regex_search(path, matches, rgx)) {
            string NS = matches[1].str();
            int w = stoi(matches[2].str());
            if(NS == "S"){
                w *= -1;
            }

            string EW = matches[3].str();
            int l = stoi(matches[4].str());
            if(EW == "W"){
                l *= -1;
            }
            wSum += w;
            lSum += l;
            cnt++;            
        }
    }   
    return glm::vec2(static_cast<float>(lSum)/cnt, static_cast<float>(wSum)/cnt);
}

std::string Map::getKey(std::pair<int,int> coords) const {
    return to_string(coords.first) + "," + to_string(coords.second);
}

MapSegmentPtr Map::createSegment(int w, int l){
    string wPref = w >= 0 ? "N" : "S";
    string lPref = l >= 0 ? "E" : "W";
    int aw = abs(w);
    int al = abs(l);
    if(aw < 10){
        wPref += "0";
    }
    if(al < 10){
        lPref += "0";
    }
    if(al < 100){
        lPref += "0";
    }

    string fileName = wPref + to_string(aw) + lPref + to_string(al) + ".hgt";
    string fullPath = dataPath + fileName;
    cout << "Loading " << fullPath << endl;
    return MapSegment::create(fullPath, w, l);
}

MapSegmentPtr Map::createEmptySegment(int w, int l){
    return MapSegment::create(w, l);
}