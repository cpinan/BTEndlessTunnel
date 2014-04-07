//
//  HomeLayer.cpp
//  BTEndlessTunnel
//
//  Created by NSS on 4/1/14.
//
//

#include "HomeLayer.h"
#include "SettingsLayer.h"

using namespace cocos2d;

HomeLayer::HomeLayer(GameLayer* gameLayer) : _gameLayer(gameLayer)
{
    disable = false;
    
    // Easy Mode
    CCMenuItemImage* menuItemEasy = CCMenuItemImage::create("btn_easy.png", "btn_easy.png", this, menu_selector(HomeLayer::_onOptionPressed));
    menuItemEasy->setTag(kTagEasyMode);
    menuItemEasy->setAnchorPoint(ccp(0, 0));
    menuItemEasy->setPosition(ccp(90, 190));
    
    // Normal Mode
    CCMenuItemImage* menuItemNormal = CCMenuItemImage::create("btn_normal.png", "btn_normal.png", this, menu_selector(HomeLayer::_onOptionPressed));
    menuItemNormal->setTag(kTagNormalMode);
    menuItemNormal->setAnchorPoint(ccp(0, 0));
    menuItemNormal->setPosition(ccp(menuItemEasy->getPositionX(), 120));
    
    // Hard Mode
    CCMenuItemImage* menuItemHard = CCMenuItemImage::create("btn_hard.png", "btn_hard.png", this, menu_selector(HomeLayer::_onOptionPressed));
    menuItemHard->setTag(kTagHardMode);
    menuItemHard->setAnchorPoint(ccp(0, 0));
    menuItemHard->setPosition(ccp(menuItemEasy->getPositionX(), 50));
    
    // Leaderboards
    CCMenuItemImage* menuItemLeaderboard = CCMenuItemImage::create("leaderboard.png", "leaderboard.png", this, menu_selector(HomeLayer::_onOptionPressed));
    menuItemLeaderboard->setTag(kTagLeaderboard);
    menuItemLeaderboard->setAnchorPoint(ccp(0, 0));
    menuItemLeaderboard->setPosition(ccp(350, 245));
    
    // Achievements
    CCMenuItemImage* menuItemAchievements = CCMenuItemImage::create("achievements.png", "achievements.png", this, menu_selector(HomeLayer::_onOptionPressed));
    menuItemAchievements->setTag(kTagAchievements);
    menuItemAchievements->setAnchorPoint(ccp(0, 0));
    menuItemAchievements->setPosition(ccp(menuItemLeaderboard->getPositionX(), 165));
    
    // Settings
    CCMenuItemImage* menuItemSettings = CCMenuItemImage::create("settings.png", "settings.png", this, menu_selector(HomeLayer::_onOptionPressed));
    menuItemSettings->setTag(kTagSettings);
    menuItemSettings->setAnchorPoint(ccp(0, 0));
    menuItemSettings->setPosition(ccp(menuItemLeaderboard->getPositionX(), 85));
    
    // Credits
    CCMenuItemImage* menuItemCredits = CCMenuItemImage::create("about.png", "about.png", this, menu_selector(HomeLayer::_onOptionPressed));
    menuItemCredits->setTag(kTagCredits);
    menuItemCredits->setAnchorPoint(ccp(0, 0));
    menuItemCredits->setPosition(ccp(menuItemLeaderboard->getPositionX(), 5));
    
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
    menu->addChild(menuItemCredits);
    
    addChild(menu);
    
}

HomeLayer::~HomeLayer()
{
    
}

void HomeLayer::_onOptionPressed(CCObject *pSender)
{
    if(disable)
        return;
    
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
            CCDirector::sharedDirector()->pushScene(CCTransitionMoveInT::create(0.5f, SettingsLayer::scene()));
            break;
            
        default:
            break;
    }
    
    if(runGame)
    {
        disable = true;
        
        CCSequence* sequence = CCSequence::create(
                                CCMoveBy::create(0.5f, ccp(0, -designResolutionSize.height)),
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