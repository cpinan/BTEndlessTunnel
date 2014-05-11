//
//  HomeScene.cpp
//  BTEndlessTunnel
//
//  Created by NSS on 4/1/14.
//
//

#include "HomeScene.h"
#include "Constants.h"

using namespace cocos2d;

HomeScene::HomeScene(GameMode gameMode, GameLevel gameLevel, bool showAds)
{
    
    HudLayer* hudLayer = new HudLayer();
    hudLayer->setVisible(false);
    hudLayer->autorelease();
    addChild(hudLayer, kZHudLayer);
    
    GameLayer* gameLayer = new GameLayer(hudLayer, gameMode, gameLevel);
    gameLayer->autorelease();
    addChild(gameLayer, kZGameLayer);
    
    if(gameMode == kGameModeHome)
    {
        HomeLayer* homeLayer = new HomeLayer(gameLayer, showAds);
        homeLayer->setVisible(!TEST_OBSTACLE);
        homeLayer->autorelease();
        addChild(homeLayer, kZHomeLayer);
    }
}

HomeScene* HomeScene::scene(GameMode gameMode, GameLevel gameLevel, bool showAds)
{
    HomeScene* scene = new HomeScene(gameMode, gameLevel, showAds);
    scene->autorelease();
    return scene;
}

