//
//  PauseLayer.cpp
//  BTEndlessTunnel
//
//  Created by NSS on 4/1/14.
//
//

#include "PauseLayer.h"
#include "AppMacros.h"
#include "Constants.h"
#include "HomeScene.h"

using namespace cocos2d;

PauseLayer::PauseLayer()
{
    // BGWhite
    CCSprite* bg_white = CCSprite::create("bg_white.png");
    bg_white->setPosition(ccp(designResolutionSize.width * 0.5f, designResolutionSize.height * 0.5f));
    addChild(bg_white);
    
    // Pause Text
    CCLabelTTF* lblPause = CCLabelTTF::create("PAUSE", "Arial", 16.0f, CCSizeMake(190, 20), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    lblPause->setAnchorPoint(ccp(0, 0));
    lblPause->setPosition(ccp(144, designResolutionSize.height - 73));
    lblPause->setColor(ccc3(0, 0, 0));
    addChild(lblPause);
    
    // Jump image
    CCSprite* spJump = CCSprite::create("moto_jump3.png");
    spJump->setAnchorPoint(ccp(0, 0));
    spJump->setPositionX(201);
    spJump->setPositionY(designResolutionSize.height - 160);
    addChild(spJump);
    
    // Buttons
    
    // Send score button
    CCMenuItemImage* itemResume = CCMenuItemImage::create("resume.png", "resume.png", this, menu_selector(PauseLayer::_onOptionPressed));
    itemResume->setTag(kTagPauseResumeGame);
    itemResume->setAnchorPoint(ccp(0, 0));
    itemResume->setPosition(ccp(161, designResolutionSize.height - 230));
    
    // Home button
    CCMenuItemImage* itemHome = CCMenuItemImage::create("home.png", "home.png", this, menu_selector(PauseLayer::_onOptionPressed));
    itemHome->setTag(kTagPauseGoHome);
    itemHome->setAnchorPoint(ccp(0, 0));
    itemHome->setPosition(ccp(222, itemResume->getPositionY()));
    
    // Play again button
    CCMenuItemImage* itemPlayAgain = CCMenuItemImage::create("play_again.png", "play_again.png", this, menu_selector(PauseLayer::_onOptionPressed));
    itemPlayAgain->setTag(kTagPausePlayAgain);
    itemPlayAgain->setAnchorPoint(ccp(0, 0));
    itemPlayAgain->setPosition(ccp(282, itemResume->getPositionY()));
    
    // Menu
    CCMenu* menu = CCMenu::create();
    menu->setAnchorPoint(ccp(0, 0));
    menu->setPosition(CCPointZero);
    
    menu->addChild(itemResume);
    menu->addChild(itemHome);
    menu->addChild(itemPlayAgain);
    
    addChild(menu);
    
}

void PauseLayer::_onOptionPressed(CCObject *pSender)
{
    CCMenuItem* item = (CCMenuItem*) pSender;
    
    switch (item->getTag()) {
        case kTagPauseResumeGame:
            CCNotificationCenter::sharedNotificationCenter()->postNotification(NOTIFICATION_RESUME_GAME);
            break;
            
        case kTagPausePlayAgain:
            CCNotificationCenter::sharedNotificationCenter()->postNotification(NOTIFICATION_PLAY_AGAIN);
            break;
            
        case kTagPauseGoHome:
            CCNotificationCenter::sharedNotificationCenter()->postNotification(NOTIFICATION_GO_HOME);
            break;
            
    }
    
    
}

