#ifndef PROGRAM_2D_H
#define PROGRAM_2D_H

#include "ShaderProgram.h"

#include <glm/glm.hpp>

class Program2D : public ShaderProgram {
    GLuint offsetId;
    GLuint translateId;
    GLuint scaleId;
    GLuint ratioId;
    GLuint markedId;

public:
    Program2D(){}
    Program2D(std::string vsName, std::string fsName);
    
    void setOffset(glm::vec2 offset);
    void setTranslate(glm::vec2 translate);
    void setScale(float scale);
    void setRatio(float ratio);
    void setMarked(bool marked);
};

#endif