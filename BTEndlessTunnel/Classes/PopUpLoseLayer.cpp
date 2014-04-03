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

using namespace cocos2d;

PopUpLoseLayer::PopUpLoseLayer()
{
    disable = false;
    CCLabelTTF* lblText;
    
    // Play Again
    lblText = CCLabelTTF::create("Play Again", "Arial", 20.0f);
    CCMenuItemLabel* itemPlayAgain = CCMenuItemLabel::create(lblText, this, menu_selector(PopUpLoseLayer::_onOptionPressed));
    itemPlayAgain->setTag(kTagPlayAgain);
    itemPlayAgain->setPosition(ccp(90, 200));
    
    // Go Home
    lblText = CCLabelTTF::create("Go Home", "Arial", 20.0f);
    CCMenuItemLabel* itemGoHome = CCMenuItemLabel::create(lblText, this, menu_selector(PopUpLoseLayer::_onOptionPressed));
    itemGoHome->setTag(kTagGoHome);
    itemGoHome->setPosition(ccp(250, 200));
    
    // Menu
    CCMenu* menu = CCMenu::create();
    menu->setPosition(CCPointZero);
    menu->addChild(itemPlayAgain);
    menu->addChild(itemGoHome);
    
    addChild(menu);
    
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
            CCScene* scene = HomeScene::scene(kGameModeHome);
            CCDirector::sharedDirector()->replaceScene(CCTransitionFadeDown::create(1.0f, scene));
            break;

    }

    
}