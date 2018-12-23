#include "MapSegment.h"
#include "DataReader.h"

#include <common/shader.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <iostream>

using namespace std;

GLuint MapSegment::offsetId;
GLuint MapSegment::programId;
GLuint MapSegment::vertexArrayIdx;
std::vector<GLuint> MapSegment::indexBufferIdx;

MapSegment::MapSegment(const std::string& fileName){
    auto heights = DataReader::read(fileName);
    vector<float> data;
    data.reserve(heights.size());
    for(int i=0;i<heights.size();i++){
        float x = static_cast<float>(i % 1201) / 1201;
        float y = heights[i];
        float z = static_cast<float>(i / 1201) / 1201;
        
        data.push_back(x);
        data.push_back(y);
        data.push_back(z);
    }

    glGenBuffers(1, &vertexBufferIdx);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferIdx);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.size(), data.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
	glVertexAttribPointer(
        0,                               
        3,                                       
        GL_FLOAT,                        
        GL_FALSE,                        
        0,                               
        (void*)0                      
    );
}

MapSegment::~MapSegment(){
    glDeleteBuffers(1, &vertexBufferIdx);
}

MapSegmentPtr MapSegment::create(const string& fileName){
    return MapSegmentPtr(new MapSegment(fileName));
}

void MapSegment::init(){
    programId =  LoadShaders( "earth.vs", "earth.fs" ); 
    glUseProgram(programId);

    //MVPId = glGetUniformLocation(programId, "MVP");

    glGenVertexArrays(1, &vertexArrayIdx);
	glBindVertexArray(vertexArrayIdx);

    for(int i=0;i<1200;i++){
        GLuint idx;
        glGenBuffers(1, &idx);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx);
        vector<GLuint> data;
        for(int j = 0; j < 1201;j++){
            data.push_back(i*1201 + j);
            data.push_back((i+1)*1201 + j);
        }
	    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * data.size(), data.data(), GL_STATIC_DRAW);
        indexBufferIdx.push_back(idx);
    }
}

void MapSegment::clear(){
    glDisableVertexAttribArray(0);
	
	glDeleteProgram(programId);
	glDeleteVertexArrays(1, &vertexArrayIdx);
}

void MapSegment::render(){
    for(GLuint idx : indexBufferIdx)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx);
        glDrawElements(GL_TRIANGLE_STRIP, 2402, GL_UNSIGNED_INT, (void*)0);
    }
}