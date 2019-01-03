#include "MapSegment.h"
#include "DataReader.h"
#include "Window.h"

#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <iostream>

using namespace std;

const GLuint MapSegment::PRIMITIVE_INDEX=666666666;

Program2D MapSegment::prog2D;
Program3D MapSegment::prog3D;
GLuint MapSegment::vertexArrayIdx;
std::vector<GLuint> MapSegment::indexBufferIdx;
std::vector<GLuint> MapSegment::indexBufferSizes;

MapSegment::MapSegment(const vector<short>& heights, int w, int l) : offset(l, w){
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

MapSegment::~MapSegment(){
    glDeleteBuffers(1, &vertexBufferIdx);
}

MapSegmentPtr MapSegment::create(const string& fileName, int w, int l){
    auto heights = DataReader::read(fileName);
    if(heights.size() != 1201 * 1201){
        cout << "Heights size for " << fileName << " is " << heights.size() << endl;
        return nullptr;
    }
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

    //glEnable(GL_DEPTH_TEST);
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
	glDeleteVertexArrays(1, &vertexArrayIdx);
}

void MapSegment::render(glm::vec2 translate, float scale, int LOD, bool marked){
    prog2D.use();
    prog2D.setOffset(offset);
    prog2D.setTranslate(translate);
    prog2D.setScale(scale);
    prog2D.setMarked(marked);
    prog2D.setRatio(Window::getRatio());

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
    glDrawElements(GL_LINE_STRIP, indexBufferSizes[idx], GL_UNSIGNED_INT, (void*)0);
}

unsigned MapSegment::getIndexSize(int LOD){
    return indexBufferSizes[lodToIdx(LOD)];
}

int MapSegment::lodToIdx(int LOD){
    return 5 - LOD;
}