//
//  GameLayer.cpp
//  BTEndlessTunnel
//
//  Created by NSS on 3/27/14.
//
//

#include "GameLayer.h"

#include "ObstacleSimple.h"
#include "ObstacleDoble.h"
#include "ObstacleDobleAir.h"

#include "VehicleFrog.h"
#include "Constants.h"

#include "SimpleAudioEngine.h"

#include "HomeScene.h"

#include "LocalStorageManager.h"
#include "Utils.h"
#include "PlayGameConstants.h"

#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>

#include "NativeUtils.h"

#define SP_PISTA "pista.png"
#define SP_CIELO "cielo.png"
#define SP_NUBE "nube.png"
#define SP_BG_BACK "background_2.png"
#define SP_BG_MID "background_1.png"
#define SP_BG_FRONT "humo.png"

#define DT_SPEED_PISTA 1.0f
#define DT_SPEED_OBSTACULOS (DT_SPEED_PISTA * 1.0f)
#define DT_SPEED_NUBE (DT_SPEED_PISTA * 0.2f)
#define DT_SPEED_BG_BACK (DT_SPEED_PISTA * 0.5f)
#define DT_SPEED_BG_MID (DT_SPEED_PISTA * 1.0f)
#define DT_SPEED_BG_FRONT (DT_SPEED_PISTA * 1.3f)
#define MIN_COLOR 100
#define MAX_COLOR 255
#define LIGHT_TIME 5.0f

using namespace cocos2d;
using namespace CocosDenshion;
using namespace std;

#pragma mark - Level Definition, Constructor and Destructor

// Level definition
/*
 0 = Obstaculo Simple Abajo
 1 = Obstaculo Simple Arriba
 2 = Obstaculo Doble en Tierra
 3 = Obstaculo Doble en Aire
 4 = x2 Obstaculo Simple Abajo
 5 = x2 Obstaculo Simple Arriba
 6 = x3 Obstaculo Doble en Tierra
 7 = x3 Obstaculo Doble en Aire
 8 = x2 Obstaculo Doble en Tierra
 9 = x2 Obstaculo Doble en Aire
*/

int easyMap[] = {
    0,0,1,1,0,0,1,1,0,0,1,1,2,0,1,0,1,0,1,
    2,3,3,2,3,3,2,3,2,0,0,1,0,0,0,1,0,1,0,
    1,0,0,0,1,1,1,0,1,1,3,2,3,3,3,2,3,1,8,
    9,0,1,0,1,2,0,0,1,1,2,0,1,2,0,0,8,0,0,
    1,1,9,1,0,1,2,9,1,2,9,0,1,2,1,0,2,1,2,
    0,0,1,0,0,1,1,0,1,1,0,0,1,4,5,0,1,3,8,
    0,0,1,1,0,1,1,0,1,0,1,0,1,0,1,0,1,3,2
};

int normalMap[] = {
    3,2,1,1,0,0,1,0,1,0,1,0,1,2,3,2,2,3,3,
    0,1,0,1,5,4,5,4,5,4,2,5,4,2,5,4,2,5,4,
    3,2,0,1,0,8,9,2,0,1,4,5,4,5,8,0,1,0,2,
    3,3,2,2,3,8,0,1,0,1,4,5,4,5,2,4,5,3,2,
    0,1,1,0,0,1,1,0,0,1,2,3,2,2,3,3,8,9,6,
    3,7,0,0,1,1,4,5,5,4,2,3,2,1,0,1,0,1,0,
    3,2,3,2,3,3,8,3,8,3,2,9,2,0,2,1,2,0,2
};

int hardMap[] = {
    6,7,8,7,6,9,5,2,4,2,5,3,4,3,8,9,0,1,0,
    1,2,9,8,3,4,5,4,5,4,5,4,5,4,5,0,1,0,1,
    8,3,6,3,8,3,2,3,8,9,6,7,0,2,1,2,0,3,9,
    6,9,6,9,8,9,3,8,9,2,9,9,2,9,9,2,4,5,8,
    9,3,8,9,8,3,3,4,5,4,5,0,1,0,1,2,3,3,8,
    6,9,6,9,6,0,1,6,9,3,6,0,9,8,9,6,8,0,9,
    6,7,6,7,6,3,3,6,3,8,9,8,9,8,1,8,0,8,1
};

vector<int> _vectorMap;
vector<MusicPlaying> _vectorMusics;
MusicPlaying _music;

// End Level definition

GameLayer::GameLayer(HudLayer* hudLayer, GameMode gameMode, GameLevel gameLevel) : _hudLayer(hudLayer), _gameMode(gameMode)
{
    srand(time(0));
    
    _lightningAnimation = NULL;
    
    _color = 255;
    _colorSign = -1;
    _lightningTimer = 0;
    
    _selectRandomMusic();
    // _preloadLightning();
    
    _player = NULL;
    _obstaclesJumped = 0;
    _obstaclesAvoided = 0;
    _isJoypad = true;
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GameLayer::pauseGame), NOTIFICATION_PAUSE_GAME, NULL);
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GameLayer::resumeGame), NOTIFICATION_RESUME_GAME, NULL);
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GameLayer::_playAgain), NOTIFICATION_PLAY_AGAIN, NULL);
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GameLayer::_goHome), NOTIFICATION_GO_HOME, NULL);
    
    _createMap();
    _initLayers();
    _gameLevel = gameLevel;
    
    if(TEST_OBSTACLE)
    {
        
        BaseObstacle* obs;
        
        obs = new ObstacleSimple();
        obs->autorelease();
        obs->setTag(1000);
        obs->setPositionX(WIN_SIZE.width * 0.2f);
        obs->setPositionY(OBSTACLE_SIMPLE_BOT_Y);
        addChild(obs, -100);
        
        obs = new ObstacleSimple();
        obs->autorelease();
        obs->setTag(1001);
        obs->setPositionX(WIN_SIZE.width * 0.4f);
        obs->setPositionY(OBSTACLE_SIMPLE_TOP_Y);
        addChild(obs, -100);
        
        obs = new ObstacleDoble();
        obs->autorelease();
        obs->setTag(1002);
        obs->setPositionX(WIN_SIZE.width * 0.6f);
        obs->setPositionY(OBSTACLE_DOBLE_GROUND_Y);
        addChild(obs, -100);
        
        obs = new ObstacleDobleAir();
        obs->autorelease();
        obs->setTag(1003);
        obs->setPositionX(WIN_SIZE.width * 0.8f);
        obs->setPositionY(OBSTACLE_DOBLE_AIR_Y);
        addChild(obs, -100);
    }
    
}

void GameLayer::_preloadLightning()
{
    CCAnimation* animation = CCAnimation::create();
    animation->setDelayPerUnit(1.0 / 12.0f);
    animation->setRestoreOriginalFrame(false);
    
    animation->addSpriteFrameWithFileName("");
    
    _lightningAnimation = CCAnimate::create(animation);
    _lightningAnimation->retain();
}

void GameLayer::onEnterTransitionDidFinish()
{
    if(_gameMode == kGameModePlay || _gameMode == kGameModePlayAgain || _gameMode == kGameModeReplayView)
    {
        configureGame(_gameLevel);
        runGame();
    }
}

void GameLayer::_selectRandomMusic()
{
    MusicPlaying mp;
    
    mp.bg_music = BG_MUSIC_01;
    mp.description = "BT Turbo Tunnel - VGMusic.com";
    _vectorMusics.push_back(mp);
    
    mp.bg_music = BG_MUSIC_02;
    mp.description = "Music by Diego Rodriguez";
    _vectorMusics.push_back(mp);
    
    mp.bg_music = BG_MUSIC_03;
    mp.description = "Turtle Blues - PlayOnLoop.com";
    _vectorMusics.push_back(mp);
    
    random_shuffle(_vectorMusics.begin(), _vectorMusics.end());
    _music = _vectorMusics[0];
    
}

void GameLayer::_showAudioPlaying()
{
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    CCLabelTTF* lblMusic = CCLabelTTF::create(CCString::createWithFormat("Playing %s", _music.description)->getCString(), FONT_GAME, 35.0f, CCSizeMake(winSize.width * 0.85f, winSize.height * 0.15f), kCCTextAlignmentRight, kCCVerticalTextAlignmentTop);
    
    lblMusic->setPositionX(origin.x + visibleSize.width * 0.55f);
    lblMusic->setPositionY(origin.y - visibleSize.height * 0.25f);
    
    CCMoveTo* ac1 = CCMoveTo::create(1.0f, ccp(lblMusic->getPositionX(), origin.y + visibleSize.height * 0.05f));
    
    CCDelayTime* ac2 = CCDelayTime::create(2.1f);
    
    CCFadeOut* ac3 = CCFadeOut::create(0.9f);
    
    CCCallFuncN* ac4 = CCCallFuncN::create(this, callfuncN_selector(GameLayer::_removeNode));
    
    addChild(lblMusic, kDeepGameFinish);
    
    lblMusic->runAction(CCSequence::create(ac1, ac2, ac3, ac4, NULL));
    
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic(_music.bg_music, true);
    
}

GameLayer::~GameLayer()
{
    
    unscheduleAllSelectors();
    
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, NOTIFICATION_PAUSE_GAME);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, NOTIFICATION_RESUME_GAME);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, NOTIFICATION_PLAY_AGAIN);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, NOTIFICATION_GO_HOME);

    CC_SAFE_RELEASE(_parallaxSky);
    CC_SAFE_RELEASE(_parallaxBGBack);
    CC_SAFE_RELEASE(_parallaxBGMid);
    CC_SAFE_RELEASE(_parallaxBGFront);
    
    CC_SAFE_RELEASE(_parallaxFloor);
    CC_SAFE_RELEASE(_arrayObstacles);
    CC_SAFE_RELEASE(_lightningAnimation);
}

#pragma mark - Init layers, Create Players and Game Elements

void GameLayer::_createMap()
{
    CCPoint visibleOrigin = CCDirector::sharedDirector()->getVisibleOrigin();
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    
    int i = 0;
    float x = 0;
    float yPos = visibleOrigin.y * 0.5f;
    CCSprite* _tmpSprite;
    
    
    // Creamos el parallax para la pista
    _parallaxFloor = CCArray::createWithCapacity(MAX_PARALLAX);
    _parallaxFloor->retain();

    CCSprite* spFloor = CCSprite::create(SP_PISTA);
    _playerStartY = spFloor->getContentSize().height * 0.55f + yPos;
    _wallHeight = spFloor->getContentSize().height * 0.25f;
    
    // Creamos el cielo
    _parallaxSky = CCArray::createWithCapacity(2);
    _parallaxSky->retain();
    
    for(i = 0; i < 2; i++)
    {
        _tmpSprite = CCSprite::create(SP_CIELO);
        _tmpSprite->setAnchorPoint(CCPointZero);
        _tmpSprite->setPosition(ccp(x, WIN_SIZE.height - _tmpSprite->getContentSize().height));
        addChild(_tmpSprite, kDeepSky);
        x += _tmpSprite->getContentSize().width;
        _parallaxSky->addObject(_tmpSprite);
    }
    
    // Creamos la nube    
    _spCloud = CCSprite::create(SP_NUBE);
    _spCloud->setPosition(ccp(WIN_SIZE.width + _spCloud->getContentSize().width * 0.7f, visibleSize.height - _spCloud->getContentSize().height * 0.9f + visibleOrigin.y));
    addChild(_spCloud, kDeepCloud);
    
    // Creamos el BG que esta mas atras
    x = 0;
    _parallaxBGBack = CCArray::createWithCapacity(MAX_PARALLAX);
    _parallaxBGBack->retain();
    for(i = 0; i < MAX_PARALLAX; i++)
    {
        _tmpSprite = CCSprite::create(SP_BG_BACK);
        _tmpSprite->setAnchorPoint(CCPointZero);
        _tmpSprite->setPosition(ccp(x, _playerStartY + _wallHeight * 1.9f));
        addChild(_tmpSprite, kDeepBGBack);
        _parallaxBGBack->addObject(_tmpSprite);
        x += _tmpSprite->getContentSize().width;
    }
    
    // Creamos el BG que esta al medio
    x = 0;
    _parallaxBGMid = CCArray::createWithCapacity(MAX_PARALLAX);
    _parallaxBGMid->retain();
    for(i = 0; i < MAX_PARALLAX; i++)
    {
        _tmpSprite = CCSprite::create(SP_BG_MID);
        _tmpSprite->setAnchorPoint(CCPointZero);
        _tmpSprite->setPosition(ccp(x, _playerStartY + _wallHeight * 1.0f));
        addChild(_tmpSprite, kDeepBGMid);
        _parallaxBGMid->addObject(_tmpSprite);
        x += _tmpSprite->getContentSize().width;
    }
    
    // Creamos el BG que esta al frente
    x = 0;
    _parallaxBGFront = CCArray::createWithCapacity(MAX_PARALLAX);
    _parallaxBGFront->retain();
    for(i = 0; i < MAX_PARALLAX; i++)
    {
        _tmpSprite = CCSprite::create(SP_BG_FRONT);
        _tmpSprite->setAnchorPoint(CCPointZero);
        _tmpSprite->setPosition(ccp(x, _playerStartY + _wallHeight * 1.0f));
        addChild(_tmpSprite, kDeepBGFront);
        _parallaxBGFront->addObject(_tmpSprite);
        x += _tmpSprite->getContentSize().width;
    }
    
    // Pista
    x = 0;
    
    for(i = 0; i < MAX_PARALLAX; i++)
    {
        spFloor = CCSprite::create(SP_PISTA);
        spFloor->setAnchorPoint(ccp(0, 0));
        spFloor->setPosition(ccp(x, yPos));
        _parallaxFloor->addObject(spFloor);
        addChild(spFloor, kDeepTracks);
        x += spFloor->getContentSize().width;
    }
    
    OBSTACLE_SIMPLE_BOT_Y = _playerStartY + _wallHeight * 0.75f;
    OBSTACLE_DOBLE_GROUND_Y = _playerStartY + _wallHeight * 0.7f;
    OBSTACLE_SIMPLE_TOP_Y = _playerStartY + _wallHeight * 1.25f;
    OBSTACLE_DOBLE_AIR_Y = _playerStartY + _wallHeight * 1.8f;
    
}

void GameLayer::configureGame(GameLevel gameLevel)
{
    
    _gameLevel = gameLevel;
    
    _accelVelocity = CCPointZero;
    
    CCPoint visOrigin = CCDirector::sharedDirector()->getVisibleOrigin();
    CCSize visSize = CCDirector::sharedDirector()->getVisibleSize();
    
    _menuPause = CCMenuItemImage::create("pause_off.png", "pause.png", this, menu_selector(GameLayer::pauseGame));
    _menuPause->setVisible(false);
    _menuPause->setPositionX(visOrigin.x + _menuPause->getContentSize().width * 0.9f);
    _menuPause->setPositionY(visOrigin.y + visSize.height - _menuPause->getContentSize().width * 0.6f);
    
    CCMenu* menu = CCMenu::create();
    menu->setAnchorPoint(ccp(0, 0));
    menu->setPosition(CCPointZero);
    menu->addChild(_menuPause);
    addChild(menu, kDeepPauseLayer);
    
    setTouchEnabled(true);
    
     _worldSpeed = START_WORLD_SPEED;
    _minDistanceObstaclesX = MIN_DISTANCE_OBSTACLES;
    
    if(_gameLevel == kGameLevelEasy)
    {
        _worldSpeed *= 1.2f;
        _vectorMap.insert(_vectorMap.begin(), easyMap, easyMap + sizeof(easyMap) / sizeof(int));
    }
    else if(_gameLevel == kGameLevelNormal)
    {
        _minDistanceObstaclesX *= 0.9f;
        _worldSpeed *= 1.8f;
        _vectorMap.insert(_vectorMap.begin(), normalMap, normalMap + sizeof(normalMap) / sizeof(int));
    }
    else if(_gameLevel == kGameLevelHard)
    {
        _minDistanceObstaclesX *= 0.7f;
        _worldSpeed *= 2.4f;
        _vectorMap.insert(_vectorMap.begin(), hardMap, hardMap + sizeof(hardMap) / sizeof(int));
    }
    
    _itemMap = 0;
    
    _score = 0.0f;
    
    _gameState = kGameStarting;
    
    _pause = false;
    _gameOver = false;
    
    _createPlayer();
    _initElements();
    
}

void GameLayer::_initLayers()
{
    
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    CCSize size = CCDirector::sharedDirector()->getVisibleSize();
    
    _lblScore = CCLabelTTF::create("0", FONT_GAME, SIZE_SCORE_GAME, CCSizeMake(380, 70), kCCTextAlignmentRight, kCCVerticalTextAlignmentTop);
    _lblScore->setAnchorPoint(ccp(0, -0.5f));
    _lblScore->setVisible(false);
    _lblScore->setPosition(ccp(origin.x + size.width * 0.56f, origin.y + size.height * 0.82f));
    addChild(_lblScore, kDeepScore);
    
    _pauseLayer = new PauseLayer();
    _pauseLayer->setVisible(false);
    _pauseLayer->setPosition(ccp(0, -WIN_SIZE.height));
    _pauseLayer->setPositionY(0);
    _pauseLayer->autorelease();
    addChild(_pauseLayer, kDeepPauseLayer);
    
    _popUpLoseLayer = new PopUpLoseLayer();
    _popUpLoseLayer->setPosition(ccp(0, -WIN_SIZE.height));
    // _popUpLoseLayer->setPositionY(0);
    _popUpLoseLayer->autorelease();
    addChild(_popUpLoseLayer, kDeepPopUpLoseLayer);
    
}

void GameLayer::_createPlayer()
{
    _player = new VehicleFrog();
    _player->setLimits(_playerStartY - _wallHeight * 0.1f, _wallHeight * 0.9f);
    _player->setPositionY(_playerStartY + _wallHeight * 0.5f);
    _player->setPositionX(-_player->getContentSize().width * 2.5f);
    _player->autorelease();
    addChild(_player);
}

void GameLayer::_initElements()
{
    
    int i = 0;
    float x;
        
    // Obstacles
    _arrayObstacles = CCArray::create();
    _arrayObstacles->retain();
    
    x = START_X_OBSTACLES;
    
    for(i = 0; i < MAX_OBSTACLES; i++)
    {
        _createObstacle(x);
        
        BaseObstacle* lastObstacle = (BaseObstacle*) _arrayObstacles->lastObject();
        x = lastObstacle->getPositionX() + MIN_DISTANCE_OBSTACLES;
    }

}

#pragma mark - Create Obstacle

void GameLayer::_createObstacle(float x)
{
    BaseObstacle* obstacle = NULL;
    float y;
    int z = 0;
    
    int type = _vectorMap[_itemMap];
    
    if(type == 0)
    {
        obstacle = new ObstacleSimple();
        y = OBSTACLE_SIMPLE_BOT_Y;
        z = OBSTACLE_SIMPLE_BOT_Y;
    }
    else if(type == 1)
    {
        obstacle = new ObstacleSimple();
        y = OBSTACLE_SIMPLE_TOP_Y;
        z = OBSTACLE_SIMPLE_TOP_Y;
    }
    else if(type == 2)
    {
        obstacle = new ObstacleDoble();
        y = OBSTACLE_DOBLE_GROUND_Y;
        z = WIN_SIZE.height * 0.5f;
    }
    else if(type == 3)
    {
        obstacle = new ObstacleDobleAir();
        y = OBSTACLE_DOBLE_AIR_Y;
    }
    else
    {
        _createMultipleObstacles(x, type);
    }
    
    if(obstacle != NULL && type <= 3)
    {
        obstacle->setPosition(ccp(x, y));
        obstacle->autorelease();
        _arrayObstacles->addObject(obstacle);
        addChild(obstacle, (WIN_SIZE.height - z) + kDeepGameElements);
    }

    _itemMap++;
    if(_itemMap > _vectorMap.size() - 1)
        _itemMap = 0;
}

void GameLayer::_createMultipleObstacles(float x, int type)
{
    float y;
    int z = 0, i = 0;
    BaseObstacle* obstacle;
    
    if(type == 4)
    {
        // Crear 2 obstaculos simples abajo
        y = OBSTACLE_SIMPLE_BOT_Y;
        z = OBSTACLE_SIMPLE_BOT_Y;
        
        for(i = 0; i < 2; i++)
        {
            obstacle = new ObstacleSimple();
            obstacle->setNumObjects(0);
            if(i == 0)
            {
                obstacle->setNumObjects(2);
                obstacle->setDistanceObjects(_minDistanceObstaclesX * 0.5f);
            }
            
            obstacle->setTag((i - 1) * -1);
            obstacle->setPosition(ccp(x, y));
            obstacle->autorelease();
            _arrayObstacles->addObject(obstacle);
            addChild(obstacle, (WIN_SIZE.height - z) + kDeepGameElements);
            x += _minDistanceObstaclesX * 0.5f;
        }
        
    }
    else if(type == 5)
    {
        // Crear 2 obstaculos simples arriba
        y = OBSTACLE_SIMPLE_TOP_Y;
        z = OBSTACLE_SIMPLE_TOP_Y;
        
        for(i = 0; i < 2; i++)
        {
            obstacle = new ObstacleSimple();
            
            obstacle->setNumObjects(0);
            if(i == 0)
            {
                obstacle->setNumObjects(2);
                obstacle->setDistanceObjects(_minDistanceObstaclesX * 0.5f);
            }
            
            obstacle->setTag((i - 1) * -1);
            obstacle->setPosition(ccp(x, y));
            obstacle->autorelease();
            _arrayObstacles->addObject(obstacle);
            addChild(obstacle, (WIN_SIZE.height - z) + kDeepGameElements);
            x += _minDistanceObstaclesX * 0.5f;
        }
        
    }
    else if(type == 6)
    {
        // Crear 3 obstaculos dobles en tierra
        y = OBSTACLE_DOBLE_GROUND_Y;
        z = WIN_SIZE.height * 0.5f;
        
        for(i = 0; i < 3; i++)
        {
            obstacle = new ObstacleDoble();
            
            obstacle->setNumObjects(0);
            if(i == 0)
            {
                obstacle->setNumObjects(3);
                obstacle->setDistanceObjects(_minDistanceObstaclesX * 0.3f);
            }
            
            obstacle->setTag((i - 1) * -1);
            obstacle->setPosition(ccp(x, y));
            obstacle->autorelease();
            _arrayObstacles->addObject(obstacle);
            addChild(obstacle, (WIN_SIZE.height - z) + kDeepGameElements);
            x += _minDistanceObstaclesX * 0.3f;
        }
        
    }
    else if(type == 7)
    {
        // Crear 3 obstaculos dobles en el aire
        y = OBSTACLE_DOBLE_AIR_Y;
        
        for(i = 0; i < 3; i++)
        {
            obstacle = new ObstacleDobleAir();
            
            obstacle->setNumObjects(0);
            if(i == 0)
            {
                obstacle->setNumObjects(3);
                obstacle->setDistanceObjects(_minDistanceObstaclesX * 0.3f);
            }
            
            obstacle->setTag((i - 1) * -1);
            obstacle->setPosition(ccp(x, y));
            obstacle->autorelease();
            _arrayObstacles->addObject(obstacle);
            addChild(obstacle, (WIN_SIZE.height - z) + kDeepGameElements);
            x += _minDistanceObstaclesX * 0.3f;
        }
    }
    else if(type == 8)
    {
        // Crear 2 obstaculos dobles en tierra
        y = OBSTACLE_DOBLE_GROUND_Y;
        z = WIN_SIZE.height * 0.5f;
        
        for(i = 0; i < 2; i++)
        {
            obstacle = new ObstacleDoble();
            
            obstacle->setNumObjects(0);
            if(i == 0)
            {
                obstacle->setNumObjects(2);
                obstacle->setDistanceObjects(_minDistanceObstaclesX * 0.3f);
            }
            
            obstacle->setTag((i - 1) * -1);
            obstacle->setPosition(ccp(x, y));
            obstacle->autorelease();
            _arrayObstacles->addObject(obstacle);
            addChild(obstacle, (WIN_SIZE.height - z) + kDeepGameElements);
            x += _minDistanceObstaclesX * 0.3f;
        }
        
    }
    else if(type == 9)
    {
        // Crear 2 obstaculos dobles en el aire
        y = OBSTACLE_DOBLE_AIR_Y;
        
        for(i = 0; i < 2; i++)
        {
            obstacle = new ObstacleDobleAir();
            
            obstacle->setNumObjects(0);
            if(i == 0)
            {
                obstacle->setNumObjects(2);
                obstacle->setDistanceObjects(_minDistanceObstaclesX * 0.3f);
            }
            
            obstacle->setTag((i - 1) * -1);
            obstacle->setPosition(ccp(x, y));
            obstacle->autorelease();
            _arrayObstacles->addObject(obstacle);
            addChild(obstacle, (WIN_SIZE.height - z) + kDeepGameElements);
            x += _minDistanceObstaclesX * 0.3f;
        }
    }
    
}

#pragma mark - Accelerometer manager

void GameLayer::didAccelerate(CCAcceleration *pAccelerationValue)
{
    //float z = pAccelerationValue->z;
    
    if(!_pause && !_gameOver)
    {
        if(_gameState == kGameReady)
        {
            float x = pAccelerationValue->x * _player->getSpeed() * 0.5f;
            float y = pAccelerationValue->y * _player->getSpeed() * 0.5f;
            _accelVelocity = ccpMult(ccp(x,y), _player->getSpeed());
        }
    }
}

#pragma mark - Game States Manager

void GameLayer::playGame()
{
    _gameMode = kGameModePlay;
    runGame();
}

void GameLayer::runGame()
{    
    _isJoypad = LocalStorageManager::isUsingJoypad();
    
    if(!_isJoypad)
    {
        
        if(!LocalStorageManager::isAchievementUnlocked(ACH_PLAY_IN_ACCELEROMETER_MODE))
        {
            Utils::unlockAchievement(ACH_PLAY_IN_ACCELEROMETER_MODE);
            LocalStorageManager::unlockAchievement(ACH_PLAY_IN_ACCELEROMETER_MODE);
        }
        
        setAccelerometerEnabled(true);
    }
    
    unscheduleUpdate();
    scheduleUpdate();
}

void GameLayer::pauseGame()
{
    if(_gameState == kGameFinish)
        return;
    
    if(_gameMode == kGameModePlay || _gameMode == kGameModePlayAgain || _gameMode == kGameModeReplayView)
    {
        if(!_pause)
        {
            _pause = true;
            _previousGameState = _gameState;
            _gameState = kGamePause;
            
            pauseSchedulerAndActions();
            _pauseAllActions();
            
            _menuPause->setVisible(false);
            _hudLayer->setVisible(false);
            _pauseLayer->setVisible(true);
            
            NativeUtils::showAd();
        }
    }
}

void GameLayer::resumeGame()
{
    if(_gameState == kGameFinish)
        return;
    
    if(_gameMode == kGameModePlay || _gameMode == kGameModePlayAgain || _gameMode == kGameModeReplayView)
    {
        if(_gameState == kGamePause)
        {
            NativeUtils::hideAd();
            
            _gameState = _previousGameState;
            _hudLayer->setVisible(true);
            _pauseLayer->setVisible(false);
            _menuPause->setVisible(true);
            _resumeEvents();
        }
    }
}

void GameLayer::_resumeEvents()
{
    resumeSchedulerAndActions();
    _resumeAllActions();
    _pause = false;
}

#pragma mark - Remove Nodes

void GameLayer::_removeNode(CCNode *node)
{
    node->removeFromParent();
}

#pragma mark - Game Logic

void GameLayer::_gameLogic(float dt)
{
    if(_isJoypad)
    {
        // Update Control
        _hudLayer->updateControl(*_player, dt);
    }
    else
    {
        // Accelerometer mode
        _player->doMove(_accelVelocity);
    }
    
    // Increment map speed
    _worldSpeed += dt * 2;
    
    _score += dt;
    _lblScore->setString(CCString::createWithFormat("%d", (int) (_score * kScoreFactor))->getCString());
    
    int z = (WIN_SIZE.height - (_player->getPlayerY() + _player->getContentSize().height * 0.75f)) + kDeepGameElements;
    
    this->reorderChild(_player, z);
    
    CCObject* object;
    CCSprite* sprite;
    BaseObstacle* obstacle;
    float spriteWidth;
    
    // Move cloud
    if(_spCloud->getPositionX() <= -_spCloud->getContentSize().width * 0.5f)
        _spCloud->setPositionX(WIN_SIZE.width + _spCloud->getContentSize().width * 0.7f);
    _spCloud->setPositionX(_spCloud->getPositionX() - _worldSpeed * dt * DT_SPEED_NUBE);
    
    ccColor3B color = ccc3((int)_color, (int)_color, (int)_color);
    
    ((CCSprite*) _parallaxSky->objectAtIndex(0))->setColor(color);
    ((CCSprite*) _parallaxSky->objectAtIndex(1))->setColor(color);
    _spCloud->setColor(color);
    
    // Bucle for _parallaxBGBack
    CCARRAY_FOREACH(_parallaxBGBack, object)
    {
        sprite = (CCSprite*) object;
        spriteWidth = sprite->getContentSize().width;
        
        if(sprite->getPositionX() <= -spriteWidth)
        {
            float diff = spriteWidth + sprite->getPositionX();
            sprite->setPositionX((_parallaxBGBack->count() - 1) * spriteWidth + diff);
        }
        sprite->setPositionX(sprite->getPositionX() - _worldSpeed * dt * DT_SPEED_BG_BACK);
        sprite->setColor(color);
    }

    // Bucle for _parallaxBGMid
    CCARRAY_FOREACH(_parallaxBGMid, object)
    {
        sprite = (CCSprite*) object;
        spriteWidth = sprite->getContentSize().width;
        
        if(sprite->getPositionX() <= -spriteWidth)
        {
            float diff = spriteWidth + sprite->getPositionX();
            sprite->setPositionX((_parallaxBGMid->count() - 1) * spriteWidth + diff);
        }
        sprite->setPositionX(sprite->getPositionX() - _worldSpeed * dt * DT_SPEED_BG_MID);
        sprite->setColor(color);
    }
    
    // Bucle for _parallaxBGFront
    CCARRAY_FOREACH(_parallaxBGFront, object)
    {
        sprite = (CCSprite*) object;
        spriteWidth = sprite->getContentSize().width;
        
        if(sprite->getPositionX() <= -spriteWidth)
        {
            float diff = spriteWidth + sprite->getPositionX();
            sprite->setPositionX((_parallaxBGFront->count() - 1) * spriteWidth + diff);
        }
        sprite->setPositionX(sprite->getPositionX() - _worldSpeed * dt * DT_SPEED_BG_FRONT);
        sprite->setColor(color);
    }
    
    _color += dt * _colorSign * 3;
    
    if(_color < MIN_COLOR && _colorSign == -1)
        _colorSign = 1;
    else if(_color > MAX_COLOR && _colorSign == 1)
        _colorSign = -1;
    
    if(_color > MAX_COLOR)
        _color = MAX_COLOR;
    
    if(_color < MIN_COLOR)
        _color = MIN_COLOR;
    
    // Bucle for _parallaxFloor
    CCARRAY_FOREACH(_parallaxFloor, object)
    {
        sprite = (CCSprite*) object;
        spriteWidth = sprite->getContentSize().width;
        
        if(sprite->getPositionX() <= -spriteWidth)
        {
            float diff = spriteWidth + sprite->getPositionX();
            sprite->setPositionX((_parallaxFloor->count() - 1) * spriteWidth + diff);
        }
        sprite->setPositionX(sprite->getPositionX() - _worldSpeed * dt * DT_SPEED_PISTA);
        
    }
    
    CCArray* _removeObstacles = CCArray::create();
    
    // Move obstacles, detect collisions, play sfx and remove
    CCARRAY_FOREACH(_arrayObstacles, object)
    {
        obstacle = (BaseObstacle*) object;
        float positionX = obstacle->getPositionX();
        
        // Show object
        if(obstacle->getNumObjects() > 0 && !obstacle->getIsObjectAlerted() && positionX - obstacle->getContentSize().width < WIN_SIZE.width * 1.3f)
        {
            obstacle->setIsObjectAlerted(true);
            
            int i = 0;
            float x = WIN_SIZE.width - obstacle->getContentSize().width * 1.5f;
            float blinkTime = 0.3f * (START_WORLD_SPEED / _worldSpeed);
            
            if(blinkTime > 0)
            {
                int _totalObjects = obstacle->getNumObjects();
                _totalObjects = 1;
                for(i = 0; i < _totalObjects; i++)
                {
                    CCSprite* alertSprite = CCSprite::createWithTexture(obstacle->getTexture());
                    
                    alertSprite->setPositionX(x);
                    alertSprite->setPositionY(obstacle->getPositionY());
                    
                    alertSprite->runAction(CCSequence::create(CCBlink::create(blinkTime, 2), CCCallFuncN::create(this, callfuncN_selector(GameLayer::_removeNode)), NULL));
                    
                    // alertSprite->setOpacity(128);
                    
                    addChild(alertSprite, kDeepScore);
                    
                    x -= obstacle->getDistanceObjects();
                    
                }
            }

        }
        
        obstacle->doUpdate(positionX, _worldSpeed * dt * DT_SPEED_OBSTACULOS);
        
        if(obstacle->getPositionX() < WIN_SIZE.width && obstacle->getPositionX() > 0 && obstacle->collision(*_player))
        {
            _lblScore->setVisible(false);
            _menuPause->setVisible(false);
            _player->dead();
            this->reorderChild(_player, kDeepGameFinish);
            _gameOver = true;
            _gameState = kGameFinish;
            break;
        }
        else
        {
            if(!obstacle->getPassPlayerSFX() && obstacle->getPositionX() + obstacle->getContentSize().width * 1.0f < _player->getPositionX())
            {
                obstacle->setPassPlayerSFX(true);
                if(obstacle->getObstacType() == kJumpObstacle)
                {
                    _obstaclesJumped++;
                }
                _obstaclesAvoided++;
                SimpleAudioEngine::sharedEngine()->playEffect(SFX_SWOOSH);
            }
            
            if(obstacle->getPositionX() < -obstacle->getContentSize().width * 0.5f)
            {
                _removeObstacles->addObject(obstacle);
            }
        }
        
    }
    
    // Remove and Add objects
    CCARRAY_FOREACH(_removeObstacles, object)
    {
        BaseObstacle* lastObstacle = (BaseObstacle*) _arrayObstacles->lastObject();
        
        obstacle = (BaseObstacle*) object;
        int currentTag = obstacle->getTag();
        _arrayObstacles->removeObject(obstacle);
        obstacle->removeFromParent();
        
        if(currentTag == 1)
        {
            float x = lastObstacle->getPositionX() + MIN_DISTANCE_OBSTACLES;
            _createObstacle(x);
        }

    }
    
    // _runLightning(dt);

}

void GameLayer::_runLightning(float dt)
{
    if(_color < MAX_COLOR - MIN_COLOR)
    {
        _lightningTimer += dt;
        if(_lightningTimer > LIGHT_TIME)
        {
            CCSprite* lightning = CCSprite::create("");
            
            CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
            CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
            
            float x = Utils::randomBetween(lightning->getContentSize().width, origin.x + visibleSize.width - lightning->getContentSize().width);
            
            float y = origin.y + visibleSize.height - lightning->getContentSize().height * 0.5f;
            
            lightning->setPosition(ccp(x, y));
            
            CCAnimate* action = (CCAnimate*) _lightningAnimation->copy()->autorelease();
            lightning->runAction(CCSequence::create(action, CCCallFuncN::create(this, callfuncN_selector(GameLayer::_removeNode)), NULL));
            addChild(lightning, kDeepTracks - 50);
            SimpleAudioEngine::sharedEngine()->playEffect(SFX_LIGHTNING);
            _lightningTimer = 0;
        }
    }
    else
    {
        _lightningTimer = 0;
    }
}

void GameLayer::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    if(!_pause && !_gameOver)
    {
        if(_gameState == kGameReady)
        {
            CCTouch* touch = (CCTouch *) pTouches->anyObject();
            CCPoint location = touch->getLocationInView();
            
            if(location.x >= WIN_SIZE.width * 0.5f)
                _player->doJump();
        }
    }
}

#pragma mark - Update method

void GameLayer::update(float dt)
{
    if(_pause)
        return;
    
    if(_gameState == kGameStarting)
    {
        
        _gameState = kGamePreparing;
        _player->runAction(CCMoveTo::create(1.5f, ccp(_player->getContentSize().width * 2.5f, _player->getPositionY())));
    }
    
    else if(_gameState == kGamePreparing)
    {
        
        if(_player->numberOfRunningActions() <= 1)
        {
            
            _lblScore->setVisible(true);
            if(_isJoypad)
                _hudLayer->setVisible(true);
            
            if(LocalStorageManager::showTutorial())
                _showTutorial();
            else
                _gameIsReady();
        }
        
    }
    else if(_gameState == kGameReady)
    {
        if(!_gameOver)
        {
            _gameLogic(dt);
        }
        
    }
    else if(_gameOver && _gameState == kGameFinish)
    {
        if(_player->numberOfRunningActions() == 0)
        {
            
            SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
            
            _checkAchievements();
            _obstaclesAvoided = 0;
            
            setTouchEnabled(false);
            if(!_isJoypad)
                setAccelerometerEnabled(false);
            _hudLayer->setVisible(false);
            _menuPause->setVisible(false);
            _popUpLoseLayer->updateScore(_gameLevel, _score * kScoreFactor);
            _popUpLoseLayer->runAction(CCMoveBy::create(0.25f, ccp(0, WIN_SIZE.height)));
            NativeUtils::showAd();
            unscheduleUpdate();
        }
    }

}

void GameLayer::_gameIsReady()
{
    _gameState = kGameReady;
    setTouchEnabled(true);
    _menuPause->setVisible(true);
    
    if(!LocalStorageManager::isMute())
        _showAudioPlaying();
}

void GameLayer::_showTutorial()
{
    
    ccLanguageType language = CCApplication::sharedApplication()->getCurrentLanguage();
    const char* tap_to_jump = "Tap to Jump";
    const char* joypad_move = "Joypad to move";
    const char* tilt_move = "Tilt to move";
    const char* tap_continue = "Tap here to continue";
    const char* avoid_obstacles = "Avoid the obstacles!";
    
    if(language == kLanguageSpanish)
    {
        tap_to_jump = "Tap para saltar";
        joypad_move = "Para moverte";
        tilt_move = "Inclina para mover";
        tap_continue = "Tap aquí para continuar";
        avoid_obstacles = "¡Evade los obstáculos!";
    }
    
    _gameState = kGameTutorial;
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint visibleOrigin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    CCPoint center = CCPointZero;
    center.x = visibleOrigin.x + visibleSize.width * 0.5f;
    center.y = visibleOrigin.y + visibleSize.height * 0.5f;
    
    CCLayerColor* layer = CCLayerColor::create(ccc4BFromccc4F(ccc4f(0, 0, 0, 0.5f)));
    layer->setTag(kTagTutorialLayer);
    
    CCSprite* spriteJump = CCSprite::create("tap.png");
    spriteJump->setPositionX(visibleOrigin.x + visibleSize.width - spriteJump->getContentSize().width * 0.5f);
    spriteJump->setPositionY(visibleOrigin.y + spriteJump->getContentSize().height * 0.4f);
    layer->addChild(spriteJump);
    spriteJump->runAction(CCRepeatForever::create(CCSequence::create(CCMoveBy::create(0.5f, ccp(0, -9)), CCMoveBy::create(0.5f, ccp(0, 9)), NULL)));

    // Jump Tutorial
    CCLabelTTF* lblJump = CCLabelTTF::create(tap_to_jump, FONT_GAME, SIZE_TUT_INST);
    lblJump->setPosition(center);
    lblJump->setPositionY(spriteJump->getPositionY() + spriteJump->getContentSize().height * 0.6f);
    lblJump->setPositionX(lblJump->getPositionX() + visibleSize.width * 0.3f);
    layer->addChild(lblJump);
    
    if(_isJoypad)
    {
        // Press Joypad Tutorial
        CCLabelTTF* lblJoypad = CCLabelTTF::create(joypad_move, FONT_GAME, SIZE_TUT_INST);
        lblJoypad->setPosition(center);
        lblJoypad->setPositionY(lblJump->getPositionY() - spriteJump->getContentSize().height * 0.1f);
        lblJoypad->setPositionX(lblJoypad->getPositionX() - visibleSize.width * 0.32f);
        layer->addChild(lblJoypad);
        
        _hudLayer->runTutorialJoypad();
    }
    else
    {
        // Accelerometer
        CCLabelTTF* lblAccelerometer = CCLabelTTF::create(tilt_move, FONT_GAME, SIZE_TUT_INST);
        lblAccelerometer->setPosition(center);
        lblAccelerometer->setPositionY(lblJump->getPositionY());
        lblAccelerometer->setPositionX(lblAccelerometer->getPositionX() - visibleSize.width * 0.35f);
        layer->addChild(lblAccelerometer);
    }
    
    CCLabelTTF* lblCloseTutorial = CCLabelTTF::create(tap_continue, FONT_GAME, SIZE_TUT_TITLE);
    CCMenuItemLabel* menuCloseTutorial = CCMenuItemLabel::create(lblCloseTutorial, this, menu_selector(GameLayer::_finishTutorial));
    menuCloseTutorial->setPosition(center);
    menuCloseTutorial->setPositionY(visibleOrigin.y + visibleSize.height - menuCloseTutorial->getContentSize().height * 1.9f);
    
    // Avoid the obstacles
    CCLabelTTF* lblAvoid = CCLabelTTF::create(avoid_obstacles, FONT_GAME, SIZE_TUT_INST, CCSizeMake(visibleSize.width * 0.8f, visibleSize.height), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    lblAvoid->setPosition(center);
    lblAvoid->setPositionY(visibleOrigin.y +  menuCloseTutorial->getContentSize().height * 1.2f);
    layer->addChild(lblAvoid);
    
    CCMenu* menu = CCMenu::create(menuCloseTutorial, NULL);
    menu->setPosition(CCPointZero);
    layer->addChild(menu);
    
    addChild(layer, kDeepTutorial);
    
    unscheduleUpdate();
    _pauseAllActions();
    
    menuCloseTutorial->runAction(CCRepeatForever::create(CCSequence::create(CCRotateTo::create(0.5f, -5), CCRotateTo::create(0.5f, 5), NULL)));
}

void GameLayer::_finishTutorial(cocos2d::CCObject *object)
{
    
    CCMenuItem* item = (CCMenuItem *) object;
    item->setEnabled(false);
    
    if(_isJoypad)
        _hudLayer->stopTutorialJoypad();
    
    CCLayerColor* layer = (CCLayerColor *) getChildByTag(kTagTutorialLayer);
    layer->removeFromParent();
    
    scheduleUpdate();
    _resumeAllActions();
    _gameIsReady();
    // pauseGame();
}

void GameLayer::_checkAchievements()
{
    
    long longScore = (long) (_score * kScoreFactor);
    
    // Obstacles avoidment
    
    if(_gameLevel == kGameLevelEasy && _obstaclesAvoided >= 100)
    {
        
        if(!LocalStorageManager::isAchievementUnlocked(ACH_AVOID_100_OBSTACLES_IN_EASY_MODE))
        {
            Utils::unlockAchievement(ACH_AVOID_100_OBSTACLES_IN_EASY_MODE);
            LocalStorageManager::unlockAchievement(ACH_AVOID_100_OBSTACLES_IN_EASY_MODE);
        }
        
    }
    else if(_gameLevel == kGameLevelNormal && _obstaclesAvoided >= 50)
    {
        
        if(!LocalStorageManager::isAchievementUnlocked(ACH_AVOID_50_OBSTACLES_IN_NORMAL_MODE))
        {
            Utils::unlockAchievement(ACH_AVOID_50_OBSTACLES_IN_NORMAL_MODE);
            LocalStorageManager::unlockAchievement(ACH_AVOID_50_OBSTACLES_IN_NORMAL_MODE);
        }
        
    }
    else if(_gameLevel == kGameLevelHard && _obstaclesAvoided >= 25)
    {
        
        if(!LocalStorageManager::isAchievementUnlocked(ACH_AVOID_25_OBSTACLES_IN_HARD_MODE))
        {
            Utils::unlockAchievement(ACH_AVOID_25_OBSTACLES_IN_HARD_MODE);
            LocalStorageManager::unlockAchievement(ACH_AVOID_25_OBSTACLES_IN_HARD_MODE);
        }
        
    }
    
    if(_gameLevel == kGameLevelHard && _obstaclesAvoided >= 100)
    {
        
        if(!LocalStorageManager::isAchievementUnlocked(ACH_AVOID_100_OBSTACLES_IN_HARD_MODE))
        {
            Utils::unlockAchievement(ACH_AVOID_100_OBSTACLES_IN_HARD_MODE);
            LocalStorageManager::unlockAchievement(ACH_AVOID_100_OBSTACLES_IN_HARD_MODE);
        }
        
    }
    
    //
    
    if(!LocalStorageManager::isAchievementUnlocked(ACH_MORE_THAN_3000) && longScore > 3000)
    {
        Utils::unlockAchievement(ACH_MORE_THAN_3000);
        LocalStorageManager::unlockAchievement(ACH_MORE_THAN_3000);
    }
    
    if(!LocalStorageManager::isAchievementUnlocked(ACH_GET_10000_OR_MORE_IN_EASY_MODE) && _gameLevel == kGameLevelEasy && longScore >= 10000)
    {
        Utils::unlockAchievement(ACH_GET_10000_OR_MORE_IN_EASY_MODE);
        LocalStorageManager::unlockAchievement(ACH_GET_10000_OR_MORE_IN_EASY_MODE);
        
    }

    if(!LocalStorageManager::isAchievementUnlocked(ACH_GET_8000_OR_MORE_IN_NORMAL_MODE) && _gameLevel == kGameLevelNormal && longScore >= 8000)
    {
        Utils::unlockAchievement(ACH_GET_8000_OR_MORE_IN_NORMAL_MODE);
        LocalStorageManager::unlockAchievement(ACH_GET_8000_OR_MORE_IN_NORMAL_MODE);
        
    }

    if(!LocalStorageManager::isAchievementUnlocked(ACH_GET_5000_OR_MORE_IN_HARD_MODE) && _gameLevel == kGameLevelHard && longScore >= 5000)
    {
        Utils::unlockAchievement(ACH_GET_5000_OR_MORE_IN_HARD_MODE);
        LocalStorageManager::unlockAchievement(ACH_GET_5000_OR_MORE_IN_HARD_MODE);
    }
    
    if(!LocalStorageManager::isAchievementUnlocked(ACH_PLAY_IN_ACCELEROMETER_MODE_AND_GET_MORE_THAN_3000) && !_isJoypad && longScore >= 3000)
    {
        Utils::unlockAchievement(ACH_PLAY_IN_ACCELEROMETER_MODE_AND_GET_MORE_THAN_3000);
        LocalStorageManager::unlockAchievement(ACH_PLAY_IN_ACCELEROMETER_MODE_AND_GET_MORE_THAN_3000);
    }
    
    if(!LocalStorageManager::isAchievementUnlocked(ACH_GET_30K_IN_EASY_MODE) && _gameLevel == kGameLevelEasy && longScore >= 30000)
    {
        Utils::unlockAchievement(ACH_GET_30K_IN_EASY_MODE);
        LocalStorageManager::unlockAchievement(ACH_GET_30K_IN_EASY_MODE);
    }
    
    if(!LocalStorageManager::isAchievementUnlocked(ACH_GET_15K_IN_NORMAL_MODE) && _gameLevel == kGameLevelNormal && longScore >= 15000)
    {
        Utils::unlockAchievement(ACH_GET_15K_IN_NORMAL_MODE);
        LocalStorageManager::unlockAchievement(ACH_GET_15K_IN_NORMAL_MODE);
    }
    
    if(!LocalStorageManager::isAchievementUnlocked(ACH_GET_10K_IN_HARD_MODE) && _gameLevel == kGameLevelEasy && longScore >= 10000)
    {
        Utils::unlockAchievement(ACH_GET_10K_IN_HARD_MODE);
        LocalStorageManager::unlockAchievement(ACH_GET_10K_IN_HARD_MODE);
    }
    
    if(_obstaclesJumped > 0)
    {
        
        int totalObstaclesJumped = LocalStorageManager::getObstaclesJumped() + _obstaclesJumped;
        
        // Jump 50 Obstacles
        if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        {
            if(totalObstaclesJumped >= 50/* && !LocalStorageManager::isAchievementUnlocked(ACH_JUMP_50_OBSTACLES)*/)
                Utils::unlockAchievement(ACH_JUMP_50_OBSTACLES);
        }
        else
        {
            Utils::incrementAchievement(ACH_JUMP_50_OBSTACLES, _obstaclesJumped);
        }
        
        // Jump 1000 Obstacles
        if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        {
            if(totalObstaclesJumped >= 1000/* && !LocalStorageManager::isAchievementUnlocked(ACH_JUMP_1000_OBSTACLES)*/)
                Utils::unlockAchievement(ACH_JUMP_1000_OBSTACLES);
        }
        else
        {
            Utils::incrementAchievement(ACH_JUMP_1000_OBSTACLES, _obstaclesJumped);
        }
        
        LocalStorageManager::updateObstaclesJumped(totalObstaclesJumped);
        
    }
    
    _obstaclesJumped = 0;
    
}

#pragma mark - Draw Method

void GameLayer::draw()
{
    CCLayer::draw();
    
    if(DRAW_COLLISIONS && _gameState == kGameReady)
    {
        
        
        CCObject* object;
        
        CCARRAY_FOREACH(_arrayObstacles, object)
        {
            BaseObstacle* obstacle = (BaseObstacle*) object;
            if(obstacle != NULL)
            {
                int i;
                CCRect area;
                float left, top, right, bottom;
                
                std::vector<CCRect> areas = obstacle->getVCollision();
                
                if(areas.size() > 0)
                {
                    for(i = 0; i < areas.size(); i++)
                    {
                        area = obstacle->currentCollisionArea(areas[i]);
                        
                        left = area.getMinX();
                        top = area.getMinY();
                        right = area.getMaxX();
                        bottom = area.getMaxY();
                        
                        CCPoint origin = ccp(left, top);
                        CCPoint destination = ccp(right, bottom);
                        ccDrawSolidRect(origin, destination, ccc4f(0.0f, 1.0f, 0.0f, 0.25f));
                        
                        
                    }
                }
                
                if(obstacle->getObstacType() == kSimpleObstacle)
                {
                    area = obstacle->boundingBox();
                    top = area.getMinY() + obstacle->getContentSize().height * 0.0f;
                    bottom = top + obstacle->getContentSize().height * 0.37f;
                    left = area.getMinX();
                    right = area.getMaxX();
                    
                    CCPoint origin = ccp(left, top);
                    CCPoint destination = ccp(right, bottom);
                    ccDrawSolidRect(origin, destination, ccc4f(0.0f, 0.0f, 1.0f, 0.25f));
                    
                }
                
            }
        }
        
        if(_player == NULL)
            return;
        
        // Verde
        CCPoint originGreenPlayer = ccp(_player->getGroundCollision().getMinX(), _player->getGroundCollision().getMinY());
        CCPoint destionationGreenPlayer = ccp(_player->getGroundCollision().getMaxX(), _player->getGroundCollision().getMaxY());
        ccDrawSolidRect(originGreenPlayer, destionationGreenPlayer, ccc4f(0.0f, 1.0f, 0.0f, 0.25f));
        
        // Rojo
        CCPoint originRedPlayer = ccp(_player->getAirCollision().getMinX(), _player->getAirCollision().getMinY());
        CCPoint destionationRedPlayer = ccp(_player->getAirCollision().getMaxX(), _player->getAirCollision().getMaxY());
        ccDrawSolidRect(originRedPlayer, destionationRedPlayer, ccc4f(1.0f, 0.0f, 0.0f, 0.25f));
        
        
    }
    
    if(TEST_OBSTACLE)
    {
        
        CCArray* _testObstacles = CCArray::create();
        _testObstacles->addObject((CCSprite *) getChildByTag(1000));
        _testObstacles->addObject((CCSprite *) getChildByTag(1001));
        _testObstacles->addObject((CCSprite *) getChildByTag(1002));
        _testObstacles->addObject((CCSprite *) getChildByTag(1003));
        
        CCObject* object;
        
        CCARRAY_FOREACH(_testObstacles, object)
        {
            BaseObstacle* obstacle = (BaseObstacle*) object;
            if(obstacle != NULL)
            {
                int i;
                CCRect area;
                float left, top, right, bottom;
                
                std::vector<CCRect> areas = obstacle->getVCollision();
                
                if(areas.size() > 0)
                {
                    for(i = 0; i < areas.size(); i++)
                    {
                        area = obstacle->currentCollisionArea(areas[i]);
                        
                        left = area.getMinX();
                        top = area.getMinY();
                        right = area.getMaxX();
                        bottom = area.getMaxY();
                        
                        CCPoint origin = ccp(left, top);
                        CCPoint destination = ccp(right, bottom);
                        ccDrawSolidRect(origin, destination, ccc4f(0.0f, 1.0f, 0.0f, 0.25f));
                        
                        
                    }
                }
                
                if(obstacle->getObstacType() == kSimpleObstacle)
                {
                    area = obstacle->boundingBox();
                    top = area.getMinY() + obstacle->getContentSize().height * 0.0f;
                    bottom = top + obstacle->getContentSize().height * 0.37f;
                    left = area.getMinX();
                    right = area.getMaxX();
                    
                    CCPoint origin = ccp(left, top);
                    CCPoint destination = ccp(right, bottom);
                    ccDrawSolidRect(origin, destination, ccc4f(0.0f, 0.0f, 1.0f, 0.25f));
                    
                }
                
            }
        }
    }
    
}

#pragma mark - Notification

void GameLayer::_playAgain()
{
    NativeUtils::hideAd();
    SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    CCScene* scene = HomeScene::scene(kGameModePlayAgain, _gameLevel);
    CCDirector::sharedDirector()->replaceScene(CCTransitionFadeDown::create(0.5f, scene));
}

void GameLayer::_goHome()
{
    SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    CCScene* scene = HomeScene::scene(kGameModeHome);
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, scene));
}

#pragma mark - Pause All Actions and Resume

void GameLayer::_pauseAllActions()
{
    CCObject* object;
    CCARRAY_FOREACH(getChildren(), object)
    {
        CCSprite* sprite = (CCSprite*) object;
        if(sprite != NULL)
            sprite->pauseSchedulerAndActions();
    }
}

void GameLayer::_resumeAllActions()
{
    CCObject* object;
    CCARRAY_FOREACH(getChildren(), object)
    {
        CCSprite* sprite = (CCSprite*) object;
        if(sprite != NULL)
            sprite->resumeSchedulerAndActions();
    }
}
