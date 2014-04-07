//
//  PopUpLoseLayer.cpp
//  BTEndlessTunnel
//
//  Created by NSS on 4/1/14.
//
//

#include "PopUpLoseLayer.h"
#include "Constants.h"
#include "HomeScene.h"
#include "AppMacros.h"

using namespace cocos2d;

PopUpLoseLayer::PopUpLoseLayer()
{
    disable = false;
    
    // BGWhite
    CCSprite* bg_white = CCSprite::create("bg_white.png");
    bg_white->setPosition(ccp(designResolutionSize.width * 0.5f, designResolutionSize.height * 0.5f));
    addChild(bg_white);
    
    // Head title
    _lblHeadTitle = CCLabelTTF::create("SCORE", "Arial", 16.0f, CCSizeMake(190, 20), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    _lblHeadTitle->setAnchorPoint(ccp(0, 0));
    _lblHeadTitle->setPosition(ccp(144, designResolutionSize.height - 73));
    _lblHeadTitle->setColor(ccc3(0, 0, 0));
    addChild(_lblHeadTitle);
    
    // Badge and record
    CCSprite* spBadge = CCSprite::create("badge.png");
    spBadge->setAnchorPoint(ccp(0, 0));
    spBadge->setPosition(ccp(163, designResolutionSize.height - 138));
    addChild(spBadge, 10);
    
    _lblScore = CCLabelTTF::create("0", "Arial", 16.0f, CCSizeMake(130, 20), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    _lblScore->setColor(ccc3(0, 0, 0));
    _lblScore->setAnchorPoint(ccp(0, 0));
    _lblScore->setPosition(ccp(223, designResolutionSize.height - 130));
    addChild(_lblScore);
    
    // Send score button
    CCMenuItemImage* itemScore = CCMenuItemImage::create("play_game.png", "play_game.png", this, menu_selector(PopUpLoseLayer::_onOptionPressed));
    itemScore->setTag(kTagSendScore);
    itemScore->setAnchorPoint(ccp(0, 0));
    itemScore->setPosition(ccp(161, designResolutionSize.height - 220));
    
    // Home button
    CCMenuItemImage* itemHome = CCMenuItemImage::create("home.png", "home.png", this, menu_selector(PopUpLoseLayer::_onOptionPressed));
    itemHome->setTag(kTagGoHome);
    itemHome->setAnchorPoint(ccp(0, 0));
    itemHome->setPosition(ccp(222, itemScore->getPositionY()));
    
    // Play again button
    CCMenuItemImage* itemPlayAgain = CCMenuItemImage::create("play_again.png", "play_again.png", this, menu_selector(PopUpLoseLayer::_onOptionPressed));
    itemPlayAgain->setTag(kTagPlayAgain);
    itemPlayAgain->setAnchorPoint(ccp(0, 0));
    itemPlayAgain->setPosition(ccp(282, itemScore->getPositionY()));
    
    // Menu
    CCMenu* menu = CCMenu::create();
    menu->setAnchorPoint(ccp(0, 0));
    menu->setPosition(CCPointZero);
    
    menu->addChild(itemScore);
    menu->addChild(itemHome);
    menu->addChild(itemPlayAgain);
    
    addChild(menu);
    
}

void PopUpLoseLayer::updateScore(float score)
{
    _lblScore->setString(CCString::createWithFormat("%d", (int) score)->getCString());
}

void PopUpLoseLayer::_onOptionPressed(CCObject *pSender)
{
    if(disable)
        return;
    
    CCMenuItem* item = (CCMenuItem*) pSender;
    
    switch (item->getTag()) {
        case kTagPlayAgain:
            CCNotificationCenter::sharedNotificationCenter()->postNotification(NOTIFICATION_PLAY_AGAIN);
            break;
            
        case kTagGoHome:
            CCNotificationCenter::sharedNotificationCenter()->postNotification(NOTIFICATION_GO_HOME);
            break;

    }

    
}