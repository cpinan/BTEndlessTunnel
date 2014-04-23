//
//  LocalStorageManager.cpp
//  BTEndlessTunnel
//
//  Created by NSS on 4/1/14.
//
//

#include "LocalStorageManager.h"
#include "Constants.h"
#include "Globals.h"

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
    return defaults()->getFloatForKey(CCString::createWithFormat("%s%s", USER_MAX_SCORE, user_data_id.c_str())->getCString(), 0);
}

void LocalStorageManager::setMaxScore(float score)
{
    defaults()->setFloatForKey(CCString::createWithFormat("%s%s", USER_MAX_SCORE, user_data_id.c_str())->getCString(), score);
    defaults()->flush();
}

void LocalStorageManager::incrementTotalGamesPlayed()
{
    defaults()->setIntegerForKey(CCString::createWithFormat("%s%s", USER_TOTAL_GAMES_PLAYED, user_data_id.c_str())->getCString(), getTotalGamesPlayed() + 1);
    defaults()->flush();
}

int LocalStorageManager::getTotalGamesPlayed()
{
    return defaults()->getIntegerForKey(CCString::createWithFormat("%s%s", USER_TOTAL_GAMES_PLAYED, user_data_id.c_str())->getCString(), 0);
}

void LocalStorageManager::setTotalScore(float score)
{
    defaults()->setFloatForKey(CCString::createWithFormat("%s%s", USER_TOTAL_SCORE, user_data_id.c_str())->getCString(), getTotalScore() + score);
    defaults()->flush();
    
    incrementTotalGamesPlayed();    
}

float LocalStorageManager::getTotalScore()
{
    return defaults()->getFloatForKey(CCString::createWithFormat("%s%s", USER_TOTAL_SCORE, user_data_id.c_str())->getCString(), 0);
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
    return defaults()->getBoolForKey(CCString::createWithFormat("%s%s", achievementID, user_data_id.c_str())->getCString(), false);
}

void LocalStorageManager::unlockAchievement(const char *achievementID)
{
    defaults()->setBoolForKey(CCString::createWithFormat("%s%s", achievementID, user_data_id.c_str())->getCString(), true);
    defaults()->flush();
}

void LocalStorageManager::setScoreInLevel(float score, int level)
{
    CCString* key = CCString::createWithFormat(CCString::createWithFormat("%s%s", USER_SCORE_IN_LEVEL, user_data_id.c_str())->getCString(), level);
    defaults()->setFloatForKey(key->getCString(), score);
    defaults()->flush();
}

float LocalStorageManager::getScoreInLevel(int level)
{
    CCString* key = CCString::createWithFormat(CCString::createWithFormat("%s%s", USER_SCORE_IN_LEVEL, user_data_id.c_str())->getCString(), level);
    return defaults()->getFloatForKey(key->getCString(), 0);
}

void LocalStorageManager::updateObstaclesJumped(int num)
{
    defaults()->setIntegerForKey(CCString::createWithFormat("%s%s", USER_OBSTACLES_JUMPED, user_data_id.c_str())->getCString(), num);
    defaults()->flush();
}

int LocalStorageManager::getObstaclesJumped()
{
    return defaults()->getIntegerForKey(CCString::createWithFormat("%s%s", USER_OBSTACLES_JUMPED, user_data_id.c_str())->getCString(), 0);
}

void LocalStorageManager::setMute(bool state)
{
    defaults()->setBoolForKey(USER_MANAGE_MUSIC, state);
    defaults()->flush();
}

bool LocalStorageManager::isMute()
{
    return defaults()->getBoolForKey(USER_MANAGE_MUSIC, false);
}

bool LocalStorageManager::showTutorial()
{
    int usingJoypad = 0;
    if(isUsingJoypad())
        usingJoypad = 1;
    
    CCString* key = CCString::createWithFormat(USER_TUTORIAL, usingJoypad);
    return defaults()->getBoolForKey(key->getCString(), true);
}

void LocalStorageManager::isTutorialOn(bool state, int mode)
{
    int usingJoypad = mode;
    if(usingJoypad == -1)
    {
        if(isUsingJoypad())
            usingJoypad = 1;
        else
            usingJoypad = 0;
    }
    
    CCString* key = CCString::createWithFormat(USER_TUTORIAL, usingJoypad);
    defaults()->setBoolForKey(key->getCString(), state);
    defaults()->flush();
}