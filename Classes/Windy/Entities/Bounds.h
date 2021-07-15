#ifndef __WINDY_BOUNDS_H__
#define __WINDY_BOUNDS_H__

#include "cocos2d.h"

#include "Logical.h"

namespace windy {
    class Bounds : public Logical
    {
    public:
        virtual bool init() override;

        int top();
        int bottom();
        int left();
        int right();

        cocos2d::Point center();


        virtual void parseBehavior(const cocos2d::ValueMap& behavior) override;
        virtual void onUpdate(float dt) override;

    };
}

#endif
