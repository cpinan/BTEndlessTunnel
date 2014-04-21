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
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint visibleOrigin = CCDirector::sharedDirector()->getVisibleOrigin();
    CCPoint origin = ccp(visibleOrigin.x + visibleSize.width * 0.5f, visibleOrigin.y + visibleSize.height* 0.5f);
    
    // BGWhite
    CCSprite* bg = CCSprite::create("bg_white.png");
    bg->setPosition(origin);
    addChild(bg);
    
    float w = bg->getContentSize().width;
    float h = bg->getContentSize().height;
    CCPoint o = ccp(w * 0.5f, h * 0.5f);
    
    // Pause Text
    CCLabelTTF* lblPause = CCLabelTTF::create("PAUSE", FONT_GAME, 16.0f, CCSizeMake(190, 20), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    lblPause->setPosition(ccp(o.x, o.y + h * 0.25f));
    lblPause->setColor(ccc3(0, 0, 0));
    bg->addChild(lblPause);
    
    // Jump image
    CCSprite* spJump = CCSprite::create("moto_jump3.png");
    spJump->setPosition(o);
    bg->addChild(spJump);
    
    // Buttons
    
    // Send score button
    CCMenuItemImage* itemResume = CCMenuItemImage::create("resume.png", "resume.png", this, menu_selector(PauseLayer::_onOptionPressed));
    itemResume->setTag(kTagPauseResumeGame);
    itemResume->setPosition(ccp(o.x - w * 0.25f, o.y - h * 0.25f));
    
    // Home button
    CCMenuItemImage* itemHome = CCMenuItemImage::create("home.png", "home.png", this, menu_selector(PauseLayer::_onOptionPressed));
    itemHome->setTag(kTagPauseGoHome);
    itemHome->setPosition(ccp(itemResume->getPositionX() + itemResume->getContentSize().width * 1.5f, itemResume->getPositionY()));
    
    // Play again button
    CCMenuItemImage* itemPlayAgain = CCMenuItemImage::create("play_again.png", "play_again.png", this, menu_selector(PauseLayer::_onOptionPressed));
    itemPlayAgain->setTag(kTagPausePlayAgain);
    itemPlayAgain->setPosition(ccp(itemHome->getPositionX() + itemResume->getContentSize().width * 1.5f, itemResume->getPositionY()));
    
    // Menu
    CCMenu* menu = CCMenu::create();
    menu->setAnchorPoint(ccp(0, 0));
    menu->setPosition(CCPointZero);
    
    menu->addChild(itemResume);
    menu->addChild(itemHome);
    menu->addChild(itemPlayAgain);
    
    bg->addChild(menu);
    
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

