//
//  VehicleFrog.cpp
//  BTEndlessTunnel
//
//  Created by NSS on 3/27/14.
//
//

#include "VehicleFrog.h"

using namespace cocos2d;

VehicleFrog::VehicleFrog() : BaseVehicle("bicho_0001.png")
{
    defaultTexture = getTexture();
    deadTexture = CCSprite::create("bicho_0004.png")->getTexture();
    
    CCAnimation* _jumpAnimation = CCAnimation::create();
    _jumpAnimation->addSpriteFrameWithFileName("bicho_0003.png");
    _jumpAnimation->setRestoreOriginalFrame(false);
    _jumpAnimation->setDelayPerUnit(1.0f / 10.0f);
    
    jumpAnimation = CCAnimate::create(_jumpAnimation);
    jumpAnimation->retain();
    
    CCAnimation* _idleAnimation = CCAnimation::create();
    
    _idleAnimation->addSpriteFrameWithFileName("bicho_0001.png");
    _idleAnimation->addSpriteFrameWithFileName("bicho_0002.png");
    
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