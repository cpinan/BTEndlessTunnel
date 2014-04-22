//
//  PauseLayer.h
//  BTEndlessTunnel
//
//  Created by NSS on 4/1/14.
//
//

#ifndef __BTEndlessTunnel__PauseLayer__
#define __BTEndlessTunnel__PauseLayer__

#include "cocos2d.h"

enum PauseButtons
{
    kTagPauseResumeGame = 0,
    kTagPauseGoHome = 1,
    kTagPausePlayAgain = 2
};

class PauseLayer : public cocos2d::CCLayerColor
{
public:
    PauseLayer();
    
private:
    void _onOptionPressed(cocos2d::CCObject* pSender);
};

#endif /* defined(__BTEndlessTunnel__PauseLayer__) */
