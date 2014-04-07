//
//  GameLayer.h
//  BTEndlessTunnel
//
//  Created by NSS on 3/27/14.
//
//

#ifndef __BTEndlessTunnel__GameLayer__
#define __BTEndlessTunnel__GameLayer__

enum GameMode
{
    kGameModeHome = 0,
    kGameModePlay = 1,
    kGameModeEnd = 2,
    kGameModeReplayView = 3,
    kGameModePlayAgain = 4
};

enum GameState
{
    kGameStarting = 0,
    kGamePreparing = 1,
    kGameReady = 2,
    kGamePause = 3,
    kGameFinish = 4
};

enum GameLevel
{
    kGameLevelEasy = 0,
    kGameLevelNormal = 1,
    kGameLevelHard = 2,
    kGameLevelStory = 3,
    kGameLevelNone = 4
};

enum GameDeep
{
    kDeepBackground = -999999,
    kDeepGameElements = -9999,
    kDeepScore = 999,
    kDeepPauseLayer = 9999,
    kDeepPopUpLoseLayer = 99999
};

#define kScoreFactor 100.0f

#define AIR_AND_TOP_Y 160

#include "cocos2d.h"

#include "BaseVehicle.h"
#include "AppMacros.h"
#include "HudLayer.h"
#include "PauseLayer.h"
#include "PopUpLoseLayer.h"
#include "BaseObstacle.h"

class GameLayer : public cocos2d::CCLayer
{
public:
    
    GameLayer(HudLayer* hudLayer, GameMode gameMode = kGameModeHome, GameLevel = kGameLevelNone);
    virtual ~GameLayer();
    
    void configureGame(GameLevel gameLevel = kGameLevelEasy);
    void pauseGame();
    void resumeGame();
    void playGame();
    void runGame();
    void update(float dt);
    void didAccelerate(cocos2d::CCAcceleration *pAccelerationValue);
    
    void draw();
    
    void onEnterTransitionDidFinish();
    
    void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    
private:
    void _createMap();
    void _initLayers();
    void _createPlayer();
    void _initElements();
    
    void _createObstacle(float x);
    void _createMultipleObstacles(float x, int type);
    
    void _resumeEvents();
    void _removeNode(cocos2d::CCNode* node);
    void _gameLogic(float dt);
    
    void _playAgain();
    
    void _goHome();
    
    void _pauseAllActions();
    void _resumeAllActions();
    
    
private:
    BaseVehicle* _player;    
    
    HudLayer* _hudLayer;
    PauseLayer* _pauseLayer;
    PopUpLoseLayer* _popUpLoseLayer;
    
    GameState _gameState;
    GameState _previousGameState;
    
    GameMode _gameMode;
    GameLevel _gameLevel;
    
    cocos2d::CCArray* _parallaxBackground;
    cocos2d::CCArray* _parallaxRoof;
    cocos2d::CCArray* _parallaxFloor;
    
    cocos2d::CCArray* _arrayObstacles;
    
    cocos2d::CCMenuItemImage* _menuPause;
    
    bool _pause;
    bool _gameOver;
    bool _isJoypad;
    
    float _score;
    float _worldSpeed;
    float _minDistanceObstaclesX;
    
    int _itemMap;
    
    cocos2d::CCLabelTTF* _lblScore;
    cocos2d::CCPoint _accelVelocity;
    
private:
    BaseObstacle* _testObstacle;
    
    
};

#endif /* defined(__BTEndlessTunnel__GameLayer__) */
