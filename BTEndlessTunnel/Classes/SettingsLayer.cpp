//
//  SettingsLayer.cpp
//  BTEndlessTunnel
//
//  Created by NSS on 4/1/14.
//
//

#include "SettingsLayer.h"
#include "AppMacros.h"
#include "LocalStorageManager.h"
#include "Constants.h"

#define CONTROL_ALPHA 100

using namespace cocos2d;

CCScene* SettingsLayer::scene()
{
    CCScene* scene = CCScene::create();
    
    SettingsLayer* layer = new SettingsLayer();
    layer->autorelease();
    
    scene->addChild(layer);
    return scene;
}

SettingsLayer::SettingsLayer()
{
    if(initWithColor(ccc4BFromccc4F(ccc4f(0, 0, 0, 0.5))))
    {
        
        bool joypad = LocalStorageManager::isUsingJoypad();
        
        CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
        CCPoint visibleOrigin = CCDirector::sharedDirector()->getVisibleOrigin();
        
        CCSprite* bg = CCSprite::create("controls_options.png");
        bg->setPositionX(visibleOrigin.x + visibleSize.width * 0.5f);
        bg->setPositionY(visibleOrigin.y + visibleSize.height * 0.5f);
        addChild(bg);
        
        CCPoint center = CCPointZero;
        center.x = bg->getContentSize().width * 0.5f;
        center.y = bg->getContentSize().height * 0.5f;
        
        CCMenu* menu = CCMenu::create();
        menu->setPosition(CCPointZero);
        menu->setAnchorPoint(ccp(0, 0));
        
        // Joypad Mode
        itemJoypad = CCMenuItemImage::create("control_joystick.png", "control_joystick.png", this, menu_selector(SettingsLayer::_onOptionPressed));
        itemJoypad->setTag(kTagSettingJoypadMode);
        itemJoypad->setPositionX(center.x - itemJoypad->getContentSize().width * 0.5f);
        itemJoypad->setPositionY(center.y + itemJoypad->getContentSize().height * 0.05f);
        menu->addChild(itemJoypad);
        if(!joypad)
            itemJoypad->setOpacity(CONTROL_ALPHA);
        
        // Accel Mode
        itemAccel = CCMenuItemImage::create("control_tilt.png", "control_tilt.png", this, menu_selector(SettingsLayer::_onOptionPressed));
        itemAccel->setTag(kTagSettingAccelMode);
        itemAccel->setPositionX(center.x + itemAccel->getContentSize().width * 0.6f);
        itemAccel->setPositionY(itemJoypad->getPositionY());
        menu->addChild(itemAccel);
        if(joypad)
            itemAccel->setOpacity(CONTROL_ALPHA);
        
        // Home button
        CCMenuItemImage* itemHome = CCMenuItemImage::create("pause_home_off.png", "pause_home.png", this, menu_selector(SettingsLayer::_onOptionPressed));
        itemHome->setTag(kTagSettingGoHome);
        itemHome->setPositionX(center.x);
        itemHome->setPositionY(center.y - itemHome->getContentSize().height * 1.6f);
        menu->addChild(itemHome);
        
        // Menu
        bg->addChild(menu);
        
    }
}

void SettingsLayer::_onOptionPressed(CCObject *pSender)
{
    CCMenuItem* item = (CCMenuItem*) pSender;
    
    switch(item->getTag())
    {
        case kTagSettingGoHome:
            CCNotificationCenter::sharedNotificationCenter()->postNotification(NOTIFICATION_ENABLE_BUTTONS);
            this->setVisible(false);
            break;
            
        case kTagSettingJoypadMode:
            itemJoypad->setOpacity(255);
            itemAccel->setOpacity(CONTROL_ALPHA);
            LocalStorageManager::setControlType(true);
            break;
            
        case kTagSettingAccelMode:
            itemJoypad->setOpacity(CONTROL_ALPHA);
            itemAccel->setOpacity(255);
            LocalStorageManager::setControlType(false);
            break;
    }
}