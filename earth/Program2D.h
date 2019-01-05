#ifndef PROGRAM_2D_H
#define PROGRAM_2D_H

#include "CommonProgram.h"

#include <glm/glm.hpp>

class Program2D : public CommonProgram {
    GLuint translateId;
    GLuint scaleId;

public:
    Program2D(){}
    Program2D(std::string vsName, std::string fsName);

    void setTranslate(glm::vec2 translate);
    void setScale(float scale);
};

#endif