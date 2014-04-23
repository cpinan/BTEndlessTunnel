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
    
    if(initWithColor(ccc4BFromccc4F(ccc4f(0, 0, 0, 100.0f / 255.0f))))
    {
        _leaderboardID = "";
        
        _score = 0;
        disable = false;
        
        CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
        CCPoint visibleOrigin = CCDirector::sharedDirector()->getVisibleOrigin();
        CCPoint origin = ccp(visibleOrigin.x + visibleSize.width * 0.5f, visibleOrigin.y + visibleSize.height* 0.5f);
        
        // BGWhite
        CCSprite* bg = CCSprite::create("gameover_screen.png");
        bg->setPosition(origin);
        addChild(bg);
        
        float w = bg->getContentSize().width;
        float h = bg->getContentSize().height;
        CCPoint o = ccp(w * 0.5f, h * 0.5f);
        
        // Badge and record
        spBadge = CCSprite::create("bicho_0004.png");
        spBadge->setPosition(ccp(o.x, o.y + spBadge->getContentSize().height * 0.5f));
        bg->addChild(spBadge, 10);
        
        _lblScore = CCLabelTTF::create("", FONT_GAME, SIZE_RATE_END, CCSizeMake(w * 0.5f, 20), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
        _lblScore->setPosition(ccp(o.x + spBadge->getContentSize().width * 0.8f, o.y - spBadge->getContentSize().height * 0.1f));
        _lblScore->setColor(ccWHITE);
        _lblScore->setRotation(-3);
        bg->addChild(_lblScore);
        
        _lblMaxScore = CCLabelTTF::create("", FONT_GAME, _lblScore->getFontSize(), CCSizeMake(w * 0.5f, 20), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
        _lblMaxScore->setPosition(ccp(_lblScore->getPositionX(), _lblScore->getPositionY() - spBadge->getContentSize().height * 0.28f));
        _lblMaxScore->setColor(ccWHITE);
        _lblMaxScore->setRotation(_lblScore->getRotation());
        bg->addChild(_lblMaxScore);
        
        // Send score button
        CCMenuItemImage* itemScore = CCMenuItemImage::create("btn_chart_2.png", "btn_chart_2.png", this, menu_selector(PopUpLoseLayer::_onOptionPressed));
        itemScore->setTag(kTagSendScore);
        itemScore->setPositionX(o.x - itemScore->getContentSize().width);
        itemScore->setPositionY(o.y - itemScore->getContentSize().height * 1.5f);
        
        // Home button
        CCMenuItemImage* itemHome = CCMenuItemImage::create("pause_home.png", "pause_home.png", this, menu_selector(PopUpLoseLayer::_onOptionPressed));
        itemHome->setTag(kTagGoHome);
        itemHome->setPositionX(itemScore->getPositionX() + itemScore->getContentSize().width * 1.1f);
        itemHome->setPositionY(itemScore->getPositionY());
        
        // Play again button
        CCMenuItemImage* itemPlayAgain = CCMenuItemImage::create("pause_replay.png", "pause_replay.png", this, menu_selector(PopUpLoseLayer::_onOptionPressed));
        itemPlayAgain->setTag(kTagPlayAgain);
        itemPlayAgain->setPositionX(itemHome->getPositionX() + itemScore->getContentSize().width * 1.1f);
        itemPlayAgain->setPositionY(itemScore->getPositionY());
        
        itemPlayAgain->setPositionY(itemPlayAgain->getPositionY() + itemScore->getContentSize().height * 0.2f);
        itemHome->setPositionY(itemHome->getPositionY() + itemScore->getContentSize().height * 0.1f);
        
        // Menu
        CCMenu* menu = CCMenu::create();
        menu->setAnchorPoint(ccp(0, 0));
        menu->setPosition(CCPointZero);
        
        menu->addChild(itemScore);
        menu->addChild(itemHome);
        menu->addChild(itemPlayAgain);
        
        bg->addChild(menu);

        
    }
}

void PopUpLoseLayer::updateScore(int level, float score)
{
    _lblScore->setString(CCString::createWithFormat("%d", (int) score)->getCString());
    long longScore = (long) score;
    _score = longScore;
    // Check Achievements
    
    int totalGamesPlayed = LocalStorageManager::getTotalGamesPlayed();
    
    // 10 Games played
    if(totalGamesPlayed <= 10)
    {
        if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        {
            Utils::incrementPercentageAchievement(ACH_PLAY_10_TIMES, 100.0f / (10.0f / totalGamesPlayed));
        }
        else
        {
            Utils::incrementAchievement(ACH_PLAY_10_TIMES, 1);
        }
    }
    
    // 100 Games played
    if(LocalStorageManager::getTotalGamesPlayed() <= 100)
    {
        if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        {
            Utils::incrementPercentageAchievement(ACH_PLAY_100_TIMES, 100.0f / (100.0f / totalGamesPlayed));
        }
        else
        {
            Utils::incrementAchievement(ACH_PLAY_100_TIMES, 1);
        }
    }
    
    // 1000 Games `layed
    if(LocalStorageManager::getTotalGamesPlayed() <= 1000)
    {
        if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        {
            Utils::incrementPercentageAchievement(ACH_PLAY_1000_TIMES, 100.0f / (1000.0f / totalGamesPlayed));
        }
        else
        {
            Utils::incrementAchievement(ACH_PLAY_1000_TIMES, 1);
        }
    }
    
    LocalStorageManager::setScore(score);
    
    long totalScore = (long) LocalStorageManager::getTotalScore();
    
    if(!LocalStorageManager::isAchievementUnlocked(ACH_AVERAGE_1000_OR_MORE_IN_50_GAMES_OR_MORE) &&  LocalStorageManager::getTotalGamesPlayed() >= 50 && LocalStorageManager::getAverageScore() >= 1000)
    {
        Utils::unlockAchievement(ACH_AVERAGE_1000_OR_MORE_IN_50_GAMES_OR_MORE);
        LocalStorageManager::unlockAchievement(ACH_AVERAGE_1000_OR_MORE_IN_50_GAMES_OR_MORE);
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
            _leaderboardID = LEAD_EASY_MODE;
            Utils::submitScore(LEAD_EASY_MODE, longScore);
        }
        else if (level == kGameLevelNormal)
        {
            _leaderboardID = LEAD_NORMAL_MODE;
            Utils::submitScore(LEAD_NORMAL_MODE, longScore);
        }
        else if(level == kGameLevelHard)
        {
            _leaderboardID = LEAD_HARD_MODE;
            Utils::submitScore(LEAD_HARD_MODE, longScore);
        }
        
        LocalStorageManager::setScoreInLevel(score, level);
        scoreInLevel = score;
        
        spBadge->setTexture(CCSprite::create("bicho_0003.png")->getTexture());
    }
    
    _lblMaxScore->setString(CCString::createWithFormat("%lu", scoreInLevel)->getCString());
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
                CCMessageBox("Login", "Game Service");
                NativeUtils::signIn();
            }
            else
                NativeUtils::showLeaderboard(_leaderboardID);

            break;

    }

    
}