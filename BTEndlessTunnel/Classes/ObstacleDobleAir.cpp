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

ObstacleDobleAir::ObstacleDobleAir() : BaseObstacle("obstaculo_1_c.png")
{
    sameCollisionArea = false;
    
    CCRect collideArea;
    
    collideArea = CCRect(getContentSize().width * 0.05f, getContentSize().height * 0.65f, getContentSize().width * 0.2f, getContentSize().height * 0.25f);
    vCollision.push_back(collideArea);

    collideArea = CCRect(getContentSize().width * 0.2f, getContentSize().height * 0.5f, getContentSize().width * 0.2f, getContentSize().height * 0.25f);
    vCollision.push_back(collideArea);
    
    collideArea = CCRect(getContentSize().width * 0.3f, getContentSize().height * 0.35f, getContentSize().width * 0.2f, getContentSize().height * 0.25f);
    vCollision.push_back(collideArea);
    
    collideArea = CCRect(getContentSize().width * 0.4f, getContentSize().height * 0.25f, getContentSize().width * 0.2f, getContentSize().height * 0.25f);
    vCollision.push_back(collideArea);
    
    collideArea = CCRect(getContentSize().width * 0.5f, getContentSize().height * 0.1f, getContentSize().width * 0.2f, getContentSize().height * 0.25f);
    vCollision.push_back(collideArea);
    
    _spShadow = CCSprite::create("sombra_obstaculo_1c.png");
    _spShadow->setPosition(ccp(_spShadow->getContentSize().width * 0.6f, -_spShadow->getContentSize().height * 0.4f));
    addChild(_spShadow, -1);
    

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