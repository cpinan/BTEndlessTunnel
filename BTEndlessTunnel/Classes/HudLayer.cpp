//
//  HudLayer.cpp
//  BTEndlessTunnel
//
//  Created by NSS on 3/27/14.
//
//

#include "HudLayer.h"
#include "AppMacros.h"

#define JOYPAD_BG "joystick.png"
#define JOYPAD_THUMB "joy_L.png"
#define JOYPAD_OPACITY 70

using namespace cocos2d;

HudLayer::HudLayer()
{
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    joypadBG = CCSprite::create(JOYPAD_BG);
    joypadBG->setOpacity(JOYPAD_OPACITY);
    joypadThumb = CCSprite::create(JOYPAD_THUMB);
    joypadThumb->setOpacity(JOYPAD_OPACITY);
    
    CCPoint joypadPosition = ccp(origin.x + joypadBG->getContentSize().width * 0.9f, origin.y + joypadBG->getContentSize().height * 0.6f);
    
    baseStick = SneakyJoystickSkinnedBase::create();
    
    joypad = new SneakyJoystick();
    joypad->initWithRect(CCRectMake(0, 0, joypadBG->getContentSize().width, joypadBG->getContentSize().height));
    joypad->autorelease();
    
    baseStick->setBackgroundSprite(joypadBG);
    baseStick->setThumbSprite(joypadThumb);
    baseStick->setJoystick(joypad);
    baseStick->setPosition(joypadPosition);
    
    joypad = baseStick->getJoystick();
    joypad->setBaseStick(baseStick);
    addChild(baseStick);
}

void HudLayer::runTutorialJoypad()
{
    
    joypad->setEnabled(false);
    
    joypadBG->setOpacity(255);
    joypadThumb->setOpacity(255);
    
    joypadBG->runAction(CCRepeatForever::create(CCSequence::create(CCScaleTo::create(0.5f, 1.2f), CCScaleTo::create(0.5f, 1.0f), NULL)));
    
    joypadThumb->runAction(CCRepeatForever::create(CCSequence::create(CCScaleTo::create(0.5f, 1.2f), CCScaleTo::create(0.5f, 1.0f), NULL)));
    
}

void HudLayer::stopTutorialJoypad()
{
    
    joypadBG->stopAllActions();
    joypadBG->setScale(1);
    joypadBG->setOpacity(JOYPAD_OPACITY);
    
    joypadThumb->stopAllActions();
    joypadThumb->setScale(1);
    joypadThumb->setOpacity(JOYPAD_OPACITY);
    
    joypad->setEnabled(true);
}

HudLayer::~HudLayer()
{
    
}

void HudLayer::updateControl(BaseVehicle& player, float dt)
{
    CCPoint velocity = joypad->getVelocity();
    velocity = ccpMult(joypad->getVelocity(), player.getSpeed());
    
    player.doMove(velocity);
}
