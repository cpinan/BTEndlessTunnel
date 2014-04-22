//
//  Utils.h
//  BTEndlessTunnel
//
//  Created by NSS on 4/1/14.
//
//

#ifndef __BTEndlessTunnel__Utils__
#define __BTEndlessTunnel__Utils__

#include "cocos2d.h"

class Utils
{
public:
    static void unlockAchievement(const char* achievementID);
    static void incrementAchievement(const char* achievementID, int numSteps);
    static void incrementPercentageAchievement(const char* achievementID, float percentage);
    static void submitScore(const char* leaderboardID, float score);
    static float randomBetween(float min, float max);
};

#endif /* defined(__BTEndlessTunnel__Utils__) */
