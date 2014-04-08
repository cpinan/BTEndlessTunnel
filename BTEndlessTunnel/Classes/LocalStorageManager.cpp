//
//  LocalStorageManager.cpp
//  BTEndlessTunnel
//
//  Created by NSS on 4/1/14.
//
//

#include "LocalStorageManager.h"
#include "Constants.h"

using namespace cocos2d;

LocalStorageManager::LocalStorageManager()
{
    // Nothing to do here.
}

CCUserDefault* LocalStorageManager::defaults()
{
    return CCUserDefault::sharedUserDefault();
}

bool LocalStorageManager::isUsingJoypad()
{
    return defaults()->getBoolForKey(USER_CONTROL_IS_JOYPAD, true);
}

void LocalStorageManager::setControlType(bool joypad)
{
    defaults()->setBoolForKey(USER_CONTROL_IS_JOYPAD, joypad);
    defaults()->flush();
}

int LocalStorageManager::getSelectedPlayer()
{
    return defaults()->getIntegerForKey(USER_PLAYER_SELECTED, kStoragePlayerFrog);
}

void LocalStorageManager::setPlayer(StorageValues value)
{
    defaults()->setIntegerForKey(USER_PLAYER_SELECTED, (int) value);
    defaults()->flush();
}

float LocalStorageManager::getMaxScore()
{
    return defaults()->getFloatForKey(USER_MAX_SCORE, 0);
}

void LocalStorageManager::setMaxScore(float score)
{
    defaults()->setFloatForKey(USER_MAX_SCORE, score);
    defaults()->flush();
}

void LocalStorageManager::incrementTotalGamesPlayed()
{
    defaults()->setIntegerForKey(USER_TOTAL_GAMES_PLAYED, getTotalGamesPlayed() + 1);
    defaults()->flush();
}

int LocalStorageManager::getTotalGamesPlayed()
{
    return defaults()->getIntegerForKey(USER_TOTAL_GAMES_PLAYED, 0);
}

void LocalStorageManager::setTotalScore(float score)
{
    defaults()->setFloatForKey(USER_TOTAL_SCORE, getTotalScore() + score);
    defaults()->flush();
    
    incrementTotalGamesPlayed();    
}

float LocalStorageManager::getTotalScore()
{
    return defaults()->getFloatForKey(USER_TOTAL_SCORE, 0);
}

float LocalStorageManager::getAverageScore()
{
    int totalGamesPlayed = getTotalGamesPlayed();
    
    if(totalGamesPlayed == 0)
        totalGamesPlayed = 1;
    
    float totalScore = getTotalScore();
    
    return totalScore / totalGamesPlayed;
}

void LocalStorageManager::setScore(float score)
{
    if(score > getMaxScore())
        setMaxScore(score);
    
    setTotalScore(score);
}

bool LocalStorageManager::isAchievementUnlocked(const char *achievementID)
{
    return defaults()->getBoolForKey(achievementID, false);
}

void LocalStorageManager::unlockAchievement(const char *achievementID)
{
    defaults()->setBoolForKey(achievementID, true);
    defaults()->flush();
}

