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
    bool collision(BaseVehicle& vehicle);
};

#endif /* defined(__BTEndlessTunnel__ObstacleDobleAir__) */
