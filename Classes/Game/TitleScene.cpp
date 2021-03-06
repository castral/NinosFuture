
#include "TitleScene.h"
#include "GameStateMachine.h"

#include "Windy/Label.h"
#include "Windy/Armature.h"
#include "Windy/Sprite.h"
#include "Windy/Input.h"

#include "Windy/Display.h"

#include "Windy/AudioManager.h"

class TitleSceneResources {
public:
    static std::string titleSpriteSet;
    static std::string font;

    static cocos2d::Color4B textActiveColor;
    static cocos2d::Color4B textInactiveColor;
};

std::string TitleSceneResources::titleSpriteSet = "sprites/gameplay/screens/title_screen/title_screen";
std::string TitleSceneResources::font = "fonts/megaman_2.ttf";

cocos2d::Color4B TitleSceneResources::textActiveColor = cocos2d::Color4B(255, 255, 255, 255);
cocos2d::Color4B TitleSceneResources::textInactiveColor = cocos2d::Color4B(127, 127, 127, 200);

using namespace game;
using namespace cocos2d;

Scene* TitleScene::scene()
{
    // 'scene' is an autorelease object
    Scene* scene = Scene::create();

    // 'layer' is an autorelease object
    TitleScene* layer = TitleScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}


bool TitleScene::init()
{
    //////////////////////////////
    // 1. super init first
    if (!LayerColor::initWithColor(cocos2d::Color4B(0, 0, 0, 0)))
    {
        return false;
    }

    windy::Armature::clearPlistCache();
    windy::Sprite::clearPlistCache();


    setCascadeOpacityEnabled(true);

    auto fadeIn = FadeIn::create(1.0f);

    //@TODO disable taking inputs before screen fades in
    /*auto audioCallback = CallFunc::create([this]() {

        });*/
    this->runAction(fadeIn);


#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS

#endif

    auto background = windy::Sprite::create(TitleSceneResources::titleSpriteSet, cocos2d::Point(0, 0));
    auto city = windy::Sprite::create(TitleSceneResources::titleSpriteSet, cocos2d::Point(0, 0));
    auto silhouette = windy::Sprite::create(TitleSceneResources::titleSpriteSet);
    auto logo = windy::Sprite::create(TitleSceneResources::titleSpriteSet);
    auto abakura = windy::Sprite::create(TitleSceneResources::titleSpriteSet, cocos2d::Point(0, 0));
    auto greentwip = windy::Sprite::create(TitleSceneResources::titleSpriteSet, cocos2d::Point(0, 0));

    background->setPosition(0, 0);
    addChild(background);

    city->setPosition(0, 0);
    addChild(city);

    silhouette->setPosition(windy::Display::getInstance().width / 2.0f, windy::Display::getInstance().height / 3.25f);
    addChild(silhouette);

    logo->setPosition(windy::Display::getInstance().width / 2.0f, (windy::Display::getInstance().height * 0.25f) * 3);
    addChild(logo);

    abakura->setPosition(0, 0);
    addChild(abakura);

    greentwip->setPosition((float)windy::Display::getInstance().width - greentwip->getContentSize().width * 0.6f, 0);
    addChild(greentwip);


    background->setAnimation("background");
    city->setAnimation("city");
    silhouette->setAnimation("silhouette");
    logo->setAnimation("logo");
    abakura->setAnimation("abakura");
    greentwip->setAnimation("greentwip");

    this->startText = windy::Label::create("START GAME", 
                                           TitleSceneResources::font, 
                                           8, 
                                           cocos2d::TextHAlignment::LEFT,
                                           cocos2d::TextVAlignment::TOP);

    this->optionsText = windy::Label::create("OPTIONS", 
                                           TitleSceneResources::font, 
                                           8, 
                                           cocos2d::TextHAlignment::LEFT,
                                           cocos2d::TextVAlignment::TOP);    

    float startTextWidth = this->startText->getContentSize().width;
    float startTextHeight = this->startText->getContentSize().height;

    float optionsTextWidth = this->optionsText->getContentSize().width;
    float optionsTextHeight = this->optionsText->getContentSize().height;


    this->startText->setPosition(
            cocos2d::Point((windy::Display::getInstance().width * 0.5f) - (startTextWidth * 0.5f),
              (windy::Display::getInstance().height * 0.5f) + (startTextHeight * 2)));

    this->optionsText->setPosition(
            cocos2d::Point((windy::Display::getInstance().width * 0.5f) - (optionsTextWidth * 0.5f),
              (windy::Display::getInstance().height * 0.5f) + (optionsTextHeight * 0.5f)));

    addChild(this->startText, 128);
    addChild(this->optionsText, 128);

    if (windy::AudioManager::getCurrentTrack() != windy::Sounds::Title) {
        windy::AudioManager::playBgm(windy::Sounds::Title);
    }

    this->selectedOption = this->startText;

    selectStart();

    this->triggered = false;

    return true;
}

void TitleScene::selectStart() {
    this->startText->setTextColor(TitleSceneResources::textActiveColor);
    this->optionsText->setTextColor(TitleSceneResources::textInactiveColor);

    this->selectedOption = this->startText;
}   

void TitleScene::selectOptions() {
    this->startText->setTextColor(TitleSceneResources::textInactiveColor);
    this->optionsText->setTextColor(TitleSceneResources::textActiveColor);

    this->selectedOption = this->optionsText;
}

void TitleScene::onEnter()
{
    Layer::onEnter();
    Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
}


void TitleScene::onExit()
{
    Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
    Layer::onExit();
}

void TitleScene::update(float dt)
{
    if (!this->triggered) {

        if (windy::Input::keyPressed(windy::InputKey::Up) || windy::Input::keyPressed(windy::InputKey::Down)) {
            if (this->selectedOption == this->startText) {
                this->selectOptions();
            }
            else {
                this->selectStart();
            }

            windy::AudioManager::playSfx(windy::Sounds::Select);
        }

        if (windy::Input::keyPressed(windy::InputKey::A)) {
            this->triggered = true;
            windy::AudioManager::playSfx(windy::Sounds::Selected);

            auto fadeOut = FadeOut::create(1.0f);
            CallFunc* callback = nullptr;

            if (this->selectedOption == this->startText) {
                callback = CallFunc::create([]() { GameStateMachine::getInstance().pushState(GameState::Save); });
            }
            else {
                callback = CallFunc::create([]() { GameStateMachine::getInstance().pushState(GameState::Options); });
            }

            auto sequence = Sequence::create(fadeOut, callback, nullptr);

            this->runAction(sequence);

        }

    }
}
