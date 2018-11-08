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

ActorPtr Shapes::createSubHexagon(float x, float y, float scaleX, float scaleY, float rotation, Mesh::Color color){
    auto subHexagon = Actor::create(nullptr);
    auto leftTri = Actor::create(Mesh::create(Mesh::EQUILATERAL, color));
    auto centerTri = Actor::create(Mesh::create(Mesh::EQUILATERAL, color));
    auto rightTri = Actor::create(Mesh::create(Mesh::EQUILATERAL, color));
    
    centerTri->rotate(180.0f);
    centerTri->setPosition(0, Mesh::EQ_TRI_H/3);
    leftTri->setPosition(-0.5f, 0.0f);
    rightTri->setPosition(0.5f, 0.0f);
    subHexagon->addChilds({leftTri, centerTri, rightTri});

    subHexagon->setPosition(x, y);
    subHexagon->setScale(scaleX, scaleY);
    subHexagon->setRotation(rotation);
    return subHexagon;
}

ActorPtr Shapes::createHexagon(float x, float y, float scaleX, float scaleY, float rotation, Mesh::Color color){
    auto hexagon = Actor::create(nullptr);
    auto top = createSubHexagon(0, Mesh::EQ_TRI_H/3, 1, 1, 0, color);
    auto bottom = createSubHexagon(0, -Mesh::EQ_TRI_H/3, 1, 1, 180, color);
    
    hexagon->addChilds({top, bottom});

    hexagon->setPosition(x, y);
    hexagon->setScale(scaleX, scaleY);
    hexagon->setRotation(rotation);
    return hexagon;
}

ActorPtr Shapes::createWorldFrame(float worldScaleX, float worldScaleY, Mesh::Color color, std::function<void()> endGameCallback){
    auto frame = Actor::create(nullptr);
    const float shiftX = 0.96f;
    const float sizeX = 2*(1-shiftX);
    const float shiftY = 0.95f;
    const float sizeY = 2*(1-shiftY);

    auto left = createRectangle(-shiftX, 0, sizeX, 2.0f, 0, color);
    auto right = createRectangle(shiftX, 0, sizeX, 2.0f, 0, color);
    auto bottom = createRectangle(0, -shiftY, 2.0f, sizeY, 0, color);
    auto top = createRectangle(0, shiftY, 2.0f, sizeY, 0, color);
    
    bottom->setOnCollide([=](ActorPtr){endGameCallback();});
    frame->setScale(1/worldScaleX, 1/worldScaleY);
    frame->addChilds({left, right, bottom, top});      
    return frame;
}

ActorPtr Shapes::createBackground(){
    auto background = Actor::create(nullptr);
    int rows = 30;
    int columns = 30;
    for(int i=0;i<rows;i++){
        for(int j=0;j<columns;j++){
            auto hex = Actor::create(Mesh::create(Mesh::HEXAGON_LINES, Mesh::RAINBOW_HEX));
            float x = (j - columns/2)*2;
            float y = (i - rows/2)*Mesh::EQ_TRI_H*2;
            hex->setPosition(x, y);
            hex->disableCollisions();
            background->addChild(hex);
        }
    }
    background->setScale(0.66f, 0.66f);
    return background;
}

ActorPtr Shapes::createPlatform(float x, float y, float scaleX, float scaleY, Mesh::Color color){
    auto platform = Actor::create(nullptr);
    auto left = createRectangle(-0.5, 0, 1, 1, 30.0f, color);
    auto center = createRectangle(0, 0.0f, 1.0f, 1.0f, 0.0f, color);
    auto right = createRectangle(0.5, 0, 1, 1, -30.0f, color);

    platform->addChilds({left, center, right});
    platform->setPosition(x, y);
    platform->setScale(scaleX, scaleY);
    return platform;
}