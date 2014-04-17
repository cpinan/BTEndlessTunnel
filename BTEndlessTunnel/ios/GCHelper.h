//
//  GCHelper.h
//  BTEndlessTunnel
//
//  Created by Carlos Pinan on 16/04/14.
//
//

#import <Foundation/Foundation.h>
#import <GameKit/GameKit.h>

@interface GCHelper : NSObject <GKLeaderboardViewControllerDelegate, GKAchievementViewControllerDelegate, GKGameCenterControllerDelegate>{
    BOOL gameCenterAvailable;
    BOOL userAuthenticated;
}

@property (assign, readonly) BOOL gameCenterAvailable;

+ (GCHelper *)sharedInstance;

- (void)authenticateLocalUser;

- (void)submitScore:(int64_t) score forCategory: (NSString*) category;
- (void)showLeaderboard:(NSString *) leaderboardID;
- (void)showAchievements;
- (void)reportAchievementIdentifier: (NSString*) identifier percentComplete: (float) percent;

@end