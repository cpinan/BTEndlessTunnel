//
//  BTEndlessTunnelAppDelegate.cpp
//  BTEndlessTunnel
//
//  Created by NSS on 3/21/14.
//  Copyright __MyCompanyName__ 2014. All rights reserved.
//

#include "AppDelegate.h"

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "AppMacros.h"
#include "Constants.h"
#include "HomeScene.h"
#include "GameLayer.h"

#include "HelloWorldScene.h"

#include <vector>
#include <string>

USING_NS_CC;
using namespace CocosDenshion;
using namespace std;

AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    CCEGLView* pEglView = CCEGLView::sharedOpenGLView();
    
    pDirector->setOpenGLView(pEglView);
    
    // Set resolution size and paths
    pEglView->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionNoBorder);
    CCSize frameSize = pEglView->getFrameSize();
    
    vector<string> searchPaths;
    
    float ratio = frameSize.width / frameSize.height;
    if(ratio <= 1.5)
    {
        // iPhone and iPads
    }
    else if(ratio <= 1.7)
    {
        // Intermediate devices
    }else
    {
        // iPhone5 and others
    }
    
    
    /*
    if(frameSize.height > smallResource.size.height)
    {
        searchPaths.push_back(mediumResource.directory);
        pDirector->setContentScaleFactor(MIN(mediumResource.size.height/designResolutionSize.height, mediumResource.size.width/designResolutionSize.width));
    }
    else
    {
        searchPaths.push_back(smallResource.directory);
        pDirector->setContentScaleFactor(MIN(smallResource.size.height/designResolutionSize.height, smallResource.size.width/designResolutionSize.width));
    }
    */
    
    searchPaths.push_back(mediumResource.directory);
    pDirector->setContentScaleFactor(MIN(mediumResource.size.height/designResolutionSize.height, mediumResource.size.width/designResolutionSize.width));
    
    searchPaths.push_back("audio");
    CCFileUtils::sharedFileUtils()->setSearchPaths(searchPaths);
    
    SimpleAudioEngine::sharedEngine()->preloadEffect(SFX_JUMP);
    SimpleAudioEngine::sharedEngine()->preloadEffect(SFX_SWOOSH);
    SimpleAudioEngine::sharedEngine()->preloadEffect(SFX_SMASH);
    SimpleAudioEngine::sharedEngine()->preloadEffect(SFX_BUTTON);
    
    SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(BG_MUSIC_01);
    SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(BG_MUSIC_02);
    SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(BG_MUSIC_03);
    
    SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(BG_MUSIC_VOLUME);
    SimpleAudioEngine::sharedEngine()->setEffectsVolume(1.0f);
    

    // turn on display FPS
    pDirector->setDisplayStats(SHOW_FPS);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    CCScene *pScene = HomeScene::scene(kGameModeHome);
    // CCScene *pScene = HelloWorld::scene();
    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
#endif
    SimpleAudioEngine::sharedEngine()->pauseAllEffects();
    CCNotificationCenter::sharedNotificationCenter()->postNotification(NOTIFICATION_PAUSE_GAME);
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
#endif
    SimpleAudioEngine::sharedEngine()->resumeAllEffects();
}
