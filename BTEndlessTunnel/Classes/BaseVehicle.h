//
//  BaseVehicle.h
//  BTEndlessTunnel
//
//  Created by NSS on 3/27/14.
//
//

#ifndef __BTEndlessTunnel__BaseVehicle__
#define __BTEndlessTunnel__BaseVehicle__

#include "cocos2d.h"

#define MAX_PLAYER_JUMP 140.0f

enum
{
    kActionJumpTag = 1000,
    kActionJumpAnimationTag = 1001,
    kActionIdleAnimationTag = 1002
};

enum ActorStates
{
    kStateNothing = 0,
    kStateIdle = 1,
    kStateJump = 2,
    kStateRun = 3,
    kStateBack = 4
};

class BaseVehicle : public cocos2d::CCSprite
{
public:
    BaseVehicle(std::string filename);
    virtual ~BaseVehicle();
    
    virtual void dead();
    virtual void doJump();
    virtual void doMove(cocos2d::CCPoint velocity);
    virtual void setLimits(float limitBotY, float height);
    
    virtual cocos2d::CCRect getGroundCollision();
    virtual cocos2d::CCRect getAirCollision();
    
    
    CC_SYNTHESIZE(ActorStates, state, State);
    CC_SYNTHESIZE(float, playerY, PlayerY);
    CC_SYNTHESIZE(cocos2d::CCTexture2D*, defaultTexture, DefaultTexture);
    CC_SYNTHESIZE(cocos2d::CCTexture2D*, deadTexture, DeadTexture);
    CC_SYNTHESIZE(cocos2d::CCAction*, jumpAnimation, JumpAnimation);
    CC_SYNTHESIZE(cocos2d::CCAction*, idleAnimation, IdleAnimation);
    CC_SYNTHESIZE(float, speed, Speed);
    
private:
    void _updateShadow();
    
private:
    cocos2d::CCJumpBy* _jumpByAction;
    cocos2d::CCSprite* _spShadow;
    
    float _limitBottomY;
    float _limitTopY;
    
};

#endif /* defined(__BTEndlessTunnel__BaseVehicle__) */
