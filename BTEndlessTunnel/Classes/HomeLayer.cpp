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

#define HIDE_TIME 0.4f

using namespace cocos2d;
using namespace CocosDenshion;

HomeLayer::HomeLayer(GameLayer* gameLayer) : _gameLayer(gameLayer)
{
    disable = false;
        
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint visibleOrigin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    // Tablero Izq.
    tablero = CCSprite::create("tablero_title.png");
    tablero->setAnchorPoint(CCPointZero);
    tablero->setPositionX(visibleOrigin.x);
    tablero->setPositionY(visibleOrigin.y);
    addChild(tablero);
    
    // Crear logo
    logo = CCSprite::create("logo.png");
    logo->setPositionX(-logo->getContentSize().width * 1.2f);
    logo->setPositionY(visibleOrigin.y + visibleSize.height * 0.55f);
    
    CCPoint finalPos = ccp(visibleOrigin.x + visibleSize.width * 0.65f, logo->getPositionY());
    CCMoveTo* move1 = CCMoveTo::create(0.9f, finalPos);
    
    logo->runAction(CCSequence::create(CCDelayTime::create(0.25f), move1, NULL));
    
    addChild(logo , -1);
    
    // Achievements
    menuItemAchievements = CCMenuItemImage::create("achievement.png", "achievement.png", this, menu_selector(HomeLayer::_onOptionPressed));
    menuItemAchievements->setTag(kTagAchievements);
    menuItemAchievements->setPositionX(visibleOrigin.x + menuItemAchievements->getContentSize().width);
    menuItemAchievements->setPositionY(visibleOrigin.y + menuItemAchievements->getContentSize().height * 0.75f);
    
    // Leaderboards
    menuItemLeaderboard = CCMenuItemImage::create("chart.png", "chart.png", this, menu_selector(HomeLayer::_onOptionPressed));
    menuItemLeaderboard->setTag(kTagLeaderboard);
    menuItemLeaderboard->setPositionX(menuItemAchievements->getPositionX() + menuItemAchievements->getContentSize().width * 1.2f);
    menuItemLeaderboard->setPositionY(menuItemAchievements->getPositionY());
    
    float scale = 1.05f;
    float time_dt = 1.3f;
    
    // Hard Mode
    menuItemHard = CCMenuItemImage::create("hard.png", "hard.png", this, menu_selector(HomeLayer::_onOptionPressed));
    menuItemHard->setTag(kTagHardMode);
    menuItemHard->setAnchorPoint(ccp(0, 0));
    menuItemHard->setPositionX(menuItemAchievements->getPositionX() - menuItemAchievements->getContentSize().width * 0.7f);
    menuItemHard->setPositionY(menuItemAchievements->getPositionY() + menuItemAchievements->getContentSize().height * 0.75f);
    menuItemHard->setRotation(-3);
    menuItemHard->runAction(CCRepeatForever::create(CCSequence::create(CCDelayTime::create(2 * time_dt), CCScaleTo::create(0.5f * time_dt, scale), CCScaleTo::create(0.5f * time_dt, 1.0f), CCDelayTime::create(0), NULL)));
    
    
    // Normal Mode
    menuItemNormal = CCMenuItemImage::create("medium.png", "medium.png", this, menu_selector(HomeLayer::_onOptionPressed));
    menuItemNormal->setTag(kTagNormalMode);
    menuItemNormal->setAnchorPoint(ccp(0, 0));
    menuItemNormal->setPositionX(menuItemHard->getPositionX());
    menuItemNormal->setPositionY(menuItemHard->getPositionY() + menuItemAchievements->getContentSize().height * 1.35f);
    menuItemNormal->setRotation(-2);
    menuItemNormal->runAction(CCRepeatForever::create(CCSequence::create(CCDelayTime::create(1 * time_dt), CCScaleTo::create(0.5f * time_dt, scale), CCScaleTo::create(0.5f * time_dt, 1.0f), CCDelayTime::create(1 * time_dt), NULL)));
    
    // Easy Mode
    menuItemEasy = CCMenuItemImage::create("easy.png", "easy.png", this, menu_selector(HomeLayer::_onOptionPressed));
    menuItemEasy->setTag(kTagEasyMode);
    menuItemEasy->setAnchorPoint(ccp(0, 0));
    menuItemEasy->setPositionX(menuItemNormal->getPositionX());
    menuItemEasy->setPositionY(menuItemNormal->getPositionY() + menuItemAchievements->getContentSize().height * 1.35f);
    menuItemEasy->setRotation(-2);
    menuItemEasy->runAction(CCRepeatForever::create(CCSequence::create(CCScaleTo::create(0.5f * time_dt, scale), CCScaleTo::create(0.5f * time_dt, 1.0f), CCDelayTime::create(2 * time_dt), NULL)));
    
    // Settings
    menuItemSettings = CCMenuItemImage::create("ajustes.png", "ajustes.png", this, menu_selector(HomeLayer::_onOptionPressed));
    menuItemSettings->setTag(kTagSettings);
    menuItemSettings->setPosition(ccp(visibleSize.width - menuItemSettings->getContentSize().width * 0.8f, visibleOrigin.y + menuItemSettings->getContentSize().height * 0.7f));
    
    // Rate App
    CCLabelTTF* lblRateApp = CCLabelTTF::create("Rate this App!", FONT_GAME, SIZE_RATE_APP);
    lblRateApp->setColor(ccWHITE);
    
    menuRateApp = CCMenuItemLabel::create(lblRateApp, this, menu_selector(HomeLayer::_onOptionPressed));
    menuRateApp->setTag(kTagRateApp);
    menuRateApp->setPositionX(visibleOrigin.x + visibleSize.width * 0.7f);
    menuRateApp->setPositionY(visibleOrigin.y + menuRateApp->getContentSize().height * 1.8f);
    menuRateApp->runAction(CCRepeatForever::create(CCSequence::create(CCScaleTo::create(0.75f, 1.2f), CCScaleTo::create(0.75f, 1.0f), NULL)));
    
    // Sound management
    CCMenuItemImage* menuSoundOn = CCMenuItemImage::create("sound_on.png", "sound_on.png", NULL, NULL);
    CCMenuItemImage* menuSoundOff = CCMenuItemImage::create("sound_off.png", "sound_off.png", NULL, NULL);
    
    menuSound = CCMenuItemToggle::createWithTarget(this, menu_selector(HomeLayer::_manageMusic), menuSoundOn, menuSoundOff, NULL);
    menuSound->setPositionX(menuItemLeaderboard->getPositionX() + menuItemAchievements->getContentSize().width * 1.2f);
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
        _hideToLeft();
        _hideToRight();
        scheduleOnce(schedule_selector(HomeLayer::_finishHideLayer), HIDE_TIME + 0.1f);
    }
    
}

void HomeLayer::_hideToLeft()
{
    CCMoveBy* move = CCMoveBy::create(HIDE_TIME, ccp(-WIN_SIZE.width * 0.8f, 0));
    tablero->runAction((CCAction*) move->copy()->autorelease());
    menuItemEasy->runAction((CCAction*) move->copy()->autorelease());
    menuItemNormal->runAction((CCAction*) move->copy()->autorelease());
    menuItemHard->runAction((CCAction*) move->copy()->autorelease());
    menuItemAchievements->runAction((CCAction*) move->copy()->autorelease());
    menuItemLeaderboard->runAction((CCAction*) move->copy()->autorelease());
    menuSound->runAction((CCAction*) move->copy()->autorelease());
}

void HomeLayer::_hideToRight()
{
    CCMoveBy* move = CCMoveBy::create(HIDE_TIME, ccp(WIN_SIZE.width * 0.8f, 0));
    menuItemSettings->runAction((CCAction*) move->copy()->autorelease());
    menuRateApp->runAction((CCAction*) move->copy()->autorelease());
    logo->runAction((CCAction*) move->copy()->autorelease());
}

void HomeLayer::_finishHideLayer()
{
    this->setVisible(false);
    _gameLayer->playGame();
    this->removeFromParent();    
}