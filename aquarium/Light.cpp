#include "Light.h"

Light::Light() : Actor(nullptr) {
}

LightPtr Light::create(){
    return LightPtr(new Light());
}