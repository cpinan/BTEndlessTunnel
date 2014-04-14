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
        
        CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
        CCPoint visibleOrigin = CCDirector::sharedDirector()->getVisibleOrigin();
        CCPoint origin = ccp(visibleOrigin.x + visibleSize.width * 0.5f, visibleOrigin.y + visibleSize.height* 0.5f);
        
        CCMenu* menu = CCMenu::create();
        menu->setPosition(CCPointZero);
        menu->setAnchorPoint(ccp(0, 0));
        
        // Joypad Mode
        CCMenuItemImage* itemJoypad = CCMenuItemImage::create("joypad_mode.png", "joypad_mode.png", this, menu_selector(SettingsLayer::_onOptionPressed));
        itemJoypad->setTag(kTagSettingJoypadMode);
        itemJoypad->setPositionX(origin.x - itemJoypad->getContentSize().width * 0.75f);
        itemJoypad->setPositionY(origin.y + itemJoypad->getContentSize().height * 0.1f);
        menu->addChild(itemJoypad);
        
        // Accel Mode
        CCMenuItemImage* itemAccel = CCMenuItemImage::create("accel_mode.png", "accel_mode.png", this, menu_selector(SettingsLayer::_onOptionPressed));
        itemAccel->setTag(kTagSettingAccelMode);
        itemAccel->setPositionX(origin.x + itemAccel->getContentSize().width * 0.4f);
        itemAccel->setPositionY(itemJoypad->getPositionY());
        menu->addChild(itemAccel);
        
        // Home button
        CCMenuItemImage* itemHome = CCMenuItemImage::create("home.png", "home.png", this, menu_selector(SettingsLayer::_onOptionPressed));
        itemHome->setTag(kTagSettingGoHome);
        itemHome->setPositionX(origin.x);
        itemHome->setPositionY(origin.y - itemHome->getContentSize().height * 3.5f);
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