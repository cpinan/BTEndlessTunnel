//
//  ControllerSingleLeaderboard.m
//  GooglePlayGameServices
//
//  Created by NSS on 4/7/14.
//
//

#import "PlayGameSingleton.h"
#include "AdMobBannerView.h"
#include "Globals.h"
#import "GCHelper.h"

#import <UIKit/UIKit.h>

#include "cocos2d.h"

#define APP_ID 864587748 //id from iTunesConnect

using namespace cocos2d;

#pragma mark - Declare Views
AdMobBannerView* adMobBannerView = 0;

#pragma mark - Destructor and Constructor
PlayGameSingleton::~PlayGameSingleton()
{
    rootController = 0;
}

PlayGameSingleton::PlayGameSingleton()
{
}

#pragma mark - Singleton
PlayGameSingleton& PlayGameSingleton::sharedInstance()
{
    static PlayGameSingleton instance;
    return instance;
}

#pragma mark - Single Leaderboard
void PlayGameSingleton::showSingleLeaderboard(const char* leaderBoardID)
{    
    if(!isSignedIn())
        authenticate();
    
    NSString* name = [NSString stringWithUTF8String:leaderBoardID];
    [[GCHelper sharedInstance] showLeaderboard:name];
}

void PlayGameSingleton::finishSingleLeaderboard()
{
}

#pragma mark - Leaderboards Picker
void PlayGameSingleton::showLeaderboards()
{
    
    if(!isSignedIn())
    {
        authenticate();
        return;
    }
    
    [[GCHelper sharedInstance] showLeaderboard:nil];
}

void PlayGameSingleton::finishLeaderboards()
{
}

#pragma mark - Submit score
void PlayGameSingleton::submitScore(long score, const char *leaderBoardID)
{
    
    if(!isSignedIn())
        return;

    NSString* name = [NSString stringWithUTF8String:leaderBoardID];
    [[GCHelper sharedInstance] submitScore: score forCategory:name];
    
}

#pragma mark - Achievements
void PlayGameSingleton::showAchievements()
{
    
    if(!isSignedIn())
    {
        authenticate();
        return;
    }
    
    [[GCHelper sharedInstance] showAchievements];
}

void PlayGameSingleton::finishAchievements()
{
}

#pragma mark - Manage achievements
void PlayGameSingleton::revealAchievement(const char *achievementID)
{
    if(!isSignedIn())
        return;
}

void PlayGameSingleton::unlockAchievement(const char *achievementID)
{
    if(!isSignedIn())
        return;
    
    if(strlen(achievementID) == 0)
        return;
    
    NSString* name = [NSString stringWithUTF8String:achievementID];
    [[GCHelper sharedInstance] reportAchievementIdentifier:name percentComplete:100];
    
}

void PlayGameSingleton::incrementPercentageAchievement(float percentage, const char *achievementID)
{
    if(!isSignedIn())
        return;
    
    if(strlen(achievementID) == 0)
        return;
    
    NSString* name = [NSString stringWithUTF8String:achievementID];
    [[GCHelper sharedInstance] reportAchievementIdentifier:name percentComplete:percentage];
}

void PlayGameSingleton::incrementAchievement(int numSteps, const char *achievementID)
{
    if(!isSignedIn())
        return;
}

#pragma mark - Login configuration
void PlayGameSingleton::trySilentAuthentication()
{
}

void PlayGameSingleton::authenticate()
{
    [[GCHelper sharedInstance] authenticateLocalUser];
}

bool PlayGameSingleton::isSignedIn()
{
    // CCLOG("user_data_id: %s", user_data_id.c_str());
    return user_data_id != "";
}

void PlayGameSingleton::signOut()
{
    if(isSignedIn())
    {

    }
}

#pragma mark - Advertisement
void PlayGameSingleton::initAd()
{
    if(!adMobBannerView)
        adMobBannerView = [[AdMobBannerView alloc] init];
    
    
    UIWindow *window =  [[UIApplication sharedApplication] keyWindow];
    
    if(!rootController)
        rootController = window.rootViewController;
    
    [((UIViewController *) rootController).view addSubview: adMobBannerView.view];
}


void PlayGameSingleton::showAd()
{
    [adMobBannerView show];
}

void PlayGameSingleton::hideAd()
{
    [adMobBannerView hide];
    //[adMobBannerView.view removeFromSuperview];
    //[adMobBannerView release];
    //adMobBannerView = 0;
}

void PlayGameSingleton::rateApp()
{
    NSString *reviewURL = [NSString stringWithFormat:@"itms-apps://ax.itunes.apple.com/WebObjects/MZStore.woa/wa/viewContentsUserReviews?type=Purple+Software&id=%d", APP_ID];
    
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:reviewURL]];
    
}

