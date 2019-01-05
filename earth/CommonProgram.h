#ifndef COMMON_PROGRAM_H
#define COMMON_PROGRAM_H

#include "ShaderProgram.h"

#include <glm/glm.hpp>

class CommonProgram : public ShaderProgram {
    GLuint offsetId;
    GLuint ratioId;
    GLuint markedId;

public:
    CommonProgram(){}
    CommonProgram(std::string vsName, std::string fsName);
    
    void setOffset(glm::vec2 offset);
    void setRatio(float ratio);
    void setMarked(bool marked);
};

#endif