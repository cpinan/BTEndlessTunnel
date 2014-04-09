//
//  PopUpLoseLayer.cpp
//  BTEndlessTunnel
//
//  Created by NSS on 4/1/14.
//
//

#include "PopUpLoseLayer.h"
#include "Constants.h"
#include "HomeScene.h"
#include "AppMacros.h"
#include "LocalStorageManager.h"
#include "Utils.h"
#include "PlayGameConstants.h"
#include "GameLayer.h"
#include "NativeUtils.h"

using namespace cocos2d;

PopUpLoseLayer::PopUpLoseLayer()
{
    _score = 0;
    disable = false;
    
    // BGWhite
    CCSprite* bg_white = CCSprite::create("bg_white.png");
    bg_white->setPosition(ccp(designResolutionSize.width * 0.5f, designResolutionSize.height * 0.5f));
    addChild(bg_white);
    
    // Head title
    _lblHeadTitle = CCLabelTTF::create("Finish!", "Arial", 16.0f, CCSizeMake(190, 20), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    _lblHeadTitle->setAnchorPoint(ccp(0, 0));
    _lblHeadTitle->setPosition(ccp(144, designResolutionSize.height - 73));
    _lblHeadTitle->setColor(ccc3(0, 0, 0));
    addChild(_lblHeadTitle);
    
    // Badge and record
    CCSprite* spBadge = CCSprite::create("badge.png");
    spBadge->setAnchorPoint(ccp(0, 0));
    spBadge->setPosition(ccp(163, designResolutionSize.height -  150));
    addChild(spBadge, 10);
    
    _lblScore = CCLabelTTF::create("0", "Arial", 16.0f, CCSizeMake(130, 20), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    _lblScore->setColor(ccc3(0, 0, 0));
    _lblScore->setAnchorPoint(ccp(0, 0));
    _lblScore->setPosition(ccp(223, spBadge->getPositionY() + 8));
    addChild(_lblScore);
    
    _lblMaxScore = CCLabelTTF::create("Max Score: 0", "Arial", 16.0f, CCSizeMake(200, 20), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    _lblMaxScore->setColor(ccc3(0, 0, 0));
    _lblMaxScore->setAnchorPoint(ccp(0, 0));
    _lblMaxScore->setPosition(ccp(160, spBadge->getPositionY() + 50));
    addChild(_lblMaxScore);
    
    _lblTotalScore = CCLabelTTF::create("Total Score: 0", "Arial", 16.0f, CCSizeMake(200, 20), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    _lblTotalScore->setColor(ccc3(0, 0, 0));
    _lblTotalScore->setAnchorPoint(ccp(0, 0));
    _lblTotalScore->setPosition(ccp(_lblMaxScore->getPositionX(), spBadge->getPositionY() - 30));
    addChild(_lblTotalScore);
    
    // Send score button
    CCMenuItemImage* itemScore = CCMenuItemImage::create("play_game.png", "play_game.png", this, menu_selector(PopUpLoseLayer::_onOptionPressed));
    itemScore->setTag(kTagSendScore);
    itemScore->setAnchorPoint(ccp(0, 0));
    itemScore->setPosition(ccp(161, designResolutionSize.height - 250));
    
    // Home button
    CCMenuItemImage* itemHome = CCMenuItemImage::create("home.png", "home.png", this, menu_selector(PopUpLoseLayer::_onOptionPressed));
    itemHome->setTag(kTagGoHome);
    itemHome->setAnchorPoint(ccp(0, 0));
    itemHome->setPosition(ccp(222, itemScore->getPositionY()));
    
    // Play again button
    CCMenuItemImage* itemPlayAgain = CCMenuItemImage::create("play_again.png", "play_again.png", this, menu_selector(PopUpLoseLayer::_onOptionPressed));
    itemPlayAgain->setTag(kTagPlayAgain);
    itemPlayAgain->setAnchorPoint(ccp(0, 0));
    itemPlayAgain->setPosition(ccp(282, itemScore->getPositionY()));
    
    // Menu
    CCMenu* menu = CCMenu::create();
    menu->setAnchorPoint(ccp(0, 0));
    menu->setPosition(CCPointZero);
    
    menu->addChild(itemScore);
    menu->addChild(itemHome);
    menu->addChild(itemPlayAgain);
    
    addChild(menu);
    
}

void PopUpLoseLayer::updateScore(int level, float score)
{
    _lblScore->setString(CCString::createWithFormat("%d", (int) score)->getCString());
    long longScore = (long) score;
    _score = longScore;
    // Check Achievements
    
    if(LocalStorageManager::getTotalGamesPlayed() < 10)
        Utils::incrementAchievement(ACH_PLAY_10_TIMES, 1);
    
    if(LocalStorageManager::getTotalGamesPlayed() < 100)
        Utils::incrementAchievement(ACH_PLAY_100_TIMES, 1);
    
    if(LocalStorageManager::getTotalGamesPlayed() < 1000)
        Utils::incrementAchievement(ACH_PLAY_1000_TIMES, 1);
    
    LocalStorageManager::setScore(score);
    
    long totalScore = (long) LocalStorageManager::getTotalScore();
    
    if(!LocalStorageManager::isAchievementUnlocked(ACH_AVERAGE_5000_OR_MORE_IN_50_GAMES_OR_MORE) &&  LocalStorageManager::getTotalGamesPlayed() >= 50 && LocalStorageManager::getAverageScore() >= 5000)
    {
        Utils::unlockAchievement(ACH_AVERAGE_5000_OR_MORE_IN_50_GAMES_OR_MORE);
        LocalStorageManager::unlockAchievement(ACH_AVERAGE_5000_OR_MORE_IN_50_GAMES_OR_MORE);
    }
    
    if(!LocalStorageManager::isAchievementUnlocked(ACH_GET_TOTAL_SCORE_100000_OR_MORE) && totalScore >= 100000)
    {
        Utils::unlockAchievement(ACH_GET_TOTAL_SCORE_100000_OR_MORE);
        LocalStorageManager::unlockAchievement(ACH_GET_TOTAL_SCORE_100000_OR_MORE);
    }
    
    long scoreInLevel = (long) LocalStorageManager::getScoreInLevel(level);
    
    if(longScore > scoreInLevel)
    {
        if(level == kGameLevelEasy)
        {
            Utils::submitScore(LEAD_EASY_MODE, longScore);
        }
        else if (level == kGameLevelNormal)
        {
            Utils::submitScore(LEAD_NORMAL_MODE, longScore);
        }
        else if(level == kGameLevelHard)
        {
            Utils::submitScore(LEAD_HARD_MODE, longScore);
        }
        
        LocalStorageManager::setScoreInLevel(score, level);
        scoreInLevel = score;
        _lblHeadTitle->setString("New Record!");
        _lblMaxScore->setString("");
    }
    else
    {
        _lblMaxScore->setString(CCString::createWithFormat("Best: %lu", scoreInLevel)->getCString());
    }
    
    _lblTotalScore->setString(CCString::createWithFormat("Total Score: %lu", totalScore)->getCString());
}

void PopUpLoseLayer::_onOptionPressed(CCObject *pSender)
{
    if(disable)
        return;
    
    CCMenuItem* item = (CCMenuItem*) pSender;
    
    switch (item->getTag()) {
        case kTagPlayAgain:
            CCNotificationCenter::sharedNotificationCenter()->postNotification(NOTIFICATION_PLAY_AGAIN);
            break;
            
        case kTagGoHome:
            CCNotificationCenter::sharedNotificationCenter()->postNotification(NOTIFICATION_GO_HOME);
            break;
            
        case kTagSendScore:
            if(!NativeUtils::isSignedIn())
            {
                NativeUtils::signIn();
            }
            else
            {
                CCMessageBox("Already signed", "Play Game");
            }

            break;

    }

    
}