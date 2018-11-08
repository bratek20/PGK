#include "Collider.h"
#include "Globals.h"
#include "Ball.h"
#include "Globals.h"
#include <algorithm>
#include <iostream>

bool Collider::collide(const std::vector<glm::vec2>& coords){
    auto ball = Globals::ball;
    auto ballCoords = ball->getWorldCoords();
    auto it = std::find_if(ballCoords.begin(), ballCoords.end(), [&](glm::vec2 p){return inside(p, coords);});
    if(it == ballCoords.end()){
        return false;
    }
    
    auto curP = *it;
    auto velocity = ball->getVelocity();
    auto prevP = curP - velocity; // point before collision
    auto colEdge = collisionEdge(prevP, curP, coords);

    auto edge = colEdge.second -  colEdge.first; // anti clockwise!
    auto norm = glm::normalize(glm::vec2(edge.y, -edge.x));
    auto newVelocity = velocity - 2 * glm::dot(velocity, norm) * norm;
    ball->setVelocity(newVelocity);
    ball->move(newVelocity * Globals::deltaTime);

    return true;
}

float Collider::cross(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3){
    auto v1 = p2 - p1;
    auto v2 = p3 - p1;
    return v1.x * v2.y - v1.y * v2.x;
}

bool Collider::turnsLeft(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3){
    return cross(p1, p2, p3) > 0;
}

void Collider::forEach(const std::vector<glm::vec2>& coords, std::function<void(const glm::vec2&, const glm::vec2&)> f){
    int size = static_cast<int>(coords.size());
    for(int i=0;i<size;i++){
        f(coords[i], coords[(i+1)%size]);
    } 
}

bool Collider::inside(const glm::vec2& p, const std::vector<glm::vec2>& coords){
    bool firstAns = turnsLeft(coords[0], coords[1], p);
    bool isInside = true;
    forEach(coords, [&](const glm::vec2& p1, const glm::vec2& p2){
        isInside &= firstAns == turnsLeft(p1, p2, p);
    });
    return isInside;
}

std::pair<glm::vec2, glm::vec2> Collider::closest(const std::vector<glm::vec2>& coords, const glm::vec2& p){
    float bestDist = 1e9;
    std::pair<glm::vec2, glm::vec2> ans;

    forEach(coords, [&](const glm::vec2& p1, const glm::vec2& p2){
        float newDist = glm::distance(p1, p) + glm::distance(p2, p);
        if(newDist < bestDist){
            bestDist = newDist;
            ans = {p1, p2};
        }
    });
    return ans;
}

bool Collider::intersects(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& q1, const glm::vec2& q2){
       return (((q1.x-p1.x)*(p2.y-p1.y) - (q1.y-p1.y)*(p2.x-p1.x))
            * ((q2.x-p1.x)*(p2.y-p1.y) - (q2.y-p1.y)*(p2.x-p1.x)) < 0)
            &&
           (((p1.x-q1.x)*(q2.y-q1.y) - (p1.y-q1.y)*(q2.x-q1.x))
            * ((p2.x-q1.x)*(q2.y-q1.y) - (p2.y-q1.y)*(q2.x-q1.x)) < 0);
}

std::pair<glm::vec2, glm::vec2> Collider::collisionEdge(const glm::vec2& prevP, const glm::vec2& curP, const std::vector<glm::vec2>& coords){
    std::pair<glm::vec2, glm::vec2> ans;
    forEach(coords, [&](const glm::vec2& p1, const glm::vec2& p2){
        if(intersects(prevP, curP, p1, p2)){
            ans = {p1, p2};
        }
    });
    return ans;
}