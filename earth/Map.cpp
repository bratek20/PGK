#include "Map.h"

#include <iostream>

using namespace std;

Map::Map(const string& /*dataPath*/){

}

Map::Map(const string& dataPath, int wBeg, int wEnd, int lBeg, int lEnd){
    int wSize = wEnd - wBeg;
    int lSize = lEnd - lBeg;
    segments.resize(wSize, vector<MapSegmentPtr>(lSize));

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
            segments[w-wBeg][l-lBeg] = seg;
        }
    }
    center = {(lBeg + lEnd)/2, (wBeg + wEnd)/2};
}
    
void Map::render(){
    for(auto& raw : segments){
        for(auto& seg : raw){
            seg->render(-center, {0.5f, 0.5f});
        }
    }
}