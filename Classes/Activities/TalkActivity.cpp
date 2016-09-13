//
//  TalkActivity.cpp
//  FlameDragonX
//
//  Created by SuiYi on 9/2/16.
//
//

#include "TalkActivity.hpp"
#include "TalkMessage.hpp"
#include "BattleScene.hpp"

TalkActivity * TalkActivity::create(BattleScene * scene, Creature * creature, std::string message)
{
    TalkActivity * activity = new TalkActivity(scene, creature, message);
    activity->autorelease();
    
    return activity;
}

TalkActivity::TalkActivity(BattleScene * scene, Creature * creature, std::string message)
: GlobalActivity(scene)
{
    this->_creature = creature;
    this->_message = message;
}

void TalkActivity::initialize()
{
    log("TalkActivity initialized.");
    
    TalkMessage * message = new TalkMessage();
    _battleScene->showMessage(message);
    message->release();
    
}

void TalkActivity::internalTick(int synchronizedTick)
{
    _hasFinished = true;
}
