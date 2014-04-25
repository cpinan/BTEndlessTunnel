//
//  SettingsLayer.h
//  BTEndlessTunnel
//
//  Created by NSS on 4/1/14.
//
//

#ifndef __BTEndlessTunnel__SettingsLayer__
#define __BTEndlessTunnel__SettingsLayer__

#include "cocos2d.h"

enum SettingsTags
{
    kTagSettingJoypadMode = 0,
    kTagSettingAccelMode = 1,
    kTagSettingGoHome = 2
};

class SettingsLayer : public cocos2d::CCLayerColor
{
public:
    SettingsLayer();
    static cocos2d::CCScene* scene();
    
private:
    void _onOptionPressed(cocos2d::CCObject* pSender);
    cocos2d::CCMenuItemImage* itemJoypad;
    cocos2d::CCMenuItemImage* itemAccel;
};

#endif /* defined(__BTEndlessTunnel__SettingsLayer__) */
