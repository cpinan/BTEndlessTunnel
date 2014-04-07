//
//  ObstacleDobleAir.h
//  BTEndlessTunnel
//
//  Created by NSS on 3/27/14.
//
//

#ifndef __BTEndlessTunnel__ObstacleDobleAir__
#define __BTEndlessTunnel__ObstacleDobleAir__

#include "BaseObstacle.h"

class ObstacleDobleAir : public BaseObstacle
{
public:
    ObstacleDobleAir();
    ~ObstacleDobleAir();
    bool collision(BaseVehicle& vehicle);
    void doUpdate(float x, float speed);
    
private:
    cocos2d::CCSprite* _spShadow;
};

#endif /* defined(__BTEndlessTunnel__ObstacleDobleAir__) */
