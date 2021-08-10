#ifndef __GAME_SHERIFF_MAN_H__
#define __GAME_SHERIFF_MAN_H__

#include "cocos2d.h"

#include "GameBoss.h"

namespace game {
    class SheriffMan : public GameBoss
    {
    public:
        virtual void onSpawn() override;

        virtual void switchToBattleBrowner() override;

        virtual void onUpdate(float dt) override;
    };
}

#endif
