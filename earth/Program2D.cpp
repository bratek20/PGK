#include "Program2D.h"

Program2D::Program2D(std::string vsName, std::string fsName) : ShaderProgram(vsName, fsName) {
    offsetId = glGetUniformLocation(id, "Offset");
    translateId = glGetUniformLocation(id, "Translate");
    scaleId = glGetUniformLocation(id, "Scale");
    ratioId = glGetUniformLocation(id, "Ratio");
    markedId = glGetUniformLocation(id, "Marked");
}

void Program2D::setOffset(glm::vec2 offset){
    glUniform2f(offsetId, offset.x, offset.y);
}

void Program2D::setTranslate(glm::vec2 translate){
    glUniform2f(translateId, translate.x, translate.y);
}

void Program2D::setScale(float scale){
    glUniform1f(scaleId, scale);
}

void Program2D::setRatio(float ratio){
    glUniform1f(ratioId, ratio);
}

void Program2D::setMarked(bool marked){
    glUniform1i(markedId, marked ? 1 : 0);
}

