#include "MapSegment.h"
#include "DataReader.h"
#include "Window.h"

#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <iostream>

using namespace std;

const GLuint MapSegment::PRIMITIVE_INDEX=666666666;
GLuint MapSegment::EMPTY_VERTEX_IDX=0;

Program2D MapSegment::prog2D;
Program3D MapSegment::prog3D;
GLuint MapSegment::vertexArrayIdx;
std::vector<GLuint> MapSegment::indexBufferIdx;
std::vector<GLuint> MapSegment::indexBufferSizes;

MapSegment::MapSegment(const vector<short>& heights, int w, int l) : offset(l, w){
    if(heights.size() != 1201 * 1201){
        empty = true;
        vertexBufferIdx = EMPTY_VERTEX_IDX;
    }
    else{
        empty = false;
        vector<float> data;
        data.reserve(heights.size());
        for(unsigned i=0;i<heights.size();i++){
            float y = heights[i];
            data.push_back(y);
        }

        glGenBuffers(1, &vertexBufferIdx);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferIdx);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size(), data.data(), GL_STATIC_DRAW);
    }
}

MapSegment::~MapSegment(){
    if(!empty){
        glDeleteBuffers(1, &vertexBufferIdx);
    }
}

MapSegmentPtr MapSegment::create(int w, int l){
    vector<short> heights;
    return MapSegmentPtr(new MapSegment(heights, w, l));
}

MapSegmentPtr MapSegment::create(const string& fileName, int w, int l){
    auto heights = DataReader::read(fileName);
    return MapSegmentPtr(new MapSegment(heights, w, l));
}

void MapSegment::init(){
    prog2D = Program2D("earth2D.vs", "earth.fs");
    prog3D = Program3D("earth3D.vs", "earth.fs");

    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(PRIMITIVE_INDEX);

    glGenVertexArrays(1, &vertexArrayIdx);
	glBindVertexArray(vertexArrayIdx);

    addIndexBuffer(1);
    addIndexBuffer(2);
    addIndexBuffer(4);
    addIndexBuffer(8);
    addIndexBuffer(16);

    vector<float> data(1201*1201, 0);
    glGenBuffers(1, &EMPTY_VERTEX_IDX);
    glBindBuffer(GL_ARRAY_BUFFER, EMPTY_VERTEX_IDX);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size(), data.data(), GL_STATIC_DRAW);

    glEnable(GL_DEPTH_TEST);
}

void MapSegment::addIndexBuffer(int shift){
    GLuint idx;
    glGenBuffers(1, &idx);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx);

    vector<GLuint> data;
    for(int i=0;i<1200;i+=shift){
        for(int j = 0; j < 1201;j+=shift){
            data.push_back(i*1201 + j);
            data.push_back((i+shift)*1201 + j);
        }
	    data.push_back(PRIMITIVE_INDEX);
    }

    indexBufferIdx.push_back(idx);
    indexBufferSizes.push_back(data.size());
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * data.size(), data.data(), GL_STATIC_DRAW);
}

void MapSegment::clear(){
    prog2D.clear();
    glDisableVertexAttribArray(0);
    glDeleteBuffers(1, &EMPTY_VERTEX_IDX);
	glDeleteVertexArrays(1, &vertexArrayIdx);
}

unsigned MapSegment::render(glm::vec2 translate, float scale, int LOD, float ratioMult){
    if(empty){
        return 0;
    }

    prog2D.use();
    prog2D.setTranslate(translate);
    prog2D.setScale(scale);

    return commonRender(prog2D, LOD, ratioMult);
}

unsigned MapSegment::render(glm::mat4 VPMat, float radius, int LOD, float ratioMult){
    prog3D.use();
    prog3D.setVPMat(VPMat);
    prog3D.setRadius(radius);
    prog3D.setEmpty(empty);

    return commonRender(prog3D, empty ? 1 : LOD, ratioMult);
}

unsigned MapSegment::commonRender(CommonProgram& prog, int LOD, float ratioMult){
    prog.setOffset(offset);
    prog.setRatio(Window::getRatio() * ratioMult);
    prog.setMarked(false);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferIdx);
    glEnableVertexAttribArray(0);
	glVertexAttribPointer(
        0,                               
        1,                                       
        GL_FLOAT,                        
        GL_FALSE,                        
        0,                               
        (void*)0                      
    );

    int idx = lodToIdx(LOD); 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferIdx[idx]);
    glDrawElements(GL_TRIANGLE_STRIP, indexBufferSizes[idx], GL_UNSIGNED_INT, (void*)0);
    return indexBufferSizes[idx];
}

int MapSegment::lodToIdx(int LOD){
    return 5 - LOD;
}