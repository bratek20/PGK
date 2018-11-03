#include "Shapes.h"

ActorPtr Shapes::createRectangle(float x, float y, float scaleX, float scaleY, float rotation, Mesh::Color color){
    auto rectangle = Actor::create(nullptr);
    auto leftTri = Actor::create(Mesh::create(Mesh::RIGHT, color));
    auto rightTri = Actor::create(Mesh::create(Mesh::RIGHT, color));
    rightTri->setRotation(180.0f);
    rectangle->addChilds({leftTri, rightTri});

    rectangle->setPosition(x, y);
    rectangle->setScale(scaleX, scaleY);
    rectangle->setRotation(rotation);
    return rectangle;
}

ActorPtr Shapes::createWorldFrame(float worldScaleX, float worldScaleY, Mesh::Color color){
    auto frame = Actor::create(nullptr);
    const float shiftX = 0.96f;
    const float sizeX = 2*(1-shiftX);
    const float shiftY = 0.95f;
    const float sizeY = 2*(1-shiftY);

    auto left = createRectangle(-shiftX, 0, sizeX, 1.0f, 0, color);
    auto right = createRectangle(shiftX, 0, sizeX, 1.0f, 0, color);
    auto bottom = createRectangle(0, -shiftY, 1.0f, sizeY, 0, color);
    auto top = createRectangle(0, shiftY, 1.0f, sizeY, 0, color);
    
    frame->setScale(1/worldScaleX, 1/worldScaleY);
    frame->addChilds({left, right, bottom, top});      
    return frame;
}

ActorPtr Shapes::createPlatform(float x, float y, float scaleX, float scaleY, Mesh::Color color){
    auto platform = Actor::create(nullptr);
    auto left = createRectangle(-1, 0, 1, 1, 30.0f, color);
    auto center = createRectangle(0, 0.0f, 1.0f, 1.0f, 0.0f, color);
    auto right = createRectangle(1, 0, 1, 1, -30.0f, color);

    platform->addChilds({left, center, right});
    platform->setPosition(x, y);
    platform->setScale(scaleX, scaleY);
    return platform;
}