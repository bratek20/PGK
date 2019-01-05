#include "Program2D.h"

Program2D::Program2D(std::string vsName, std::string fsName) : CommonProgram(vsName, fsName){
    translateId = glGetUniformLocation(id, "Translate");
    scaleId = glGetUniformLocation(id, "Scale");
}

void Program2D::setTranslate(glm::vec2 translate){
    glUniform2f(translateId, translate.x, translate.y);
}

void Program2D::setScale(float scale){
    glUniform1f(scaleId, scale);
}
