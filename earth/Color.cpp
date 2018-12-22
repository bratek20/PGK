#include "Color.h"
#include "Globals.h"

void Color::apply(GLuint uniformId){
    glUniform3f(uniformId, r, g, b);
}

Color Color::random(){
    return {Globals::random(), Globals::random(), Globals::random()};
}