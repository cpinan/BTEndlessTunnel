//
//  HomeScene.h
//  BTEndlessTunnel
//
//  Created by NSS on 4/1/14.
//
//

#ifndef __BTEndlessTunnel__HomeScene__
#define __BTEndlessTunnel__HomeScene__

#include "cocos2d.h"

#include "GameLayer.h"
#include "HudLayer.h"
#include "HomeLayer.h"

enum
{
    kZHomeLayer = 99999,
    kZHudLayer = 9999,
    kZGameLayer = 999
};

class HomeScene : public cocos2d::CCScene
{
private:
    HomeScene(GameMode gameMode, GameLevel gameLevel = kGameLevelNone);
public:
    static HomeScene* scene(GameMode gameMode, GameLevel gameLevel = kGameLevelNone);
};

#endif /* defined(__BTEndlessTunnel__HomeScene__) */
