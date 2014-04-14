//
//  ObstacleSimple.cpp
//  BTEndlessTunnel
//
//  Created by NSS on 3/27/14.
//
//

#include "ObstacleSimple.h"

using namespace cocos2d;

ObstacleSimple::ObstacleSimple() : BaseObstacle("wall_02.png")
{
    sameCollisionArea = true;
    obstacType = kSimpleObstacle;
    
    // CCRect collideArea = CCRect(0, 0, getContentSize().width, getContentSize().height);
    CCRect collideArea = CCRect(getContentSize().width * 0.25f, getContentSize().height * 0.1f,getContentSize().width * 0.6f, getContentSize().height * 0.8f);
    
    vCollision.push_back(collideArea);
}

bool ObstacleSimple::collision(BaseVehicle &vehicle)
{
    float top, bottom;
    CCRect area = boundingBox();
    
    top = area.getMinY() + getContentSize().height * 0.0f;
    bottom = top + getContentSize().height * 0.37f;
    
    float y = vehicle.getGroundCollision().getMinY() + (vehicle.getGroundCollision().getMaxY() - vehicle.getGroundCollision().getMinY()) * 0.25f;
    
    y = vehicle.getPlayerY() + vehicle.getContentSize().height * 0.3f * 0.5f;
    
    if(y < top || y > bottom)
        return false;
    
    // CCLog("Pass %f ; %f ; %f", y, top, bottom);
    
    return BaseObstacle::collision(vehicle);
}