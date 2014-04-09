//
//  ObstacleSimple.h
//  BTEndlessTunnel
//
//  Created by NSS on 3/27/14.
//
//

#ifndef __BTEndlessTunnel__ObstacleSimple__
#define __BTEndlessTunnel__ObstacleSimple__

#include "BaseObstacle.h"

class ObstacleSimple : public BaseObstacle
{
public:
    ObstacleSimple();
    bool collision(BaseVehicle& vehicle);
};

#endif /* defined(__BTEndlessTunnel__ObstacleSimple__) */
