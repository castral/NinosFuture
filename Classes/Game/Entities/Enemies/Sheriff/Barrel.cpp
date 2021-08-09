#include "Barrel.h"

#include "Windy/Armature.h"
#include "Windy/AnimationAction.h"
#include "Windy/Sprite.h"

#include "Windy/GeometryExtensions.h"

#include "Game/Entities/Player/GamePlayer.h"

using namespace game;


class BarrelResources {
public:
    static std::string spritePath;
    static std::string armaturePath;
};

std::string BarrelResources::spritePath = "sprites/characters/enemy/sheriff/barrel/barrel";
std::string BarrelResources::armaturePath = "physics/characters/enemy/sheriff/barrel/barrel";

void Barrel::preloadResources() {
    windy::Armature::cache(BarrelResources::armaturePath);
    windy::Sprite::cache(BarrelResources::spritePath);
}


void Barrel::setup() {

    this->power = 4;

    this->maxHealth = 24;
    this->health = this->maxHealth;

    this->attackState = AttackState::Scanning;

    this->walkSpeed = 1;

    Enemy::composite(this, BarrelResources::armaturePath, BarrelResources::spritePath, "barrel");

    std::vector<windy::AnimationAction> actionSet = {
        windy::AnimationAction("still",      "barrel_still",      true,   0.10f),
        windy::AnimationAction("stand",      "barrel_stand",      false,   0.10f),
        windy::AnimationAction("walk",       "barrel_walk",       true,   0.10f)
        
    };

    this->sprite->appendActionSet(actionSet, false);

    this->sprite->setAnimation("barrel_still");

    this->isStandReversed = false;

    this->standTimer = 0;
    this->walkTimer = 0;
    this->cooldownTimer = 0;

    this->standTimeInterval = this->sprite->getActionDuration("stand") * 2;
    this->walkTimeInterval = this->sprite->getActionDuration("walk");
    this->cooldownTimeInterval = 1;

}


std::shared_ptr<cocos2d::Rect> Barrel::getEntryCollisionRectangle(const cocos2d::Point& position, const cocos2d::Size& size) {
    return Enemy::buildEntryCollisionRectangle(position, size, BarrelResources::armaturePath, "barrel");
}

void Barrel::attack() {
    switch (this->attackState) {

        case AttackState::Scanning: {

            auto& playerCollisionBox = *this->level->player->collisionBox;
            auto projectedCollisionBoxRight = *this->collisionBox;
            auto projectedCollisionBoxLeft = *this->collisionBox;

            projectedCollisionBoxRight.size.width = 128;

            projectedCollisionBoxLeft.origin.x -= 128;
            projectedCollisionBoxLeft.size.width = 128;

            if (windy::GeometryExtensions::rectIntersectsRect(playerCollisionBox, projectedCollisionBoxRight) ||
                windy::GeometryExtensions::rectIntersectsRect(playerCollisionBox, projectedCollisionBoxLeft)) {
                this->sprite->runAction("stand");

                if (isStandReversed) {
                    this->sprite->reverseAction();
                }

                this->standTimer = this->standTimeInterval;
                this->attackState = AttackState::Standing;
            }

        }
        break;

        case AttackState::Standing: {
            if (this->standTimer <= 0) {
                this->sprite->runAction("walk");
                this->walkTimer = this->walkTimeInterval;
                this->attackState = AttackState::Walking;
            }
            else {
                this->standTimer -= 1.0f / 60.0f;
            }
        }
        break;

        case AttackState::Walking: {

            float playerDistance = this->getPosition().getDistance(this->level->player->getPosition());

            if (playerDistance > 32) {
                this->speed.x = static_cast<float>(this->walkSpeed * this->getSpriteNormal()) * -1;
            }
            

            if (this->walkTimer <= 0) {
                this->speed.x = 0;

                this->sprite->runAction("stand");
                this->sprite->reverseAction();
                this->isStandReversed = true;

                this->attackState = AttackState::Sitting;

                this->standTimer = this->standTimeInterval;
            }
            else {
                this->walkTimer -= 1.0f / 60.0f;
            }

        }
        break;

        case AttackState::Sitting:
        {
            if (this->standTimer <= 0) {
                this->sprite->runAction("still");
                this->cooldownTimer = this->cooldownTimeInterval;
                this->attackState = AttackState::CoolDown;
            }
            else {
                this->standTimer -= 1.0f / 60.0f;
            }
        }
        break;

        case AttackState::CoolDown: {
            if (this->cooldownTimer <= 0) {
                this->attackState = AttackState::Scanning;
            }
            else {
                this->cooldownTimer -= 1.0f / 60.0f;
            }

        }
        break;

    }
}