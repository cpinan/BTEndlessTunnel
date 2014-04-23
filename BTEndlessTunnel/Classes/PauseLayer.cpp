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
    
    if(initWithColor(ccc4BFromccc4F(ccc4f(0, 0, 0, 100.0f / 255.0f))))
    {
        CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
        CCPoint visibleOrigin = CCDirector::sharedDirector()->getVisibleOrigin();
        CCPoint origin = ccp(visibleOrigin.x + visibleSize.width * 0.5f, visibleOrigin.y + visibleSize.height* 0.5f);
        
        // BGWhite
        CCSprite* bg = CCSprite::create("pause_screen.png");
        bg->setPosition(origin);
        addChild(bg);
        
        float w = bg->getContentSize().width;
        float h = bg->getContentSize().height;
        CCPoint o = ccp(w * 0.5f, h * 0.5f);
        
        // Buttons
        
        
        // Play again button
        CCMenuItemImage* itemPlayAgain = CCMenuItemImage::create("pause_replay.png", "pause_replay.png", this, menu_selector(PauseLayer::_onOptionPressed));
        itemPlayAgain->setTag(kTagPausePlayAgain);
        itemPlayAgain->setPositionX(o.x - itemPlayAgain->getContentSize().width);
        itemPlayAgain->setPositionY(o.y - itemPlayAgain->getContentSize().height * 0.9f);
        
        CCMenuItemImage* itemResume = CCMenuItemImage::create("pause_play.png", "pause_play.png", this, menu_selector(PauseLayer::_onOptionPressed));
        itemResume->setTag(kTagPauseResumeGame);
        itemResume->setPositionX(itemPlayAgain->getPositionX() + itemResume->getContentSize().width * 1.1f);
        itemResume->setPositionY(itemPlayAgain->getPositionY());
        
        // Home button
        CCMenuItemImage* itemHome = CCMenuItemImage::create("pause_home.png", "pause_home.png", this, menu_selector(PauseLayer::_onOptionPressed));
        itemHome->setTag(kTagPauseGoHome);
        itemHome->setPositionX(itemResume->getPositionX() + itemResume->getContentSize().width * 1.1f);
        itemHome->setPositionY(itemResume->getPositionY());
        
        itemPlayAgain->setPositionY(itemPlayAgain->getPositionY() - itemPlayAgain->getContentSize().height * 0.3f);
        itemResume->setPositionY(itemResume->getPositionY() - itemResume->getContentSize().height * 0.15f);
        
        
        // Menu
        CCMenu* menu = CCMenu::create();
        menu->setAnchorPoint(ccp(0, 0));
        menu->setPosition(CCPointZero);
        
        menu->addChild(itemResume);
        menu->addChild(itemHome);
        menu->addChild(itemPlayAgain);
        
        bg->addChild(menu);
    }
    
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

