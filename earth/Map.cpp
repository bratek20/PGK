#include "Map.h"

#include <iostream>

using namespace std;

Map::Map(const string& /*dataPath*/){

}

Map::Map(const string& dataPath, int wBeg, int wEnd, int lBeg, int lEnd) {
    for(int w = wBeg; w < wEnd; w++){
        for(int l = lBeg; l < lEnd; l++){
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

            auto seg = MapSegment::create(fullPath, w, l);
            if(seg == nullptr){
                cout << "Segment loading failed for path: " << fullPath << endl;
            }
            segments[getKey({l, w})] = seg;
        }
    }

    cam = Camera::create(glm::vec2((lBeg + lEnd)/2, (wBeg + wEnd)/2));
}
    
void Map::render(bool is2D){
    cam->update(is2D);
    auto center = cam->getCenterSegment();
    auto size = cam->getViewSize();
    int LOD = getLOD();
    int segs = 0;
    unsigned verts = 0;
    for(int x = -size.first/2; x <= size.first/2; x++){
        for(int y = -size.second/2; y <= size.second/2; y++){
            auto seg = segments[getKey({x + center.first, y + center.second})];
            if(seg != nullptr){
                if(is2D){
                    seg->render(-cam->getPos2D() * cam->getZoom(), cam->getZoom(), LOD, false);//x==0 && y==0);
                }
                else{
                    seg->render(cam->getVPMat(), Camera::RADIUS, LOD);
                }
                
                segs++;
                verts += MapSegment::getIndexSize(LOD);
            }
        }  
    }

    //cout << "Segments rendered: " << segs << endl;
    //cout << "LOD: " << LOD << endl;
    //cout << "Verts: " << verts << endl;
}

std::string Map::getKey(std::pair<int,int> coords) const {
    return to_string(coords.first) + "," + to_string(coords.second);
}

int Map::getLOD(){
    float zoom = cam->getZoom();
    int lod = 1;
    if(zoom > 1){
        lod = 2;
    }
    if(zoom > 3){
        lod = 3;
    }
    if(zoom > 5){
        lod = 4;
    }
    if(zoom > 7){
        lod = 5;
    }
    return lod;
}