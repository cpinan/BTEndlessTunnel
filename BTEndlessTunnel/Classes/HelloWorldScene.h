#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "BaseVehicle.h"
#include "AppMacros.h"
#include "HudLayer.h"
#include "Constants.h"



class HelloWorld : public cocos2d::CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    virtual ~HelloWorld();

    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    void update(float dt);
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(HelloWorld);
    
    void draw();
    
private:
    
    void _removeNode(cocos2d::CCNode* node);
    
private:
    float _parallaxSpeedX;
    
    //BaseVehicle* _player;
    std::vector<int> _vectorMap;
    int _currentItemMap;
    
    cocos2d::CCSprite* _spClouds;    
    cocos2d::CCArray* _parallaxBackground;
    cocos2d::CCArray* _parallaxFloor;
    cocos2d::CCArray* _arrayObstacles;
    
    cocos2d::CCLabelTTF* _lblScore;
    
    bool gameOver;
    
};

#endif // __HELLOWORLD_SCENE_H__
