#include "Bounds.h"

#include "./../GameTags.h"

using namespace windy;

bool Bounds::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }


    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
    this->collisionRectangles[0].origin = cocos2d::Point(-visibleSize.width * 0.5f, -visibleSize.height * 0.5f);
    this->collisionRectangles[0].size = visibleSize;

    for (int i = 0; i < this->collisionRectangles.size(); ++i) {
        this->collisionRectangles[i] = Logical::normalizeCollisionRectangle(this, this->collisionRectangles[i]);
    }
    this->setTag(GameTags::General::Bounds);

    return true;
}

int Bounds::top() {
    return static_cast<int>(collisionRectangles[0].getMaxY());
}

int Bounds::bottom() {
    return static_cast<int>(collisionRectangles[0].getMinY());
}

int Bounds::left() {
    return static_cast<int>(collisionRectangles[0].getMinX());
}

int Bounds::right() {
    return static_cast<int>(collisionRectangles[0].getMaxX());
}

cocos2d::Point Bounds::center() {
    return cocos2d::Point(collisionRectangles[0].getMidX(), collisionRectangles[0].getMidY());
}


void Bounds::parseBehavior(const cocos2d::ValueMap& behavior) {

}

void Bounds::onUpdate(float dt) {

}