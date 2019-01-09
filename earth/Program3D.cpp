#include "Program3D.h"

Program3D::Program3D(std::string vsName, std::string fsName) : CommonProgram(vsName, fsName) {
    VPMatId = glGetUniformLocation(id, "VP");
    radiusId = glGetUniformLocation(id, "Radius");
    emptyId = glGetUniformLocation(id, "Empty");
}

void Program3D::setVPMat(glm::mat4 VPMat){
    glUniformMatrix4fv(VPMatId, 1, GL_FALSE, &VPMat[0][0]);
}

void Program3D::setRadius(float radius){
    glUniform1f(radiusId, radius);
}

void Program3D::setEmpty(bool empty){
    glUniform1i(emptyId, empty ? 1 : 0);
}
