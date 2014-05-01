//
//  ControllerSingleLeaderboard.m
//  GooglePlayGameServices
//
//  Created by NSS on 4/7/14.
//
//

#import "PlayGameSingleton.h"
#import "AdMobBannerView.h"
#import "Globals.h"
#import <FacebookSDK/FacebookSDK.h>
#import "GCHelper.h"
#import "GameLayer.h"
#import <UIKit/UIKit.h>

#import "cocos2d.h"

#define APP_ID 864587748 //id from iTunesConnect

using namespace cocos2d;

// A function for parsing URL parameters returned by the Feed Dialog.
NSDictionary* parseURLParams(NSString* query)
{
    NSArray *pairs = [query componentsSeparatedByString:@"&"];
    NSMutableDictionary *params = [[NSMutableDictionary alloc] init];
    for (NSString *pair in pairs) {
        NSArray *kv = [pair componentsSeparatedByString:@"="];
        NSString *val =
        [kv[1] stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
        params[kv[0]] = val;
    }
    return params;
}

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
    
    CCDirector::sharedDirector()->stopAnimation();
    NSString* name = [NSString stringWithUTF8String:leaderBoardID];
    [[GCHelper sharedInstance] showLeaderboard:name];
}

void PlayGameSingleton::finishSingleLeaderboard()
{
    CCDirector::sharedDirector()->startAnimation();
}

#pragma mark - Leaderboards Picker
void PlayGameSingleton::showLeaderboards()
{
    
    if(!isSignedIn())
    {
        authenticate();
        return;
    }
    
    CCDirector::sharedDirector()->stopAnimation();
    [[GCHelper sharedInstance] showLeaderboard:nil];
}

void PlayGameSingleton::finishLeaderboards()
{
    CCDirector::sharedDirector()->startAnimation();
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
    
    CCDirector::sharedDirector()->stopAnimation();
    [[GCHelper sharedInstance] showAchievements];
}

void PlayGameSingleton::finishAchievements()
{
    CCDirector::sharedDirector()->startAnimation();
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

void PlayGameSingleton::shareOnFacebook(long score, int level, int obstacles)
{
    
    ccLanguageType language = CCApplication::sharedApplication()->getCurrentLanguage();
    
    NSString* str_level = @"fácil";
    if(level == kGameLevelNormal)
        str_level = @"normal";
    else if(level == kGameLevelHard)
        str_level = @"difícil";
    
    if(language == kLanguageEnglish)
    {
       if(level == kGameLevelEasy)
           str_level = @"easy";
        else if(level == kGameLevelHard)
            str_level = @"hard";
    }
    
    NSString* linkToShare = @"https://itunes.apple.com/us/app/turbo-race/id864587748?ls=1&mt=8";
    
    NSString* name = @"Turbo Race";
    
    NSString* caption = @"Estoy jugando Turbo Race";
    if(language == kLanguageEnglish)
        caption = @"I am playing Turbo Race";
    
    NSString* picture = @"http://carlospinan.com/icon/turbo_race.png";
    
    NSString* str_format = @"He obtenido %ld puntos en modo %@ y he esquivado %d obstáculos.";
    if(language == kLanguageEnglish)
        str_format = @"I got %ld points in %@ mode and I avoided %d obstacles.";
    
    NSString* description = [NSString stringWithFormat:str_format, score, str_level, obstacles];
    
    NSURL* params_link = [NSURL URLWithString:linkToShare];
    NSURL* params_picture = [NSURL URLWithString:picture];
    
    // Check if the Facebook app is installed and we can present the share dialog
    FBShareDialogParams *params = [[FBShareDialogParams alloc] init];
    params.link = params_link;
    params.name = name;
    params.caption = caption;
    params.picture = params_picture;
    params.description = description;
    
    // If the Facebook app is installed and we can present the share dialog
    if ([FBDialogs canPresentShareDialogWithParams:params]) {
        
        // Present share dialog
        [FBDialogs presentShareDialogWithLink:params.link
                                         name:params.name
                                      caption:params.caption
                                  description:params.description
                                      picture:params.picture
                                  clientState:nil
                                      handler:^(FBAppCall *call, NSDictionary *results, NSError *error) {
                                          if(error) {
                                              // An error occurred, we need to handle the error
                                              // See: https://developers.facebook.com/docs/ios/errors
                                          } else {
                                              // Success
                                              NSLog(@"result %@", results);
                                          }
                                      }];
        
        
    } else {
        // Present the feed dialog
        
        // Put together the dialog parameters
        NSMutableDictionary *params2 = [NSMutableDictionary dictionaryWithObjectsAndKeys:
                                       name, @"name",
                                       caption, @"caption",
                                       description, @"description",
                                       linkToShare, @"link",
                                       picture, @"picture",
                                       nil];
        
        // Show the feed dialog
        [FBWebDialogs presentFeedDialogModallyWithSession:nil
                                               parameters:params2
                                                  handler:^(FBWebDialogResult result, NSURL *resultURL, NSError *error) {
                                                      if (error) {
                                                          // An error occurred, we need to handle the error
                                                          // See: https://developers.facebook.com/docs/ios/errors
                                                      } else {
                                                          if (result == FBWebDialogResultDialogNotCompleted) {
                                                              // User cancelled.
                                                              NSLog(@"User cancelled.");
                                                          } else {
                                                              // Handle the publish feed callback
                                                              NSDictionary *urlParams = parseURLParams([resultURL query]);
                                                              
                                                              if (![urlParams valueForKey:@"post_id"]) {
                                                                  // User cancelled.
                                                                  NSLog(@"User cancelled.");
                                                                  
                                                              } else {
                                                                  // User clicked the Share button
                                                                  NSString *result = [NSString stringWithFormat: @"Posted story, id: %@", [urlParams valueForKey:@"post_id"]];
                                                                  NSLog(@"result %@", result);
                                                              }
                                                          }
                                                      }
                                                  }];
        
    }
    
    [params_link release];
    [params_picture release];
    [description release];
    
}

