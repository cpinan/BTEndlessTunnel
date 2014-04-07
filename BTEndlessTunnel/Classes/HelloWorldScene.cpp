#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

#include "BaseObstacle.h"

#include "ObstacleSimple.h"
#include "ObstacleDoble.h"
#include "ObstacleDobleAir.h"

#include "VehicleFrog.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

HelloWorld::~HelloWorld()
{
    CC_SAFE_RELEASE(_parallaxBackground);
    CC_SAFE_RELEASE(_parallaxRoof);
    CC_SAFE_RELEASE(_parallaxFloor);
    CC_SAFE_RELEASE(_arrayObstacles);
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    /*
     0 = wall simple bot
     1 = wall simple top
     2 = wall doble ground
     3 = wall doble air
     */
    int map[] = {
        0,0,1,1,0,0,1,1,0,1,0,1,0,1,0,1,2,2,2,
        3,2,3,2,3,3,3,2,2,3,3,1,1,0,1,0,1,0,1,
        0,1,0,1,0,1,0,1,0,1,3,2,3,3,2,2,3,3,3,
        2,1,0,1,0,1,2,0,1,0,0,1,2,1,1,2,3,3,1,
        1,1,2,2,3,2,1,2,1,2,0,2,1,2,0,1,2,1,0,
        1,2,3,2,1,3,2,3,2,3,2,3,2,3,2,2,3,3,2,
        2,3,2,3,2,3,2,3,1,1,2,2,3};
    
    _vectorMap.insert(_vectorMap.begin(), map, map + sizeof(map) / sizeof(map[0]));
    
    _currentItemMap = 0;
    
    setTouchEnabled(true);
    
    _lblScore = CCLabelTTF::create("0", "Arial", 25.0f);
    _lblScore->setContentSize(CCSizeMake(200, 50));
    _lblScore->setHorizontalAlignment(kCCTextAlignmentLeft);
    _lblScore->setPosition(ccp(designResolutionSize.width - 70, designResolutionSize.height - 20));
    addChild(_lblScore, 99999);
    
    gameOver = false;
    
    _parallaxSpeedX = START_WORLD_SPEED;

    int i = 0;
    float x, y;
    
    // HudLayer
    _hudLayer = new HudLayer();
    _hudLayer->autorelease();
    addChild(_hudLayer, 99999);
    
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
    
    // Obstacles
    _arrayObstacles = CCArray::create();
    _arrayObstacles->retain();
    
    x = START_X_OBSTACLES;
    y = 145;
    // 115 Obstacle Simple Bot And ObstacleDoble terrain
    // 145 Obstacle Simple Top and ObstacleDoble air

    for(i = 0; i < MAX_OBSTACLES; i++)
    {
        // _createWall(x);
        _createMapWall(x);
        x += MIN_DISTANCE_OBSTACLES;
    }

    // 115 ObstacleDoble terrain
    // 145 ObstacleDobleAir
    
    // Create player
    
    
    _player = new VehicleFrog();
    _player->setPosition(ccp(250, 115));
    _player->autorelease();
    addChild(_player, -10);
        
    scheduleUpdate();
    
    return true;
}

void HelloWorld::_createMapWall(float x)
{
    BaseObstacle* obstacle;
    float y;
    int z = 0;
    
    int type = _vectorMap[_currentItemMap];
    
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
    else
    {
        obstacle = new ObstacleDobleAir();
        y = 145;
    }
    
    obstacle->setPosition(ccp(x, y));
    
    obstacle->autorelease();
    _arrayObstacles->addObject(obstacle);
    addChild(obstacle, designResolutionSize.height - z + obstacle->getContentSize().height * 0.5f);
    
    _currentItemMap++;
    if(_currentItemMap > _vectorMap.size())
        _currentItemMap = 0;
}

void HelloWorld::_createWall(float x)
{
    BaseObstacle* obstacle;
    float y;
    float random = CCRANDOM_0_1() * 7;
    int z = 0;
    
    if(random <= 2)
    {
        float rnd = CCRANDOM_0_1() * 8;
        obstacle = new ObstacleSimple();
        
        if(rnd <= 3)
        {
            y = 115;//115;
            z = 100;
        }
        else
        {
            y = 145;
            z = 145;
        }
        
        
    }
    else if(random <= 4)
    {
        obstacle = new ObstacleDoble();
        y = 115;
        z = designResolutionSize.height * 0.5f;
    }
    else
    {
        obstacle = new ObstacleDobleAir();
        y = 145;
    }
    
    
    obstacle->setPosition(ccp(x, y));
    
    obstacle->autorelease();
    _arrayObstacles->addObject(obstacle);
    addChild(obstacle, designResolutionSize.height - z + obstacle->getContentSize().height * 0.5f);
    
}

void HelloWorld::update(float dt)
{
    // return;
    if(!gameOver)
    {
        
        // Update Control
        _hudLayer->updateControl(*_player, dt);
        
        // Increment map speed
        _parallaxSpeedX += dt * 2;
        
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
            sprite->setPositionX(sprite->getPositionX() - _parallaxSpeedX * dt);
            
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
            sprite->setPositionX(sprite->getPositionX() - _parallaxSpeedX * dt);
            
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
            sprite->setPositionX(sprite->getPositionX() - _parallaxSpeedX * dt);
            
        }
        
        CCArray* _removeObstacles = CCArray::create();
        
        // Move obstacles, detect collisions, play sfx and remove
        CCARRAY_FOREACH(_arrayObstacles, object)
        {
            obstacle = (BaseObstacle*) object;
            float positionX = obstacle->getPositionX();
            
            // Show object
            if(!obstacle->getIsObjectAlerted() && positionX - obstacle->getContentSize().width < designResolutionSize.width * 1.1f)
            {
                
                obstacle->setIsObjectAlerted(true);
                
                CCSprite* alertSprite = CCSprite::createWithTexture(obstacle->getTexture());
                
                alertSprite->setPositionX(designResolutionSize.width - obstacle->getContentSize().width * 1.5f);
                alertSprite->setPositionY(obstacle->getPositionY());
                
                alertSprite->runAction(CCSequence::create(CCBlink::create(0.3f * (START_WORLD_SPEED / _parallaxSpeedX), 3), CCCallFuncN::create(this, callfuncN_selector(HelloWorld::_removeNode)), NULL));
                
                alertSprite->setOpacity(128);
                
                addChild(alertSprite);
                
            }
            
            obstacle->setPositionX(positionX - _parallaxSpeedX * dt);
            
            if(obstacle->collision(*_player))
            {
                _player->dead();
                this->reorderChild(_player, 9999);
                
                gameOver = true;
                unscheduleUpdate();
                SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
                break;
            }
            else
            {
                if(!obstacle->getPassPlayerSFX() && obstacle->getPositionX() + obstacle->getContentSize().width * 1.0f < _player->getPositionX())
                {
                    obstacle->setPassPlayerSFX(true);
                    SimpleAudioEngine::sharedEngine()->playEffect("swoosh.mp3");
                }
                else if(obstacle->getPositionX() < -obstacle->getContentSize().width * 0.5f)
                {
                    _removeObstacles->addObject(obstacle);
                }
            }
            
        }
        
        // Remove and Add objects
        
        CCARRAY_FOREACH(_removeObstacles, object)
        {
            obstacle = (BaseObstacle*) object;
            _arrayObstacles->removeObject(obstacle);
            obstacle->removeFromParent();

            BaseObstacle* lastObstacle = (BaseObstacle*) _arrayObstacles->lastObject();
            
            float rnd = CCRANDOM_0_1() + 0.5f;
            
            if(rnd > 1.0f)
                rnd = 1.0f;
            
            if(rnd < 0.3f)
                rnd = 0.3f;
            
            rnd = 1.0f;
            
            /*
            if(_score > 40)
                rnd = 0.6f;
            else if(_score > 20)
                rnd = 0.8f;*/
            
            float x = lastObstacle->getPositionX() + MIN_DISTANCE_OBSTACLES * rnd;
            
            _createMapWall(x);
            
            // _createWall(x);
        }
        
    }

    
}

void HelloWorld::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
    if(gameOver)
    {
        CCScene* scene = HelloWorld::scene();
        CCDirector::sharedDirector()->replaceScene(CCTransitionPageTurn::create(1.0f, scene, true));
    }
}

void HelloWorld::draw()
{
    CCLayer::draw();
    
    if(DRAW_COLLISIONS)
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
                        ccDrawSolidRect(origin, destination, ccc4f(0.0f, 1.0f, 0.0f, 0.5f));
                        
                        
                    }
                }

                /*
                if(obstacle->getTypeObstacle() == kObstacleTypeSingle)
                {
                    
                    area = obstacle->boundingBox();
                    
                    left = area.getMinX();
                    top = area.getMinY() + getContentSize().height * 0.0f;
                    right = area.getMaxX();
                    bottom = top + obstacle->getContentSize().height * 0.38f;
                    
                    CCPoint origin = ccp(left, top);
                    CCPoint destination = ccp(right, bottom);
                    ccDrawSolidRect(origin, destination, ccc4f(0.0f, 0.0f, 1.0f, 0.5f));
                 
                }*/
                
            }
        }
        
        // Verde
        CCPoint originPlayer1 = ccp(_player->getGroundCollision().getMinX(), _player->getGroundCollision().getMinY());
        CCPoint destinationPlayer1 = ccp(_player->getGroundCollision().getMaxX(), _player->getGroundCollision().getMaxY());
        ccDrawSolidRect(originPlayer1, destinationPlayer1, ccc4f(1.0f, 0.0, 0, 0.5f));
        
        // Rojo
        CCPoint originPlayer = ccp(_player->getAirCollision().getMinX(), _player->getAirCollision().getMinY());
        CCPoint destinationPlayer = ccp(_player->getAirCollision().getMaxX(), _player->getAirCollision().getMaxY());
        ccDrawSolidRect(originPlayer, destinationPlayer, ccc4f(0.0f, 1.0f, 0, 0.5f));
        
    }
    
   
    
}

void HelloWorld::_removeNode(CCNode* node)
{
    node->removeFromParent();
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
