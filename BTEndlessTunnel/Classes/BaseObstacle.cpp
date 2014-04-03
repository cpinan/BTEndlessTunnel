//
//  BaseObstacle.cpp
//  BTEndlessTunnel
//
//  Created by NSS on 3/27/14.
//
//

#include "BaseObstacle.h"

using namespace cocos2d;

BaseObstacle::BaseObstacle(std::string filename)
{
    if(initWithFile(filename.c_str()))
    {
        isObjectAlerted = false;
        passPlayerSFX = false;
        sameCollisionArea = false;
        distanceObjects = 0;
        numObjects = 1;
        setTag(1);
    }
}

BaseObstacle::~BaseObstacle()
{
    
}

bool BaseObstacle::collision(BaseVehicle& vehicle)
{
    
    CCRect rectAir = vehicle.getAirCollision();
    CCRect rectFloor = vehicle.getGroundCollision();
    
    int i;
    CCRect area;
    
    for(i = 0; i < vCollision.size(); i++)
    {
        area = currentCollisionArea(vCollision[i]);
        if(area.intersectsRect(rectAir) && area.intersectsRect(rectFloor))
        {
            return true;
        }
    }
    
    
    return false;
}

CCRect BaseObstacle::currentCollisionArea(CCRect area)
{
    
    float left, top, right, bottom;
    float width, height;
    
    left = area.getMinX() + getPositionX() - getContentSize().width * 0.5f;
    top = area.getMinY() + getPositionY() - getContentSize().height * 0.5f;
    right = area.getMaxX() + getPositionX() - getContentSize().width * 0.5f;
    bottom = area.getMaxY() + getPositionY() - getContentSize().height * 0.5f;
    
    width = right - left;
    height = bottom - top;
    
    CCRect newArea = CCRect(left, top, width, height);

    
    return newArea;
}