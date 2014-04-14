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
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint visibleOrigin = CCDirector::sharedDirector()->getVisibleOrigin();
    CCPoint origin = ccp(visibleOrigin.x + visibleSize.width * 0.5f, visibleOrigin.y + visibleSize.height* 0.5f);
    
    // BGWhite
    CCSprite* bg = CCSprite::create("bg_white.png");
    bg->setPosition(origin);
    addChild(bg);
    
    float w = bg->getContentSize().width;
    float h = bg->getContentSize().height;
    CCPoint o = ccp(w * 0.5f, h * 0.5f);
    
    // Head title
    _lblHeadTitle = CCLabelTTF::create("Finish!", "Arial", 16.0f, CCSizeMake(190, 20), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
    _lblHeadTitle->setPosition(ccp(o.x, o.y + h * 0.35f));
    _lblHeadTitle->setColor(ccc3(0, 0, 0));
    bg->addChild(_lblHeadTitle);
    
    // Badge and record
    CCSprite* spBadge = CCSprite::create("badge.png");
    spBadge->setPosition(ccp(o.x - spBadge->getContentSize().width * 1.2f, o.y + h * 0.05f));
    bg->addChild(spBadge, 10);
    
    _lblScore = CCLabelTTF::create("0", "Arial", 16.0f, CCSizeMake(130, 20), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    _lblScore->setColor(ccc3(0, 0, 0));
    _lblScore->setAnchorPoint(ccp(0, 0));
    _lblScore->setPosition(ccp(o.x, o.y));
    bg->addChild(_lblScore);
    
    _lblMaxScore = CCLabelTTF::create("Best: 0", "Arial", 16.0f, CCSizeMake(200, 20), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    _lblMaxScore->setColor(ccc3(0, 0, 0));
    _lblMaxScore->setAnchorPoint(ccp(0, 0));
    _lblMaxScore->setPosition(ccp(o.x - w * 0.3f, o.y + h * 0.2f));
    bg->addChild(_lblMaxScore);
    
    _lblTotalScore = CCLabelTTF::create("Total Score: 0", "Arial", 16.0f, CCSizeMake(200, 20), kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    _lblTotalScore->setColor(ccc3(0, 0, 0));
    _lblTotalScore->setAnchorPoint(ccp(0, 0));
    _lblTotalScore->setPosition(ccp(o.x - w * 0.3f, o.y - h * 0.15f));
    bg->addChild(_lblTotalScore);
    
    // Send score button
    CCMenuItemImage* itemScore = CCMenuItemImage::create("play_game.png", "play_game.png", this, menu_selector(PopUpLoseLayer::_onOptionPressed));
    itemScore->setTag(kTagSendScore);
    itemScore->setAnchorPoint(ccp(0, 0));
    itemScore->setPosition(ccp(o.x - w * 0.35f, o.y - h * 0.35f));
    
    // Home button
    CCMenuItemImage* itemHome = CCMenuItemImage::create("home.png", "home.png", this, menu_selector(PopUpLoseLayer::_onOptionPressed));
    itemHome->setTag(kTagGoHome);
    itemHome->setAnchorPoint(ccp(0, 0));
    itemHome->setPosition(ccp(itemScore->getPositionX() + itemScore->getContentSize().width * 1.6f, itemScore->getPositionY()));
    
    // Play again button
    CCMenuItemImage* itemPlayAgain = CCMenuItemImage::create("play_again.png", "play_again.png", this, menu_selector(PopUpLoseLayer::_onOptionPressed));
    itemPlayAgain->setTag(kTagPlayAgain);
    itemPlayAgain->setAnchorPoint(ccp(0, 0));
    itemPlayAgain->setPosition(ccp(itemHome->getPositionX() + itemScore->getContentSize().width * 1.6f, itemScore->getPositionY()));
    
    // Menu
    CCMenu* menu = CCMenu::create();
    menu->setAnchorPoint(ccp(0, 0));
    menu->setPosition(CCPointZero);
    
    menu->addChild(itemScore);
    menu->addChild(itemHome);
    menu->addChild(itemPlayAgain);
    
    bg->addChild(menu);
    
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
    }
    
    _lblMaxScore->setString(CCString::createWithFormat("Best: %lu", scoreInLevel)->getCString());
    
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