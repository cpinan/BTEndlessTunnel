//
//  PopUpLoseLayer.h
//  BTEndlessTunnel
//
//  Created by NSS on 4/1/14.
//
//

#ifndef __BTEndlessTunnel__PopUpLoseLayer__
#define __BTEndlessTunnel__PopUpLoseLayer__

#include "cocos2d.h"

enum PopUpLoseButtons
{
    kTagPlayAgain = 0,
    kTagGoHome = 1,
    kTagSendScore = 2
};

class PopUpLoseLayer : public cocos2d::CCLayerColor
{
public:
    PopUpLoseLayer();
    void updateScore(int level, float score, int obstaclesAvoided);
    
private:
    void _onOptionPressed(cocos2d::CCObject* pSender);
    void _finishHideLayer();
    
private:
    const char* _leaderboardID;
    
    int _obstaclesAvoided;
    int _level;
    long _score;
    
    cocos2d::CCLabelTTF* _lblScore;
    cocos2d::CCLabelTTF* _lblMaxScore;
    cocos2d::CCSprite* spBadge;
    bool disable;
};

#endif /* defined(__BTEndlessTunnel__PopUpLoseLayer__) */
