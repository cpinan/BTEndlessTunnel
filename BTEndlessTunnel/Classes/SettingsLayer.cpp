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
    if(initWithColor(ccc4(255, 255, 255, 255)))
    {
        
        CCMenu* menu = CCMenu::create();
        menu->setPosition(CCPointZero);
        menu->setAnchorPoint(ccp(0, 0));
        
        // Joypad Mode
        CCMenuItemImage* itemJoypad = CCMenuItemImage::create("joypad_mode.png", "joypad_mode.png", this, menu_selector(SettingsLayer::_onOptionPressed));
        itemJoypad->setAnchorPoint(ccp(0, 1.0f));
        itemJoypad->setTag(kTagSettingJoypadMode);
        itemJoypad->setPositionX(42);
        itemJoypad->setPositionY(designResolutionSize.height - 50);
        menu->addChild(itemJoypad);
        
        // Accel Mode
        CCMenuItemImage* itemAccel = CCMenuItemImage::create("accel_mode.png", "accel_mode.png", this, menu_selector(SettingsLayer::_onOptionPressed));
        itemAccel->setAnchorPoint(ccp(0, 1.0f));
        itemAccel->setTag(kTagSettingAccelMode);
        itemAccel->setPositionX(226);
        itemAccel->setPositionY(itemJoypad->getPositionY());
        menu->addChild(itemAccel);
        
        // Home button
        CCMenuItemImage* itemHome = CCMenuItemImage::create("home.png", "home.png", this, menu_selector(SettingsLayer::_onOptionPressed));
        itemHome->setAnchorPoint(ccp(0, 1.0f));
        itemHome->setTag(kTagSettingGoHome);
        itemHome->setPositionX(220);
        itemHome->setPositionY(designResolutionSize.height - 260);
        menu->addChild(itemHome);
        
        // Menu
        addChild(menu);
        
    }
}

void SettingsLayer::_onOptionPressed(CCObject *pSender)
{
    CCMenuItem* item = (CCMenuItem*) pSender;
    
    switch(item->getTag())
    {
        case kTagSettingGoHome:
            CCDirector::sharedDirector()->popScene();
            break;
            
        case kTagSettingJoypadMode:
            LocalStorageManager::setControlType(true);
            CCMessageBox("Joypad Mode activated", "Settings");
            break;
            
        case kTagSettingAccelMode:
            LocalStorageManager::setControlType(false);
            CCMessageBox("Accelerometer Mode activated", "Settings");
            break;
    }
}