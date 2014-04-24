//
//  HudLayer.cpp
//  BTEndlessTunnel
//
//  Created by NSS on 3/27/14.
//
//

#include "HudLayer.h"
#include "AppMacros.h"

#define JOYPAD_BG "joy_cont_L.png"
#define JOYPAD_THUMB "joy_L.png"

using namespace cocos2d;

HudLayer::HudLayer()
{
    
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    joypadBG = CCSprite::create(JOYPAD_BG);
    joypadThumb = CCSprite::create(JOYPAD_THUMB);
    
    CCPoint joypadPosition = ccp(origin.x + joypadBG->getContentSize().width * 0.9f, origin.y + joypadBG->getContentSize().height * 0.6f);
    
    SneakyJoystickSkinnedBase* baseStick = SneakyJoystickSkinnedBase::create();
    
    joypad = new SneakyJoystick();
    joypad->initWithRect(CCRectMake(0, 0, joypadBG->getContentSize().width, joypadBG->getContentSize().height));
    joypad->autorelease();
    
    baseStick->setBackgroundSprite(joypadBG);
    baseStick->setThumbSprite(joypadThumb);
    baseStick->setJoystick(joypad);
    baseStick->setPosition(joypadPosition);
    
    joypad = baseStick->getJoystick();
    addChild(baseStick);

}

void HudLayer::runTutorialJoypad()
{
    joypadBG->runAction(CCRepeatForever::create(CCSequence::create(CCScaleTo::create(0.5f, 1.2f), CCScaleTo::create(0.5f, 1.0f), NULL)));
    
    joypadThumb->runAction(CCRepeatForever::create(CCSequence::create(CCScaleTo::create(0.5f, 1.2f), CCScaleTo::create(0.5f, 1.0f), NULL)));
    
}

void HudLayer::stopTutorialJoypad()
{
    joypadBG->stopAllActions();
    joypadBG->setScale(1);
    
    joypadThumb->stopAllActions();
    joypadThumb->setScale(1);
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
