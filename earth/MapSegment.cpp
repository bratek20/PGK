#include "MapSegment.h"
#include "DataReader.h"
#include "Window.h"
#include <common/shader.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <iostream>

using namespace std;

const GLuint MapSegment::PRIMITIVE_INDEX=666666666;
GLuint MapSegment::offsetId;
GLuint MapSegment::translateId;
GLuint MapSegment::scaleId;
GLuint MapSegment::ratioId;

GLuint MapSegment::programId;
GLuint MapSegment::vertexArrayIdx;
GLuint MapSegment::indexBufferIdx;

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
    programId =  LoadShaders( "earth2D.vs", "earth2D.fs" ); 
    glUseProgram(programId);

    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(PRIMITIVE_INDEX);

    offsetId = glGetUniformLocation(programId, "Offset");
    translateId = glGetUniformLocation(programId, "Translate");
    scaleId = glGetUniformLocation(programId, "Scale");
    ratioId = glGetUniformLocation(programId, "Ratio");

    glGenVertexArrays(1, &vertexArrayIdx);
	glBindVertexArray(vertexArrayIdx);

    glGenBuffers(1, &indexBufferIdx);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferIdx);
    vector<GLuint> data;
    for(int i=0;i<1200;i++){
        for(int j = 0; j < 1201;j++){
            data.push_back(i*1201 + j);
            data.push_back((i+1)*1201 + j);
        }
	    data.push_back(PRIMITIVE_INDEX);
    }
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * data.size(), data.data(), GL_STATIC_DRAW);

    //glEnable(GL_DEPTH_TEST);
}

void MapSegment::clear(){
    glDisableVertexAttribArray(0);
	
	glDeleteProgram(programId);
	glDeleteVertexArrays(1, &vertexArrayIdx);
}

void MapSegment::render(glm::vec2 translate, float scale){
    glUniform2f(offsetId, offset.x, offset.y);
    glUniform2f(translateId, translate.x, translate.y);
    glUniform1f(scaleId, scale);
    glUniform1f(ratioId, Window::getRatio());

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

    glDrawElements(GL_TRIANGLE_STRIP, 2403 * 1200, GL_UNSIGNED_INT, (void*)0);
}