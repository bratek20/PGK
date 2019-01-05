#include "CommonProgram.h"

CommonProgram::CommonProgram(std::string vsName, std::string fsName) : ShaderProgram(vsName, fsName) {
    offsetId = glGetUniformLocation(id, "Offset");
    ratioId = glGetUniformLocation(id, "Ratio");
    markedId = glGetUniformLocation(id, "Marked");
}

void CommonProgram::setOffset(glm::vec2 offset){
    glUniform2f(offsetId, offset.x, offset.y);
}

void CommonProgram::setRatio(float ratio){
    glUniform1f(ratioId, ratio);
}

void CommonProgram::setMarked(bool marked){
    glUniform1i(markedId, marked ? 1 : 0);
}

