#include "Color.h"

void Color::apply(GLuint uniformId){
    glUniform3f(uniformId, r, g, b);
}