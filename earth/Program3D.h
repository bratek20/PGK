#ifndef PROGRAM_3D_H
#define PROGRAM_3D_H

#include "ShaderProgram.h"

class Program3D : public ShaderProgram {
public:
    Program3D(){}
    Program3D(std::string vsName, std::string fsName);
};

#endif