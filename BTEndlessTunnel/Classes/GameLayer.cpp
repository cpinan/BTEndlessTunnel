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

#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>

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
    0,0,1,0,0,1,1,0,1,1,0,0,1,4,5,0,1,3,8
};

int normalMap[] = {
    3,2,1,1,0,0,1,0,1,0,1,0,1,2,3,2,2,3,3,
    0,1,0,1,5,4,5,4,5,4,2,5,4,2,5,4,2,5,4,
    3,2,0,1,0,8,9,2,0,1,4,5,4,5,8,0,1,0,2,
    3,3,2,2,3,8,0,1,0,1,4,5,4,5,2,4,5,3,2,
    0,1,1,0,0,1,1,0,0,1,2,3,2,2,3,3,8,9,6,
    3,7,0,0,1,1,4,5,5,4,2,3,2,1,0,1,0,1,0
};

int hardMap[] = {
    6,7,8,7,6,9,5,2,4,2,5,3,4,3,8,9,0,1,0,
    1,2,9,8,3,4,5,4,5,4,5,4,5,4,5,0,1,0,1,
    8,3,6,3,8,3,2,3,8,9,6,7,0,2,1,2,0,3,9,
    6,9,6,9,8,9,3,8,9,2,9,9,2,9,9,2,4,5,8,
    9,3,8,9,8,3,3,4,5,4,5,0,1,0,1,2,3,3,8,
    6,9,6,9,6,0,1,6,9,3,6,0,9,8,9,6,8,0,9
};

vector<int> _vectorMap;

// End Level definition

GameLayer::GameLayer(HudLayer* hudLayer, GameMode gameMode, GameLevel gameLevel) : _hudLayer(hudLayer), _gameMode(gameMode)
{
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GameLayer::pauseGame), NOTIFICATION_PAUSE_GAME, NULL);
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GameLayer::resumeGame), NOTIFICATION_RESUME_GAME, NULL);
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(GameLayer::_playAgain), NOTIFICATION_PLAY_AGAIN, NULL);
    
    _testObstacle = new ObstacleDobleAir();
    _testObstacle->setVisible(false);
    _testObstacle->autorelease();
    _testObstacle->setPosition(ccp(200, 145));
    addChild(_testObstacle, -10);
    
    _createMap();
    _initLayers();
    _gameLevel = gameLevel;
}

void GameLayer::onEnterTransitionDidFinish()
{
    if(_gameMode == kGameModePlay || _gameMode == kGameModePlayAgain || _gameMode == kGameModeReplayView)
    {
        configureGame(_gameLevel);
        runGame();
    }
}

GameLayer::~GameLayer()
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, NOTIFICATION_PAUSE_GAME);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, NOTIFICATION_RESUME_GAME);
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, NOTIFICATION_PLAY_AGAIN);

    CC_SAFE_RELEASE(_parallaxBackground);
    CC_SAFE_RELEASE(_parallaxRoof);
    CC_SAFE_RELEASE(_parallaxFloor);
    CC_SAFE_RELEASE(_arrayObstacles);
}

#pragma mark - Init layers, Create Players and Game Elements

void GameLayer::_createMap()
{
    int i = 0;
    float x, y;
    
    // Parallax Background
    _parallaxBackground = CCArray::createWithCapacity(MAX_PARALLAX);
    _parallaxBackground->retain();
    CCSprite* bgParallax = CCSprite::create("bg_parallax.png");
    x = 0;
    y = designResolutionSize.height - bgParallax->getContentSize().height;
    for(i = 0; i < MAX_PARALLAX; i++)
    {
        bgParallax = CCSprite::create("bg_parallax.png");
        bgParallax->setAnchorPoint(ccp(0, 0));
        bgParallax->setPosition(ccp(x, y));
        _parallaxBackground->addObject(bgParallax);
        addChild(bgParallax, -100);
        x += bgParallax->getContentSize().width;
    }
    
    // Parallax Roof
    _parallaxRoof = CCArray::createWithCapacity(MAX_PARALLAX);
    _parallaxRoof->retain();
    CCSprite* spRoof = CCSprite::create("bg_techo_parallax.png");
    x = 0;
    y = designResolutionSize.height - spRoof->getContentSize().height;
    for(i = 0; i < MAX_PARALLAX; i++)
    {
        spRoof = CCSprite::create("bg_techo_parallax.png");
        spRoof->setAnchorPoint(ccp(0, 0));
        spRoof->setPosition(ccp(x, y));
        _parallaxRoof->addObject(spRoof);
        addChild(spRoof, -90);
        x += spRoof->getContentSize().width;
    }
    
    // Parallax Floor
    _parallaxFloor = CCArray::createWithCapacity(MAX_PARALLAX);
    _parallaxFloor->retain();
    CCSprite* spFloor = CCSprite::create("floor_parallax.png");
    x = 0;
    y = 0;
    for(i = 0; i < MAX_PARALLAX; i++)
    {
        spFloor = CCSprite::create("floor_parallax.png");
        spFloor->setAnchorPoint(ccp(0, 0));
        spFloor->setPosition(ccp(x, y));
        _parallaxFloor->addObject(spFloor);
        addChild(spFloor, -80);
        x += spFloor->getContentSize().width;
    }
}

void GameLayer::configureGame(GameLevel gameLevel)
{
    
    _gameLevel = gameLevel;
    
    // setAccelerometerEnabled(true);
    setTouchEnabled(true);
    
     _worldSpeed = START_WORLD_SPEED;
    _minDistanceObstaclesX = MIN_DISTANCE_OBSTACLES;
    
    if(_gameLevel == kGameLevelEasy)
    {
        _vectorMap.insert(_vectorMap.begin(), easyMap, easyMap + sizeof(easyMap) / sizeof(int));
    }
    else if(_gameLevel == kGameLevelNormal)
    {
        _minDistanceObstaclesX *= 0.9f;
        _worldSpeed *= 1.3f;
        _vectorMap.insert(_vectorMap.begin(), normalMap, normalMap + sizeof(normalMap) / sizeof(int));
    }
    else if(_gameLevel == kGameLevelHard)
    {
        _minDistanceObstaclesX *= 0.8f;
        _worldSpeed *= 1.8f;
        _vectorMap.insert(_vectorMap.begin(), hardMap, hardMap + sizeof(hardMap) / sizeof(int));
    }
    
    _itemMap = 0;
    
    _score = 0.0f;
    
    _gameState = kGameStarting;
    
    _arrayObstacles = CCArray::create();
    _arrayObstacles->retain();
    
    _pause = false;
    _gameOver = false;
    
    _createPlayer();
    _initElements();
    
}

void GameLayer::_initLayers()
{
    
    _lblScore = CCLabelTTF::create("0", "Arial", 25.0f);
    _lblScore->setVisible(false);
    _lblScore->setContentSize(CCSizeMake(200, 50));
    _lblScore->setHorizontalAlignment(kCCTextAlignmentLeft);
    _lblScore->setPosition(ccp(designResolutionSize.width - 70, designResolutionSize.height - 20));
    addChild(_lblScore, kDeepScore);
    
    _pauseLayer = new PauseLayer();
    _pauseLayer->autorelease();
    
    _popUpLoseLayer = new PopUpLoseLayer();
    _popUpLoseLayer->setPosition(ccp(0, -designResolutionSize.height));
    _popUpLoseLayer->autorelease();
    addChild(_popUpLoseLayer, kDeepPopUpLoseLayer);
    
}

void GameLayer::_createPlayer()
{
    _player = new VehicleFrog();
    _player->setPositionY(100);
    _player->setPositionX(-_player->getContentSize().width * 2.5f);
    _player->autorelease();
    addChild(_player);
}

void GameLayer::_initElements()
{
    
    int i = 0;
    float x, y;
        
    // Obstacles
    _arrayObstacles = CCArray::create();
    _arrayObstacles->retain();
    
    x = START_X_OBSTACLES;
    y = 145;
    
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
        y = 115;
        z = 100;
    }
    else if(type == 1)
    {
        obstacle = new ObstacleSimple();
        y = 145;
        z = 145;
    }
    else if(type == 2)
    {
        obstacle = new ObstacleDoble();
        y = 115;
        z = designResolutionSize.height * 0.5f;
    }
    else if(type == 3)
    {
        obstacle = new ObstacleDobleAir();
        y = 145;
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
        addChild(obstacle, designResolutionSize.height - z + obstacle->getContentSize().height * 0.5f);
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
        y = 115;
        z = 100;
        
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
            addChild(obstacle, designResolutionSize.height - z + obstacle->getContentSize().height * 0.5f);
            x += _minDistanceObstaclesX * 0.5f;
        }
        
    }
    else if(type == 5)
    {
        // Crear 2 obstaculos simples arriba
        y = 145;
        z = 145;
        
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
            addChild(obstacle, designResolutionSize.height - z + obstacle->getContentSize().height * 0.5f);
            x += _minDistanceObstaclesX * 0.5f;
        }
        
    }
    else if(type == 6)
    {
        // Crear 3 obstaculos dobles en tierra
        y = 115;
        z = designResolutionSize.height * 0.5f;
        
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
            addChild(obstacle, designResolutionSize.height - z + obstacle->getContentSize().height * 0.5f);
            x += _minDistanceObstaclesX * 0.3f;
        }
        
    }
    else if(type == 7)
    {
        // Crear 3 obstaculos dobles en el aire
        y = 145;
        
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
            addChild(obstacle, designResolutionSize.height - z + obstacle->getContentSize().height * 0.5f);
            x += _minDistanceObstaclesX * 0.3f;
        }
    }
    else if(type == 8)
    {
        // Crear 2 obstaculos dobles en tierra
        y = 115;
        z = designResolutionSize.height * 0.5f;
        
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
            addChild(obstacle, designResolutionSize.height - z + obstacle->getContentSize().height * 0.5f);
            x += _minDistanceObstaclesX * 0.3f;
        }
        
    }
    else if(type == 9)
    {
        // Crear 2 obstaculos dobles en el aire
        y = 145;
        
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
            addChild(obstacle, designResolutionSize.height - z + obstacle->getContentSize().height * 0.5f);
            x += _minDistanceObstaclesX * 0.3f;
        }
    }
    
}

#pragma mark - Accelerometer manager

void GameLayer::didAccelerate(CCAcceleration *pAccelerationValue)
{
    //float x = pAccelerationValue->x;
    //float y = pAccelerationValue->y;
    //float z = pAccelerationValue->z;
}

#pragma mark - Game States Manager

void GameLayer::playGame()
{
    _gameMode = kGameModePlay;
    runGame();
}

void GameLayer::runGame()
{
    unscheduleUpdate();
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic(BG_MUSIC_01, true);
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
            _player->pauseSchedulerAndActions();
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
            _gameState = _previousGameState;
        }
    }
}

void GameLayer::_resumeEvents()
{
    _player->resumeSchedulerAndActions();
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
    // Update Control
    _hudLayer->updateControl(*_player, dt);
    
    // Increment map speed
    _worldSpeed += dt * 2;
    
    _score += dt;
    _lblScore->setString(CCString::createWithFormat("%d", (int) (_score * 100))->getCString());
    
    
    this->reorderChild(_player, designResolutionSize.height - (_player->getPlayerY() - _player->getContentSize().height * 0.5f));
    
    CCObject* object;
    CCSprite* sprite;
    BaseObstacle* obstacle;
    float spriteWidth;
    
    // Bucle for _parallaxBackground
    CCARRAY_FOREACH(_parallaxBackground, object)
    {
        sprite = (CCSprite*) object;
        spriteWidth = sprite->getContentSize().width;
        
        if(sprite->getPositionX() <= -spriteWidth)
        {
            float diff = spriteWidth + sprite->getPositionX();
            sprite->setPositionX((_parallaxBackground->count() - 1) * spriteWidth + diff);
        }
        sprite->setPositionX(sprite->getPositionX() - _worldSpeed * dt);
        
    }
    
    // Bucle for _parallaxRoof
    CCARRAY_FOREACH(_parallaxRoof, object)
    {
        sprite = (CCSprite*) object;
        spriteWidth = sprite->getContentSize().width;
        
        if(sprite->getPositionX() <= -spriteWidth)
        {
            float diff = spriteWidth + sprite->getPositionX();
            sprite->setPositionX((_parallaxRoof->count() - 1) * spriteWidth + diff);
        }
        sprite->setPositionX(sprite->getPositionX() - _worldSpeed * dt);
        
    }
    
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
        sprite->setPositionX(sprite->getPositionX() - _worldSpeed * dt);
        
    }
    
    CCArray* _removeObstacles = CCArray::create();
    
    // Move obstacles, detect collisions, play sfx and remove
    CCARRAY_FOREACH(_arrayObstacles, object)
    {
        obstacle = (BaseObstacle*) object;
        float positionX = obstacle->getPositionX();
        
        // Show object
        if(obstacle->getNumObjects() > 0 && !obstacle->getIsObjectAlerted() && positionX - obstacle->getContentSize().width < designResolutionSize.width * 1.3f)
        {
            obstacle->setIsObjectAlerted(true);
            
            int i = 0;
            float x = designResolutionSize.width - obstacle->getContentSize().width * 1.5f;
            float blinkTime = 0.3f * (START_WORLD_SPEED / _worldSpeed);
            
            if(blinkTime > 0)
            {
                for(i = 0; i < obstacle->getNumObjects(); i++)
                {
                    CCSprite* alertSprite = CCSprite::createWithTexture(obstacle->getTexture());
                    
                    alertSprite->setPositionX(x);
                    alertSprite->setPositionY(obstacle->getPositionY());
                    
                    alertSprite->runAction(CCSequence::create(CCBlink::create(blinkTime, 2), CCCallFuncN::create(this, callfuncN_selector(GameLayer::_removeNode)), NULL));
                    
                    alertSprite->setOpacity(128);
                    
                    addChild(alertSprite);
                    
                    x -= obstacle->getDistanceObjects();
                    
                }
            }

        }
        
        obstacle->setPositionX(positionX - _worldSpeed * dt);
        
        if(obstacle->collision(*_player))
        {
            _player->dead();
            this->reorderChild(_player, 9999);
            _gameOver = true;
            _gameState = kGameFinish;
            break;
        }
        else
        {
            if(!obstacle->getPassPlayerSFX() && obstacle->getPositionX() + obstacle->getContentSize().width * 1.0f < _player->getPositionX())
            {
                obstacle->setPassPlayerSFX(true);
                SimpleAudioEngine::sharedEngine()->playEffect("swoosh.mp3");
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
            _gameState = kGameReady;
            _lblScore->setVisible(true);
            _hudLayer->setVisible(true);
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
            _popUpLoseLayer->runAction(CCMoveBy::create(0.25f, ccp(0, designResolutionSize.height)));
            unscheduleUpdate();
        }
    }

}

#pragma mark - Draw Method

void GameLayer::draw()
{
    CCLayer::draw();
    
    if(DRAW_COLLISIONS)
    {
        if(_testObstacle == NULL)
            return;
        
        int i;
        CCRect area;
        float left, top, right, bottom;
        
        std::vector<CCRect> areas = _testObstacle->getVCollision();
        
        if(areas.size() > 0)
        {
            for(i = 0; i < areas.size(); i++)
            {
                area = _testObstacle->currentCollisionArea(areas[i]);
                
                left = area.getMinX();
                top = area.getMinY();
                right = area.getMaxX();
                bottom = area.getMaxY();
                
                CCPoint origin = ccp(left, top);
                CCPoint destination = ccp(right, bottom);
                ccDrawSolidRect(origin, destination, ccc4f(0.0f, 1.0f, 0.0f, 0.5f));
                
                
            }
        }
    }

    
}

#pragma mark - Notification

void GameLayer::_playAgain()
{
    CCScene* scene = HomeScene::scene(kGameModePlayAgain, _gameLevel);
    CCDirector::sharedDirector()->replaceScene(CCTransitionMoveInT::create(1.0f, scene));
}
