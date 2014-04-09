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

class PopUpLoseLayer : public cocos2d::CCLayer
{
public:
    PopUpLoseLayer();
    void updateScore(int level, float score);
    
private:
    void _onOptionPressed(cocos2d::CCObject* pSender);
    void _finishHideLayer();
    
private:
    long _score;
    cocos2d::CCLabelTTF* _lblHeadTitle;
    cocos2d::CCLabelTTF* _lblScore;
    cocos2d::CCLabelTTF* _lblMaxScore;
    cocos2d::CCLabelTTF* _lblTotalScore;
    bool disable;
};

#endif /* defined(__BTEndlessTunnel__PopUpLoseLayer__) */
