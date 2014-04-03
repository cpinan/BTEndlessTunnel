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
    kTagGoHome = 1
};

class PopUpLoseLayer : public cocos2d::CCLayer
{
public:
    PopUpLoseLayer();
    
private:
    void _onOptionPressed(cocos2d::CCObject* pSender);
    void _finishHideLayer();
    
private:
    bool disable;
};

#endif /* defined(__BTEndlessTunnel__PopUpLoseLayer__) */
