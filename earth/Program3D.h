#ifndef PROGRAM_3D_H
#define PROGRAM_3D_H

#include "CommonProgram.h"
#include <glm/glm.hpp>

class Program3D : public CommonProgram {
    GLuint VPMatId;
    GLuint radiusId;
    GLuint emptyId;

public:
    Program3D(){}
    Program3D(std::string vsName, std::string fsName);

    void setVPMat(glm::mat4 VPMat);
    void setRadius(float radius);
    void setEmpty(bool empty);
};

#endif