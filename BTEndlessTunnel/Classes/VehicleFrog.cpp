//
//  VehicleFrog.cpp
//  BTEndlessTunnel
//
//  Created by NSS on 3/27/14.
//
//

#include "VehicleFrog.h"

using namespace cocos2d;

VehicleFrog::VehicleFrog() : BaseVehicle("bibho_0001@X.png")
{
    defaultTexture = getTexture();
    deadTexture = CCSprite::create("moto_over.png")->getTexture();
    
    CCAnimation* _jumpAnimation = CCAnimation::create();
    
    _jumpAnimation->addSpriteFrameWithFileName("moto_jump1.png");
    _jumpAnimation->addSpriteFrameWithFileName("moto_jump2.png");
    _jumpAnimation->addSpriteFrameWithFileName("moto_jump3.png");
    
    _jumpAnimation->setRestoreOriginalFrame(false);
    _jumpAnimation->setDelayPerUnit(1.0f / 16.0f);
    
    jumpAnimation = CCAnimate::create(_jumpAnimation);
    jumpAnimation->retain();
    
    CCAnimation* _idleAnimation = CCAnimation::create();
    
    _idleAnimation->addSpriteFrameWithFileName("bibho_0001@X.png");
    _idleAnimation->addSpriteFrameWithFileName("bibho_0002@X.png");
    
    _idleAnimation->setRestoreOriginalFrame(false);
    _idleAnimation->setDelayPerUnit(1.0f / 12.0f);
    
    idleAnimation = CCRepeatForever::create(CCAnimate::create(_idleAnimation));
    idleAnimation->retain();
    
    runAction(idleAnimation);
    
}

VehicleFrog::~VehicleFrog()
{
    CC_SAFE_RELEASE(jumpAnimation);
    CC_SAFE_RELEASE(idleAnimation);
}