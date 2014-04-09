//
//  HudLayer.cpp
//  BTEndlessTunnel
//
//  Created by NSS on 3/27/14.
//
//

#include "HudLayer.h"
#include "AppMacros.h"

#define JOYPAD_NAME "joy_cont_L.png"
#define JOYPAD_FINGER "joy_L.png"
#define JUMP_BUTTON "handDown.png"

#define RECT_JOYPAD CCRectMake(0, 0, 78, 78)
#define RECT_JUMP_BUTTON CCRectMake(0, 0, 200, 200)

#define JOYPAD_POSITION ccp(100, 50)
#define JUMP_POSITION ccp(designResolutionSize.width - 100, 50)

using namespace cocos2d;

HudLayer::HudLayer()
{
    SneakyJoystickSkinnedBase* baseStick = SneakyJoystickSkinnedBase::create();
    
    joypad = new SneakyJoystick();
    joypad->initWithRect(RECT_JOYPAD);
    joypad->autorelease();
    
    baseStick->setBackgroundSprite(CCSprite::create(JOYPAD_NAME));
    baseStick->setThumbSprite(CCSprite::create(JOYPAD_FINGER));
    baseStick->setJoystick(joypad);
    baseStick->setPosition(JOYPAD_POSITION);
    
    joypad = baseStick->getJoystick();
    addChild(baseStick);
    
    // Add jump
    SneakyButtonSkinnedBase* jumpButtonBase = SneakyButtonSkinnedBase::create();
    jumpButton = new SneakyButton();
    jumpButton->initWithRect(RECT_JUMP_BUTTON);
    jumpButton->autorelease();
    jumpButtonBase->setDefaultSprite(CCSprite::create("handDown.png"));
    jumpButtonBase->setActivatedSprite(CCSprite::create("handDown.png"));
    jumpButtonBase->setPressSprite(CCSprite::create("handDown.png"));
    jumpButtonBase->setButton(jumpButton);
    jumpButtonBase->setPosition(JUMP_POSITION);
    jumpButton = jumpButtonBase->getButton();
    jumpButton->setIsToggleable(false);
    addChild(jumpButtonBase);

}

HudLayer::~HudLayer()
{
    
}

void HudLayer::updateControl(BaseVehicle& player, float dt)
{

    if(jumpButton->getIsActive())
        player.doJump();
    
    CCPoint velocity = joypad->getVelocity();
    velocity = ccpMult(joypad->getVelocity(), player.getSpeed());
    
    player.doMove(velocity);
    
}
