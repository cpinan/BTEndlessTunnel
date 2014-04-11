#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

#include "BaseObstacle.h"

#include "ObstacleSimple.h"
#include "ObstacleDoble.h"
#include "ObstacleDobleAir.h"

#include "VehicleFrog.h"
#include "Constants.h"

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
    float x = 0, y = 0;
    
    // Cielo
    CCSprite* spCielo;
    for(i = 0; i < 2; i++)
    {
        spCielo = CCSprite::create("cielo.png");
        spCielo->setAnchorPoint(CCPointZero);
        spCielo->setPosition(ccp(x, WIN_SIZE.height - spCielo->getContentSize().height));
        addChild(spCielo, -9999);
        x += spCielo->getContentSize().width;
    }
    
    _spClouds = CCSprite::create("nube.png");
    _spClouds->setPosition(ccp(WIN_SIZE.width * 0.7f, WIN_SIZE.height - _spClouds->getContentSize().height * 2.3f));
    addChild(_spClouds, -1000);
    
    // Parallax Background
    _parallaxBackground = CCArray::createWithCapacity(MAX_PARALLAX);
    _parallaxBackground->retain();
    CCSprite* bgParallax = CCSprite::create("fondo.png");
    x = 0;
    y = WIN_SIZE.height - bgParallax->getContentSize().height * 0.9f;
    for(i = 0; i < MAX_PARALLAX; i++)
    {
        bgParallax = CCSprite::create("fondo.png");
        bgParallax->setAnchorPoint(ccp(0, 0));
        bgParallax->setPosition(ccp(x, y));
        _parallaxBackground->addObject(bgParallax);
        addChild(bgParallax, -100);
        x += bgParallax->getContentSize().width;
    }
    
    
    // Parallax Floor
    _parallaxFloor = CCArray::createWithCapacity(MAX_PARALLAX);
    _parallaxFloor->retain();
    CCSprite* spFloor = CCSprite::create("pista.png");
    x = 0;
    y = 0;
    for(i = 0; i < MAX_PARALLAX; i++)
    {
        spFloor = CCSprite::create("pista.png");
        spFloor->setAnchorPoint(ccp(0, 0));
        spFloor->setPosition(ccp(x, y));
        _parallaxFloor->addObject(spFloor);
        addChild(spFloor, -80);
        x += spFloor->getContentSize().width;
    }
    
    // Obstacles
    _arrayObstacles = CCArray::create();
    _arrayObstacles->retain();
    
    // Create player
    
    /*
    _player = new VehicleFrog();
    _player->setPosition(ccp(250, 115));
    _player->autorelease();
    addChild(_player, -10);
        */
    scheduleUpdate();
    
    return true;
}

void HelloWorld::update(float dt)
{
    // return;
    if(!gameOver)
    {
        
        // Increment map speed
        _parallaxSpeedX += dt * 2;
        
        CCObject* object;
        CCSprite* sprite;
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
        
        if(_spClouds->getPositionX() <= -_spClouds->getContentSize().width * 0.5f)
            _spClouds->setPositionX(WIN_SIZE.width + _spClouds->getContentSize().width * 0.7f);
        _spClouds->setPositionX(_spClouds->getPositionX() - _parallaxSpeedX * dt * 0.5f);
        
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
            }
        }
        
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
