#include "SphereGenerator.h"

#include <cmath>

using namespace std;

static const float PI = 3.14159265f;
glm::vec3 SphereGenerator::getPoint(int m, int M, int n, int N, float radius){
    return { 
    sin(PI * m/M) * cos(2*PI * n/N) * radius,
    sin(PI * m/M) * sin(2*PI * n/N) * radius,
    cos(PI * m/M) * radius };
}

void SphereGenerator::add(std::vector<float>& coords, const std::vector<glm::vec3>& points){
    for(auto p : points){
        coords.push_back(p.x);
        coords.push_back(p.y);
        coords.push_back(p.z);
    }
}

vector<float> SphereGenerator::generate(int horLines, int vertLines, float radius){
    int M = horLines-1;
    int N = vertLines; 

    vector<float> coords;
    for(int m=1; m<=M; m++){
        for(int n=0; n<N; n++){
            auto getP = [&](int dm, int dn){
                return getPoint(m+dm, M, n+dn, N, radius);
            };
            add(coords, {getP(0,0), getP(-1,0), getP(-1,-1)});
            add(coords, {getP(0,0), getP(-1,-1), getP(0,-1)});
        }
    }

    return coords;
}