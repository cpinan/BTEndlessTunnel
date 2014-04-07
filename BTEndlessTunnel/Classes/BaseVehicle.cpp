//
//  BaseVehicle.cpp
//  BTEndlessTunnel
//
//  Created by NSS on 3/27/14.
//
//

#include "BaseVehicle.h"
#include "SimpleAudioEngine.h"
#include "AppMacros.h"

using namespace cocos2d;
using namespace CocosDenshion;

BaseVehicle::BaseVehicle(std::string filename)
{
    if(initWithFile(filename.c_str()))
    {
        defaultTexture = NULL;
        jumpAnimation = NULL;
        idleAnimation = NULL;
        deadTexture = NULL;
        speed = 6.0f;
        state = kStateIdle;
        
        playerY = getPositionY() - getContentSize().height * 0.5f;
        
        _jumpByAction = CCJumpBy::create(MAX_PLAYER_JUMP / 100.0f, ccp(0, 0), MAX_PLAYER_JUMP, 1);
        _jumpByAction->setTag(kActionJumpTag);
        _jumpByAction->retain();
        
        _spShadow = CCSprite::create("shadow.png");
        _spShadow->setPositionX(getContentSize().width * 0.5f);
        addChild(_spShadow, -1);
        _updateShadow();
        
    }
}

BaseVehicle::~BaseVehicle()
{
    
}

void BaseVehicle::doJump()
{
    float y = getPositionY() - getPlayerY() - getContentSize().height * 0.5f;
    if(y == 0 && state != kStateJump && getActionByTag(kActionJumpTag) == NULL)
    {
        if(jumpAnimation != NULL && idleAnimation != NULL)
        {
            stopAction(idleAnimation);
            runAction(jumpAnimation);
        }
        runAction(_jumpByAction);
        state = kStateJump;
        SimpleAudioEngine::sharedEngine()->playEffect("jump.mp3");
    }
}

void BaseVehicle::dead()
{
    if(deadTexture != NULL)
    {
        stopAllActions();
        setTexture(deadTexture);
        SimpleAudioEngine::sharedEngine()->playEffect("smash.mp3");
        
        float x = getPositionX() + designResolutionSize.width * 1.5f;
        
        runAction(CCMoveTo::create(1.5f, ccp(x, getPositionY())));
    }
}

void BaseVehicle::doMove(CCPoint velocity)
{
    
    if(getActionByTag(kActionJumpTag) == NULL)
    {
        if(state == kStateJump)
        {
            if(defaultTexture != NULL)
            {
                setTexture(defaultTexture);
                if(idleAnimation != NULL)
                    runAction(idleAnimation);
            }
            state = kStateIdle;
        }
        playerY = getPositionY() - getContentSize().height * 0.5f;
    }
    else
    {
        if(playerY + velocity.y > 120)
            velocity.y = 0;
        else if(playerY + velocity.y < 70)
            velocity.y = 0;
    }
    
    CCPoint newPosition = ccpAdd(getPosition(), velocity);
    
    playerY += velocity.y;
    
    if(playerY > 120)
        playerY = 120;
    else if(playerY < 70)
        playerY = 70;
    
    if(newPosition.x < getContentSize().width * 0.5f)
        newPosition.x = getContentSize().width * 0.5f;
    
    if(newPosition.x > designResolutionSize.width * 0.8f)
        newPosition.x = designResolutionSize.width * 0.8f;
    
    if(getActionByTag(kActionJumpTag) == NULL)
        newPosition.y = playerY + getContentSize().height * 0.5f;
    
    setPosition(newPosition);
    _updateShadow();

}

CCRect BaseVehicle::getGroundCollision()
{
    float x = boundingBox().getMinX() + getContentSize().width * 0.4f;
    float y = playerY;
    
    float width = getContentSize().width * 0.5f;
    float height = getContentSize().height * 0.3f;
    
    CCRect rect = CCRectMake(x, y, width, height);
    
    return rect;
}

CCRect BaseVehicle::getAirCollision()
{
    float x = boundingBox().getMaxX() - getContentSize().width * 0.2f;
    float y = boundingBox().getMinY() + getContentSize().height * 0.15f;
    float size = getContentSize().width * 0.2f;
    
    CCRect rect;
    rect.setRect(x, y, size, size);
    
    return rect;
}

void BaseVehicle::_updateShadow()
{
    _spShadow->setPositionY(playerY - getPositionY() + getContentSize().height * 0.55f);
}