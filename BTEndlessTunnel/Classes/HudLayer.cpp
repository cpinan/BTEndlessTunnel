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
    
    CCSprite* joypadBG = CCSprite::create(JOYPAD_BG);
    joypadBG->setOpacity(128);
    
    CCSprite* joypadThumb = CCSprite::create(JOYPAD_THUMB);
    joypadThumb->setOpacity(joypadBG->getOpacity());
    
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

HudLayer::~HudLayer()
{
    
}

void HudLayer::updateControl(BaseVehicle& player, float dt)
{
    CCPoint velocity = joypad->getVelocity();
    velocity = ccpMult(joypad->getVelocity(), player.getSpeed());
    
    player.doMove(velocity);
}
