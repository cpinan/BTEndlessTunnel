//
//  GCHelper.m
//  BTEndlessTunnel
//
//  Created by Carlos Pinan on 16/04/14.
//
//

#import "GCHelper.h"
#import "Globals.h"

@implementation GCHelper

@synthesize gameCenterAvailable;

#pragma mark Initialization

static GCHelper *sharedHelper = nil;
+ (GCHelper *) sharedInstance {
    if (!sharedHelper) {
        sharedHelper = [[GCHelper alloc] init];
    }
    return sharedHelper;
}

- (BOOL)isGameCenterAvailable {
    // check for presence of GKLocalPlayer API
    Class gcClass = (NSClassFromString(@"GKLocalPlayer"));
    
    NSString *reqSysVer = @"6.1";
    NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
    BOOL osVersionSupported = ([currSysVer compare:reqSysVer
                                           options:NSNumericSearch] != NSOrderedAscending);
    
    return (gcClass && osVersionSupported);
}

- (id)init {
    if ((self = [super init])) {
        gameCenterAvailable = [self isGameCenterAvailable];
        if (gameCenterAvailable) {
            NSNotificationCenter *nc =
            [NSNotificationCenter defaultCenter];
            [nc addObserver:self
                   selector:@selector(authenticationChanged)
                       name:GKPlayerAuthenticationDidChangeNotificationName
                     object:nil];
        }
    }
    return self;
}

- (void)authenticationChanged {
    
    if ([GKLocalPlayer localPlayer].isAuthenticated && !userAuthenticated) {
        //NSLog(@"Authentication changed: player authenticated.");
        user_data_id = [[GKLocalPlayer localPlayer].playerID UTF8String];
        userAuthenticated = TRUE;
    } else if (![GKLocalPlayer localPlayer].isAuthenticated && userAuthenticated) {
       // NSLog(@"Authentication changed: player not authenticated");
        user_data_id = "";
        userAuthenticated = FALSE;
    }
    
}

#pragma mark User functions

- (void)authenticateLocalUser {
    
    if (!gameCenterAvailable)
        return;
    
   // NSLog(@"Authenticating local user...");
    
    
    if ([GKLocalPlayer localPlayer].authenticated == NO) {
        
        GKLocalPlayer *localPlayer = [GKLocalPlayer localPlayer];
        
        localPlayer.authenticateHandler = ^(UIViewController *viewController,NSError *error) {
            if (localPlayer.authenticated) {
                //localPlayer.playerID
                user_data_id = [localPlayer.playerID UTF8String];
               // NSLog(@"Already authenticated");
                // [self authenticatedPlayer: localPlayer];
                //already authenticated
            } else if(viewController) {
                //NSLog(@"Present the login form");
                // [self presentViewController:viewController];//present the login form
                [self showAuthenticationDialogWhenReasonable: viewController];
            } else {
               // NSLog(@"Problem with auth");
                [self disableGameCenter];
                //problem with authentication,probably bc the user doesn't use Game Center
            } 
        };
        
    } else {
        
        
        //NSLog(@"Already authenticated!");
        
        
    }
    
}


-(void)showAuthenticationDialogWhenReasonable:(UIViewController *)controller
{
    [[[[[UIApplication sharedApplication] delegate] window] rootViewController] presentViewController:controller animated:YES completion:nil];
}

-(void)authenticatedPlayer:(GKLocalPlayer *)player
{
    // player = _localPlayer;
}

-(void)disableGameCenter
{
    
}

-(void)showLeaderboard:(NSString *) leaderboardID
{
   // NSLog(@"showLeaderboard");
    UIWindow *window =  [[UIApplication sharedApplication] keyWindow];
    UIViewController* rootController = window.rootViewController;
    
    GKLeaderboardViewController *leaderboardController = [[GKLeaderboardViewController alloc] init];
    if (leaderboardController != NULL)
    {
        // if (leaderboardID != nil) {//keep category nil and let user see default
            leaderboardController.category = leaderboardID;
        // }
        leaderboardController.timeScope = GKLeaderboardTimeScopeAllTime;
        leaderboardController.leaderboardDelegate = self;
        [rootController presentViewController:leaderboardController animated:NO completion:nil];
    }
    
}

-(void)leaderboardViewControllerDidFinish:(GKLeaderboardViewController *)leaderboardController
{
    // NSLog(@"leaderboardViewControllerDidFinish");
    UIWindow *window =  [[UIApplication sharedApplication] keyWindow];
    UIViewController* rootController = window.rootViewController;
    
    [rootController dismissViewControllerAnimated:NO completion:nil];
    [leaderboardController release];
}

-(void)showAchievements
{
   // NSLog(@"showAchievements");
    UIWindow *window =  [[UIApplication sharedApplication] keyWindow];
    UIViewController* rootController = window.rootViewController;
    
    /*
    GKAchievementViewController *achievements = [[GKAchievementViewController alloc] init];
	if (achievements != NULL)
	{
		achievements.achievementDelegate = self;
		[rootController presentViewController:achievements animated:NO completion:nil];
	}
     */
    
    GKGameCenterViewController *gameCenterController = [[GKGameCenterViewController alloc] init];
    
    if (gameCenterController != nil)
    {
        gameCenterController.gameCenterDelegate = self;
        gameCenterController.viewState = GKGameCenterViewControllerStateAchievements;
        
        [rootController presentViewController: gameCenterController animated:NO completion:nil];
        
    }
}

- (void)achievementViewControllerDidFinish:(GKAchievementViewController *)viewController;
{
   // NSLog(@"achievementViewControllerDidFinish");
    UIWindow *window =  [[UIApplication sharedApplication] keyWindow];
    UIViewController* rootController = window.rootViewController;
    
    [rootController dismissViewControllerAnimated:NO completion:nil];
	[viewController release];
}

- (void)gameCenterViewControllerDidFinish:(GKGameCenterViewController *)gameCenterViewController
{
   // NSLog(@"gameCenterViewControllerDidFinish");
    UIWindow *window =  [[UIApplication sharedApplication] keyWindow];
    UIViewController* rootController = window.rootViewController;
    
    [rootController dismissViewControllerAnimated:NO completion:nil];
	[gameCenterViewController release];
    
}

- (void)submitScore: (int64_t) score forCategory: (NSString*) category
{
	GKScore *scoreReporter = [[[GKScore alloc] initWithCategory:category] autorelease];
	scoreReporter.value = score;
	[scoreReporter reportScoreWithCompletionHandler: ^(NSError *error)
	 {
         dispatch_async(dispatch_get_main_queue(), ^(void)
                        {
                            if (error != nil)
                            {
                               // NSLog(@"%lli",score);
                            }
                            else{
                               // NSLog(@"Succeded");
                                
                            }
                        });
	 }];
}

- (void)reportAchievementIdentifier: (NSString*) identifier percentComplete: (float) percent
{
    GKAchievement *achievement = [[GKAchievement alloc] initWithIdentifier: identifier];
    achievement.showsCompletionBanner = YES;
    if (achievement && achievement.percentComplete < 100)
    {
        achievement.percentComplete = percent;
        [achievement reportAchievementWithCompletionHandler:^(NSError *error)
         {
             
             dispatch_async(dispatch_get_main_queue(), ^(void)
                            {
                                if (error != nil)
                                {
                                   // NSLog(@"Error in reporting achievements: %@", error);
                                }
                                else
                                {
                                    
                                   // NSLog(@"Achievement unlocked");
                                }
                            });
             
         }];
    }
}

@end
