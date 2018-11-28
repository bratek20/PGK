#ifndef SPHERE_GENERATOR_H
#define SPHERE_GENERATOR_H

#include <vector>
#include <glm/glm.hpp>

class SphereGenerator {
public:
    static std::vector<float> generate(int horLines, int vertLines, float radius);

private:
    static glm::vec3 getPoint(int m, int M, int n, int N, float radius);
    static void add(std::vector<float>& coords, const std::vector<glm::vec3>& points);
};
#endif