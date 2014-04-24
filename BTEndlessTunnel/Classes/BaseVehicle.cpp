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
#include "Constants.h"

using namespace cocos2d;
using namespace CocosDenshion;

BaseVehicle::BaseVehicle(std::string filename)
{
    if(initWithFile(filename.c_str()))
    {
        _limitBottomY = 0;
        _limitTopY = 0;
        
        defaultTexture = NULL;
        jumpAnimation = NULL;
        idleAnimation = NULL;
        deadTexture = NULL;
        speed = 11.0f;
        state = kStateIdle;
        
        playerY = getPositionY() - getContentSize().height * 0.5f;
        
        _jumpByAction = CCJumpBy::create(0.6f, ccp(0, 0), MAX_PLAYER_JUMP, 1);
        _jumpByAction->setTag(kActionJumpTag);
        _jumpByAction->retain();
        
        _spShadow = CCSprite::create(SP_SHADOW);
        _spShadow->setPositionX(getContentSize().width * 0.5f);
        addChild(_spShadow, -1);
        _updateShadow();
        
    }
}

BaseVehicle::~BaseVehicle()
{
    CC_SAFE_RELEASE(_jumpByAction);
}

void BaseVehicle::doJump()
{
    int y = getPositionY() - getPlayerY() - getContentSize().height * 0.5f;
    if(y <= 1 && state != kStateJump/* && getActionByTag(kActionJumpTag) == NULL*/)
    {
        state = kStateJump;
        if(jumpAnimation != NULL && idleAnimation != NULL)
        {
            stopAction(idleAnimation);
            runAction(jumpAnimation);
        }
        runAction(_jumpByAction);
        SimpleAudioEngine::sharedEngine()->playEffect(SFX_JUMP);
    }
}

void BaseVehicle::dead()
{
    if(deadTexture != NULL)
    {
        stopAllActions();
        setTexture(deadTexture);
        SimpleAudioEngine::sharedEngine()->playEffect(SFX_SMASH);
        
        //float x = getPositionX() + WIN_SIZE.width * 1.5f;
        //runAction(CCMoveTo::create(1.5f, ccp(x, getPositionY())));
        runAction(CCBlink::create(1.5f, 8));
        if(state == kStateJump)
        {
            float time = 1.0f;
            float times = 30;
            float y = _spShadow->getPositionY() + getPositionY();
            runAction(CCMoveTo::create(time, ccp(getPositionX() + getContentSize().width * 0.15f, y)));
            schedule(schedule_selector(BaseVehicle::_updateEnd), time / times, times, 0);
        }
    }
}

void BaseVehicle::_updateEnd()
{
    _updateShadow();
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
        if(playerY + velocity.y > _limitTopY)
            velocity.y = 0;
        else if(playerY + velocity.y < _limitBottomY)
            velocity.y = 0;
    }
    
    CCPoint newPosition = ccpAdd(getPosition(), velocity);
    
    playerY += velocity.y;
    
    if(playerY > _limitTopY)
        playerY = _limitTopY;
    else if(playerY < _limitBottomY)
        playerY = _limitBottomY;
    
    if(newPosition.x < getContentSize().width * 0.5f)
        newPosition.x = getContentSize().width * 0.5f;
    
    if(newPosition.x > WIN_SIZE.width * 0.8f)
        newPosition.x = WIN_SIZE.width * 0.8f;
    
    if(getActionByTag(kActionJumpTag) == NULL)
        newPosition.y = playerY + getContentSize().height * 0.5f;
    
    setPosition(newPosition);
    _updateShadow();

}

CCRect BaseVehicle::getGroundCollision()
{
    float x = boundingBox().getMinX() + getContentSize().width * 0.3f;
    float y = playerY;
    
    float width = getContentSize().width * 0.55f;
    float height = getContentSize().height * 0.3f;
    
    CCRect rect = CCRectMake(x, y, width, height);
    
    return rect;
}

CCRect BaseVehicle::getAirCollision()
{
    float x = boundingBox().getMinX() + getContentSize().width * 0.3f;
    float y = boundingBox().getMinY() + getContentSize().height * 0.16f;
    float w = getContentSize().width * 0.55f;
    float h = getContentSize().width * 0.2f;
    
    CCRect rect;
    rect.setRect(x, y, w, h);
    
    return rect;
}

void BaseVehicle::_updateShadow()
{
    _spShadow->setPositionY(playerY - getPositionY() + getContentSize().height * 0.55f);
}

void BaseVehicle::setLimits(float limitBotY, float height)
{
    _limitBottomY = limitBotY;
    _limitTopY = _limitBottomY + height;
}