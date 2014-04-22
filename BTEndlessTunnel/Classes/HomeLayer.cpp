//
//  HomeLayer.cpp
//  BTEndlessTunnel
//
//  Created by NSS on 4/1/14.
//
//

#include "HomeLayer.h"
#include "SettingsLayer.h"
#include "NativeUtils.h"
#include "Constants.h"
#include "SimpleAudioEngine.h"
#include "PlayGameConstants.h"
#include "LocalStorageManager.h"

using namespace cocos2d;
using namespace CocosDenshion;

HomeLayer::HomeLayer(GameLayer* gameLayer) : _gameLayer(gameLayer)
{
    disable = false;
        
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint visibleOrigin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    // Tablero Izq.
    CCSprite* tablero = CCSprite::create("tablero_title.png");
    tablero->setAnchorPoint(CCPointZero);
    tablero->setPositionX(visibleOrigin.x);
    tablero->setPositionY(visibleOrigin.y);
    addChild(tablero);
    
    // Pantalla
    CCSprite* pantalla = CCSprite::create("pantalla_0001.png");
    pantalla->setPositionY(visibleSize.height - pantalla->getContentSize().height * 0.5f);
    pantalla->setPositionX(visibleOrigin.x + pantalla->getContentSize().width * 0.5f);
    
    // Crear logo
    CCSprite* logo = CCSprite::create("logo.png");
    logo->setPositionX(visibleOrigin.x + visibleSize.width * 0.7f);
    logo->setPositionY(visibleOrigin.y + visibleSize.height * 0.65f);
    addChild(logo);
    
    
    CCAnimation* animation = CCAnimation::create();
    for(int i = 1; i <= 5; i++ )
    {
        CCString* str = CCString::createWithFormat("pantalla_000%d.png", i);
        animation->addSpriteFrameWithFileName(str->getCString());
    }
    animation->setDelayPerUnit(1.0 / 12.0f);
    animation->setRestoreOriginalFrame(false);
    pantalla->runAction(CCRepeatForever::create(CCAnimate::create(animation)));
    tablero->addChild(pantalla);
    
    // Easy Mode
    CCMenuItemImage* menuItemEasy = CCMenuItemImage::create("easy.png", "easy.png", this, menu_selector(HomeLayer::_onOptionPressed));
    menuItemEasy->setTag(kTagEasyMode);
    menuItemEasy->setAnchorPoint(ccp(0, 0));
    menuItemEasy->setPosition(ccp(visibleOrigin.x + menuItemEasy->getContentSize().width * 0.1f, visibleOrigin.y + menuItemEasy->getContentSize().height * 3.2));
    
    // Normal Mode
    CCMenuItemImage* menuItemNormal = CCMenuItemImage::create("medium.png", "medium.png", this, menu_selector(HomeLayer::_onOptionPressed));
    menuItemNormal->setTag(kTagNormalMode);
    menuItemNormal->setAnchorPoint(ccp(0, 0));
    menuItemNormal->setPosition(ccp(menuItemEasy->getPositionX(), menuItemEasy->getPositionY() - menuItemEasy->getContentSize().height * 1.1f));
    
    // Hard Mode
    CCMenuItemImage* menuItemHard = CCMenuItemImage::create("hard.png", "hard.png", this, menu_selector(HomeLayer::_onOptionPressed));
    menuItemHard->setTag(kTagHardMode);
    menuItemHard->setAnchorPoint(ccp(0, 0));
    menuItemHard->setPosition(ccp(menuItemNormal->getPositionX(), menuItemNormal->getPositionY() - menuItemEasy->getContentSize().height * 1.1f));
    
    
    // Achievements
    CCMenuItemImage* menuItemAchievements = CCMenuItemImage::create("achievement.png", "achievement.png", this, menu_selector(HomeLayer::_onOptionPressed));
    menuItemAchievements->setTag(kTagAchievements);
    menuItemAchievements->setPositionX(menuItemHard->getPositionX() + menuItemAchievements->getContentSize().width * 0.9f);
    menuItemAchievements->setPositionY(menuItemHard->getPositionY() - menuItemAchievements->getContentSize().height * 0.65f);
    
    // Leaderboards
    CCMenuItemImage* menuItemLeaderboard = CCMenuItemImage::create("chart.png", "chart.png", this, menu_selector(HomeLayer::_onOptionPressed));
    menuItemLeaderboard->setTag(kTagLeaderboard);
    menuItemLeaderboard->setPositionX(menuItemAchievements->getPositionX() + menuItemAchievements->getContentSize().width * 1.1f);
    menuItemLeaderboard->setPositionY(menuItemAchievements->getPositionY());
    
    // Settings
    CCMenuItemImage* menuItemSettings = CCMenuItemImage::create("ajustes.png", "ajustes.png", this, menu_selector(HomeLayer::_onOptionPressed));
    menuItemSettings->setTag(kTagSettings);
    menuItemSettings->setPosition(ccp(visibleSize.width - menuItemSettings->getContentSize().width * 0.8f, visibleOrigin.y + menuItemSettings->getContentSize().height * 0.7f));
    
    // Rate App
    CCLabelTTF* lblRateApp = CCLabelTTF::create("Rate this App!", FONT_GAME, 35.0f);
    lblRateApp->setColor(ccWHITE);
    
    CCMenuItemLabel* menuRateApp = CCMenuItemLabel::create(lblRateApp, this, menu_selector(HomeLayer::_onOptionPressed));
    menuRateApp->setTag(kTagRateApp);
    menuRateApp->setPositionX(visibleOrigin.x + visibleSize.width * 0.7f);
    menuRateApp->setPositionY(visibleOrigin.y + menuRateApp->getContentSize().height * 1.6f);
    
    // Sound management
    CCMenuItemImage* menuSoundOn = CCMenuItemImage::create("snd_on.png", "snd_on.png", NULL, NULL);
    CCMenuItemImage* menuSoundOff = CCMenuItemImage::create("snd_off.png", "snd_off.png", NULL, NULL);
    
    CCMenuItemToggle* menuSound = CCMenuItemToggle::createWithTarget(this, menu_selector(HomeLayer::_manageMusic), menuSoundOn, menuSoundOff, NULL);
    menuSound->setPositionX(menuItemLeaderboard->getPositionX() + menuSoundOn->getContentSize().width * 1.8f);
    menuSound->setPositionY(menuItemLeaderboard->getPositionY());
    
    if(LocalStorageManager::isMute())
        menuSound->setSelectedIndex(1);
    
    // Menu
    CCMenu* menu = CCMenu::create();
    menu->setPosition(CCPointZero);
    menu->addChild(menuItemEasy);
    menu->addChild(menuItemNormal);
    menu->addChild(menuItemHard);
    menu->addChild(menuItemLeaderboard);
    menu->addChild(menuItemAchievements);
    menu->addChild(menuItemSettings);
    menu->addChild(menuSound);
    menu->addChild(menuRateApp);
    
    addChild(menu);
    
    NativeUtils::showAd();
    
}

void HomeLayer::_manageMusic(cocos2d::CCObject* pSender)
{
    if(disable)
        return;
    
    SimpleAudioEngine::sharedEngine()->playEffect(SFX_BUTTON);
        
    bool state = LocalStorageManager::isMute();
    LocalStorageManager::setMute(!state);
}

HomeLayer::~HomeLayer()
{
    
}

void HomeLayer::_onOptionPressed(CCObject *pSender)
{
    if(disable)
        return;
    
    SimpleAudioEngine::sharedEngine()->playEffect(SFX_BUTTON);
    
    CCMenuItem* item = (CCMenuItem*) pSender;
    bool runGame = false;
    
    switch (item->getTag()) {
        case kTagEasyMode:
            runGame = true;
            _gameLayer->configureGame(kGameLevelEasy);
            break;
            
        case kTagNormalMode:
            runGame = true;
            _gameLayer->configureGame(kGameLevelNormal);
            break;
            
        case kTagHardMode:
            runGame = true;
            _gameLayer->configureGame(kGameLevelHard);
            break;
            
        case kTagSettings:
            CCDirector::sharedDirector()->pushScene(SettingsLayer::scene());
            break;
            
        case kTagLeaderboard:
            NativeUtils::showLeaderboards();
            break;
            
        case kTagAchievements:
            NativeUtils::showAchievements();
            break;
            
        case kTagRateApp:
            NativeUtils::rateApp();
            break;
            
        default:
            break;
    }
    
    if(runGame)
    {
        NativeUtils::hideAd();
        disable = true;
        
        CCSequence* sequence = CCSequence::create(
                                CCMoveBy::create(0.5f, ccp(0, -WIN_SIZE.height)),
                                CCCallFuncN::create(this, callfuncN_selector(HomeLayer::_finishHideLayer))                                               ,
                                                  NULL
                                                  );
        this->runAction(CCFadeOut::create(0.45f));
        this->runAction(sequence);
    }
    
}

void HomeLayer::_finishHideLayer()
{
    this->setVisible(false);
    _gameLayer->playGame();
    this->removeFromParent();    
}