//
//  ObstacleDobleAir.cpp
//  BTEndlessTunnel
//
//  Created by NSS on 3/27/14.
//
//

#include "ObstacleDobleAir.h"
#include "Constants.h"

using namespace cocos2d;

ObstacleDobleAir::~ObstacleDobleAir()
{
    removeAllChildren();
}

ObstacleDobleAir::ObstacleDobleAir() : BaseObstacle("wall_03.png")
{
    sameCollisionArea = false;
    
    /*
    CCRect collideArea1 = CCRect(getContentSize().width * 0.1f, getContentSize().height * 0.5f, getContentSize().width * 0.5f, getContentSize().height * 0.5f);
    
    CCRect collideArea2 = CCRect(getContentSize().width * 0.5f, getContentSize().height * 0.0f, getContentSize().width * 0.5f, getContentSize().height * 0.5f);
    */
    
    CCRect collideArea;
    
    collideArea = CCRect(getContentSize().width * 0.05f, getContentSize().height * 0.65f, getContentSize().width * 0.2f, getContentSize().height * 0.25f);
    vCollision.push_back(collideArea);

    collideArea = CCRect(getContentSize().width * 0.2f, getContentSize().height * 0.5f, getContentSize().width * 0.2f, getContentSize().height * 0.25f);
    vCollision.push_back(collideArea);
    
    collideArea = CCRect(getContentSize().width * 0.4f, getContentSize().height * 0.35f, getContentSize().width * 0.2f, getContentSize().height * 0.25f);
    vCollision.push_back(collideArea);
    
    collideArea = CCRect(getContentSize().width * 0.6f, getContentSize().height * 0.2f, getContentSize().width * 0.2f, getContentSize().height * 0.25f);
    vCollision.push_back(collideArea);
    
    _spShadow = CCSprite::create(SP_SHADOW);
    _spShadow->setPosition(ccp(_spShadow->getContentSize().width * 0.8f, -_spShadow->getContentSize().height * 1.25f));
    _spShadow->setOpacity(128);
    addChild(_spShadow, -1);
    
    //vCollision.push_back(collideArea2);

}

bool ObstacleDobleAir::collision(BaseVehicle &vehicle)
{
    if(vehicle.getState() != kStateJump)
        return false;
    
    float y = vehicle.getPositionY() - vehicle.getPlayerY() - vehicle.getContentSize().height * 0.5f;
    if(y < MAX_PLAYER_JUMP * 0.45f)
        return false;
    
    CCRect rectAir = vehicle.getAirCollision();
    
    int i;
    CCRect area;
    
    for(i = 0; i < vCollision.size(); i++)
    {
        area = currentCollisionArea(vCollision[i]);
        if(area.intersectsRect(rectAir))
        {
            return true;
        }
    }
    
    return false;
}

void ObstacleDobleAir::doUpdate(float x, float speed)
{
    BaseObstacle::doUpdate(x, speed);
    _spShadow->setPositionX(_spShadow->getPositionX() - speed);
}