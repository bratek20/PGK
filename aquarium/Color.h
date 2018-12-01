#ifndef COLOR_H
#define COLOR_H

#include <GL/glew.h>

struct Color
{
    float r;
    float g;
    float b;

    bool operator==(const Color& other)
    {
        return r == other.r && g == other.g && b == other.b;
    }

    void apply(GLuint uniformId);
};

namespace Colors{
    constexpr Color WHITE = {1.0f, 1.0f, 1.0f};
    constexpr Color BLACK = {0.0f, 0.0f, 0.0f};
    constexpr Color GREY = {0.75f, 0.75f, 0.75f};
    constexpr Color BROWN = {1.0f, 0.9f, 0.8f};

    constexpr Color RED = {1.0f, 0.0f, 0.0f};
    constexpr Color GREEN = {0.0f, 1.0f, 0.0f};
    constexpr Color BLUE = {0.0f, 0.0f, 1.0f};
    constexpr Color YELLOW = {1.0f, 1.0f, 0.0f};
    constexpr Color MAGENTA = {1.0f, 0.0f, 1.0f};
    constexpr Color CYAN = {0.0f, 1.0f, 1.0f};
}


#endif
