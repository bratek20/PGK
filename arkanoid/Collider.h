#ifndef COLLIDER_H
#define COLLIDER_H

#include <glm/glm.hpp>
#include <vector>
#include <functional>

class Collider{
public:
    static void collide(const std::vector<glm::vec2>& coords);
private:
    static float cross(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3);
    static bool turnsLeft(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3);
    static void forEach(const std::vector<glm::vec2>& coords, std::function<void(const glm::vec2&, const glm::vec2&)> f);
    static bool inside(const glm::vec2& p, const std::vector<glm::vec2>& coords);
    static std::pair<glm::vec2, glm::vec2> closest(const std::vector<glm::vec2>& coords, const glm::vec2& p);
    static bool intersects(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& q1, const glm::vec2& q2);
    static std::pair<glm::vec2, glm::vec2> collisionEdge(const glm::vec2& prevP, const glm::vec2& curP, const std::vector<glm::vec2>& coords);
};

#endif