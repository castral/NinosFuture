#include <memory>
#include <string>

#include "Enemy.h"

#include "Player.h"
#include "Bullet.h"

#include "./../GameTags.h"
#include "./../Sprite.h"

using namespace windy;

std::random_device Enemy::itemRandomDevice;
std::default_random_engine  Enemy::itemRandomEngine(Enemy::itemRandomDevice());

bool Enemy::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Node::init())
    {
        return false;
    }

    this->power = 4;
    this->health = 15;
    this->maxHealth = 15;

    this->isFlipping = false;

    this->setup();

    this->setTag(GameTags::General::Enemy);

    return true;
}


void Enemy::parseBehavior(const cocos2d::ValueMap& behavior) {

}

void Enemy::setOrientation() {
    if (!this->isFlipping) {
        if (this->level->player->getPositionX() <= this->getPositionX()) {
            if (this->sprite->isFlippedX()) {
                this->sprite->setFlippedX(false);
                this->isFlipping = false;
            }            
        }
        else {
            if (!this->sprite->isFlippedX()) {
                this->sprite->setFlippedX(true);
                this->isFlipping = false;
            }
        }
    }
}


int Enemy::getSpriteNormal() {

    if (this->sprite->isFlippedX()) {
        return -1;
    }
    else {
        return 1;
    }
}

void Enemy::checkHealth() {
    if (this->health <= 0) {
        this->health = 0;
        this->onDefeated();
    }
}


void Enemy::onCollisionEnter(Logical* collision) {

    if (collision->getTag() == GameTags::Weapon::WeaponPlayer) {
        
        
        AudioManager::playSfx(Sounds::EnemyHit);
        
        auto blink = cocos2d::Blink::create(0.2f, 4);
        auto callback = cocos2d::CallFunc::create([this]() {
            
            if (!this->sprite->isVisible()) {
                this->sprite->setVisible(true);
            }

        });

        auto sequence = cocos2d::Sequence::create(blink, callback, nullptr);

        ((cocos2d::Node*)this->sprite)->runAction(sequence);

        auto bullet = dynamic_cast<Bullet*>(collision);

        this->health -= bullet->power;

        if (this->health <= 0) {
            this->onDefeated();
        }

        if (bullet->power < 3) {
            bullet->finish();
        }


    }


}

void Enemy::onUpdate(float dt) {
    this->setOrientation();
    this->attack();
    this->checkHealth();
}