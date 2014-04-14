//
//  BaseObstacle.h
//  BTEndlessTunnel
//
//  Created by NSS on 3/27/14.
//
//

#ifndef __BTEndlessTunnel__BaseObstacle__
#define __BTEndlessTunnel__BaseObstacle__

#include "cocos2d.h"
#include "BaseVehicle.h"

enum ObstacleType
{
    kNormalObstacle = 0,
    kJumpObstacle = 1,
    kSimpleObstacle = 2
};

class BaseObstacle : public cocos2d::CCSprite
{
public:
    BaseObstacle(std::string file);
    virtual ~BaseObstacle();
    
    CC_SYNTHESIZE_READONLY(bool, sameCollisionArea, SameCollisionArea);
    CC_SYNTHESIZE(bool, passPlayerSFX, PassPlayerSFX);
    
    CC_SYNTHESIZE(std::vector<cocos2d::CCRect>, vCollision, VCollision);
    CC_SYNTHESIZE(bool, isObjectAlerted, IsObjectAlerted);
    
    CC_SYNTHESIZE(int, numObjects, NumObjects);
    CC_SYNTHESIZE(float, distanceObjects, DistanceObjects);
    
    CC_SYNTHESIZE(ObstacleType, obstacType, ObstacType);
    
    virtual bool collision(BaseVehicle& vehicle);
    virtual cocos2d::CCRect currentCollisionArea(cocos2d::CCRect area);
    
    void doUpdate(float x, float speed);
    
};

#endif /* defined(__BTEndlessTunnel__BaseObstacle__) */
