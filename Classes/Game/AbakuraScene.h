#ifndef __ABAKURA_SCENE_H__
#define __ABAKURA_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

namespace windy {
    class Sprite;
}


class AbakuraScene : public cocos2d::LayerColor
{
public:
    virtual bool init();

    static cocos2d::Scene* scene();

    virtual void onEnter();
    virtual void onExit();

    virtual void update(float dt);

    void abakuraIntro();

    CREATE_FUNC(AbakuraScene);

private:
    windy::Sprite* abakuraLogo;
};
#endif
