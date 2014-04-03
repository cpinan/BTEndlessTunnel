//
//  HomeLayer.cpp
//  BTEndlessTunnel
//
//  Created by NSS on 4/1/14.
//
//

#include "HomeLayer.h"

using namespace cocos2d;

HomeLayer::HomeLayer(GameLayer* gameLayer) : _gameLayer(gameLayer)
{
    disable = false;
    CCLabelTTF* lblText;
    
    // Easy Mode
    lblText = CCLabelTTF::create("Easy Mode", "Arial", 20.0f);
    CCMenuItemLabel* itemEasy = CCMenuItemLabel::create(lblText, this, menu_selector(HomeLayer::_onOptionPressed));
    itemEasy->setTag(kTagEasyMode);
    itemEasy->setPosition(ccp(90, 200));
    
    // Normal Mode
    lblText = CCLabelTTF::create("Normal Mode", "Arial", 20.0f);
    CCMenuItemLabel* itemNormal = CCMenuItemLabel::create(lblText, this, menu_selector(HomeLayer::_onOptionPressed));
    itemNormal->setTag(kTagNormalMode);
    itemNormal->setPosition(ccp(240, 200));
    
    // Hard Mode
    lblText = CCLabelTTF::create("Hard Mode", "Arial", 20.0f);
    CCMenuItemLabel* itemHard = CCMenuItemLabel::create(lblText, this, menu_selector(HomeLayer::_onOptionPressed));
    itemHard->setTag(kTagHardMode);
    itemHard->setPosition(ccp(390, 200));
    
    // Credits
    lblText = CCLabelTTF::create("Credits", "Arial", 20.0f);
    CCMenuItemLabel* itemCredits = CCMenuItemLabel::create(lblText, this, menu_selector(HomeLayer::_onOptionPressed));
    itemCredits->setTag(kTagCredits);
    itemCredits->setPosition(ccp(90, 140));
    
    // Leaderboards
    lblText = CCLabelTTF::create("Leaderboard", "Arial", 20.0f);
    CCMenuItemLabel* itemLeaderboard = CCMenuItemLabel::create(lblText, this, menu_selector(HomeLayer::_onOptionPressed));
    itemLeaderboard->setTag(kTagLeaderboard);
    itemLeaderboard->setPosition(ccp(250, 140));
    
    // Achievements
    lblText = CCLabelTTF::create("Achievements", "Arial", 20.0f);
    CCMenuItemLabel* itemAchievements = CCMenuItemLabel::create(lblText, this, menu_selector(HomeLayer::_onOptionPressed));
    itemAchievements->setTag(kTagAchievements);
    itemAchievements->setPosition(ccp(100, 90));
    
    // Settings
    lblText = CCLabelTTF::create("Settings", "Arial", 20.0f);
    CCMenuItemLabel* itemSettings = CCMenuItemLabel::create(lblText, this, menu_selector(HomeLayer::_onOptionPressed));
    itemSettings->setTag(kTagSettings);
    itemSettings->setPosition(ccp(250, 90));
    
    // Menu
    CCMenu* menu = CCMenu::create();
    menu->setPosition(CCPointZero);
    menu->addChild(itemEasy);
    menu->addChild(itemNormal);
    menu->addChild(itemHard);
    menu->addChild(itemCredits);
    menu->addChild(itemLeaderboard);
    menu->addChild(itemAchievements);
    menu->addChild(itemSettings);
    
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
        this->runAction(sequence);
    }
    
}

void HomeLayer::_finishHideLayer()
{
    this->setVisible(false);
    _gameLayer->playGame();
    this->removeFromParent();    
}