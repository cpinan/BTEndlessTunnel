//
//  HomeScene.cpp
//  BTEndlessTunnel
//
//  Created by NSS on 4/1/14.
//
//

#include "HomeScene.h"

using namespace cocos2d;

HomeScene::HomeScene(GameMode gameMode, GameLevel gameLevel)
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
        HomeLayer* homeLayer = new HomeLayer(gameLayer);
        homeLayer->autorelease();
        addChild(homeLayer, kZHomeLayer);
    }
}

HomeScene* HomeScene::scene(GameMode gameMode, GameLevel gameLevel)
{
    HomeScene* scene = new HomeScene(gameMode, gameLevel);
    scene->autorelease();
    return scene;
}

