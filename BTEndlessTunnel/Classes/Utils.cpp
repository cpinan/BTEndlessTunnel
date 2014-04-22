//
//  Utils.cpp
//  BTEndlessTunnel
//
//  Created by NSS on 4/1/14.
//
//

#include "Utils.h"
#include "AppMacros.h"
#include "NativeUtils.h"

using namespace cocos2d;

void Utils::unlockAchievement(const char *achievementID)
{
    NativeUtils::unlockAchievement(achievementID);
}

void Utils::incrementAchievement(const char *achievementID, int numSteps)
{
    NativeUtils::incrementAchievement(achievementID, numSteps);
}

void Utils::incrementPercentageAchievement(const char *achievementID, float percentage)
{
    NativeUtils::incrementPercentageAchievement(achievementID, percentage);
}

void Utils::submitScore(const char* leaderboardID, float score)
{
    NativeUtils::submitScore(leaderboardID, (long) score);
}

float Utils::randomBetween(float min, float max)
{
    float random = min + CCRANDOM_0_1() * (max - min + 1);
    return random;
}