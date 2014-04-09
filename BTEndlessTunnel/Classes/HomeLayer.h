//
//  HomeLayer.h
//  BTEndlessTunnel
//
//  Created by NSS on 4/1/14.
//
//

#ifndef __BTEndlessTunnel__HomeLayer__
#define __BTEndlessTunnel__HomeLayer__

#include "cocos2d.h"
#include "GameLayer.h"

enum HomeButtons
{
    kTagEasyMode = 0,
    kTagNormalMode = 1,
    kTagHardMode = 2,
    kTagCredits = 3,
    kTagLeaderboard = 4,
    kTagAchievements = 5,
    kTagSettings = 6
};

class HomeLayer : public cocos2d::CCLayer
{
public:
    HomeLayer(GameLayer* gameLayer);
    virtual ~HomeLayer();
    
private:
    void _onOptionPressed(cocos2d::CCObject* pSender);
    void _finishHideLayer();
    
private:
    bool disable;
    GameLayer* _gameLayer;
};

#endif /* defined(__BTEndlessTunnel__HomeLayer__) */
