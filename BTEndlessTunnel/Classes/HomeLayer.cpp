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


using namespace cocos2d;
using namespace CocosDenshion;

HomeLayer::HomeLayer(GameLayer* gameLayer) : _gameLayer(gameLayer)
{
    disable = false;
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint visibleOrigin = CCDirector::sharedDirector()->getVisibleOrigin();
    CCPoint origin = ccp(visibleOrigin.x + visibleSize.width * 0.5f, visibleOrigin.y + visibleSize.height* 0.5f);
    
    // Easy Mode
    CCMenuItemImage* menuItemEasy = CCMenuItemImage::create("btn_easy.png", "btn_easy.png", this, menu_selector(HomeLayer::_onOptionPressed));
    menuItemEasy->setTag(kTagEasyMode);
    menuItemEasy->setAnchorPoint(ccp(0, 0));
    menuItemEasy->setPosition(ccp(origin.x - menuItemEasy->getContentSize().width, origin.y + menuItemEasy->getContentSize().height));
    
    // Normal Mode
    CCMenuItemImage* menuItemNormal = CCMenuItemImage::create("btn_normal.png", "btn_normal.png", this, menu_selector(HomeLayer::_onOptionPressed));
    menuItemNormal->setTag(kTagNormalMode);
    menuItemNormal->setAnchorPoint(ccp(0, 0));
    menuItemNormal->setPosition(ccp(menuItemEasy->getPositionX(), menuItemEasy->getPositionY() - menuItemEasy->getContentSize().height * 1.5f));
    
    // Hard Mode
    CCMenuItemImage* menuItemHard = CCMenuItemImage::create("btn_hard.png", "btn_hard.png", this, menu_selector(HomeLayer::_onOptionPressed));
    menuItemHard->setTag(kTagHardMode);
    menuItemHard->setAnchorPoint(ccp(0, 0));
    menuItemHard->setPosition(ccp(menuItemNormal->getPositionX(), menuItemNormal->getPositionY() - menuItemEasy->getContentSize().height * 1.5f));
    
    // Leaderboards
    CCMenuItemImage* menuItemLeaderboard = CCMenuItemImage::create("leaderboard.png", "leaderboard.png", this, menu_selector(HomeLayer::_onOptionPressed));
    menuItemLeaderboard->setTag(kTagLeaderboard);
    menuItemLeaderboard->setAnchorPoint(ccp(0, 0));
    menuItemLeaderboard->setPosition(ccp(origin.x + menuItemEasy->getContentSize().width * 0.5f, menuItemEasy->getPositionY()));
    
    // Achievements
    CCMenuItemImage* menuItemAchievements = CCMenuItemImage::create("achievements.png", "achievements.png", this, menu_selector(HomeLayer::_onOptionPressed));
    menuItemAchievements->setTag(kTagAchievements);
    menuItemAchievements->setAnchorPoint(ccp(0, 0));
    menuItemAchievements->setPosition(ccp(menuItemLeaderboard->getPositionX(), menuItemLeaderboard->getPositionY() - menuItemLeaderboard->getContentSize().height * 1.3f));
    
    // Settings
    CCMenuItemImage* menuItemSettings = CCMenuItemImage::create("settings.png", "settings.png", this, menu_selector(HomeLayer::_onOptionPressed));
    menuItemSettings->setTag(kTagSettings);
    menuItemSettings->setAnchorPoint(ccp(0, 0));
    menuItemSettings->setPosition(ccp(menuItemLeaderboard->getPositionX(), menuItemAchievements->getPositionY() - menuItemLeaderboard->getContentSize().height * 1.3f));

    // Menu
    CCMenu* menu = CCMenu::create();
    menu->setAnchorPoint(ccp(0, 0));
    menu->setPosition(CCPointZero);
    menu->addChild(menuItemEasy);
    menu->addChild(menuItemNormal);
    menu->addChild(menuItemHard);
    menu->addChild(menuItemLeaderboard);
    menu->addChild(menuItemAchievements);
    menu->addChild(menuItemSettings);
    
    addChild(menu);
    
    NativeUtils::showAd();
    
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