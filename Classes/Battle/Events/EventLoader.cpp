//
//  EventLoader.cpp
//  FlameDragonX
//
//  Created by SuiYi on 9/2/16.
//
//

#include "EventLoader.hpp"
#include "EventHandler.hpp"
#include "TurnCondition.hpp"
#include "LocalizedStrings.hpp"
#include "TalkActivity.hpp"
#include "BattleScene.hpp"
#include "BattleField.hpp"
#include "CreatureDeadCondition.hpp"
#include "CreatureDyingCondition.hpp"
#include "TeamEliminatedCondition.hpp"

EventLoader::EventLoader()
{
    
}

EventLoader::~EventLoader()
{
    
}

void EventLoader::loadEvents()
{
    
}

void EventLoader::initWithScene(BattleScene * scene, EventHandler * handler)
{
    this->_battleScene = scene;
    this->_battleField = scene->getBattleField();
    this->_eventHandler = handler;
    
    _generatedEventId = 0;
}

int EventLoader::loadSingleEvent(EventCondition * condition, CallbackMethod * method)
{
    FDEvent * event = new FDEvent(++_generatedEventId);
    event->initWithCondition(condition, method);
    
    _eventHandler->addEvent(event);
    
    event->autorelease();
    return event->getEventId();
}

int EventLoader::loadTurnEvent(int turnNumber, CreatureType type, SEL_CALLBACK0 function)
{
    TurnCondition * cond = new TurnCondition(turnNumber, type);
    CallbackMethod * method = new CallbackMethod(this, function);
    
    int eventId = this->loadSingleEvent(cond, method);
    
    cond->release();
    method->release();
    
    return eventId;
}

int EventLoader::loadDieEvent(int creatureId, SEL_CALLBACK0 function)
{
    CreatureDeadCondition * cond = new CreatureDeadCondition(creatureId);
    CallbackMethod * method = new CallbackMethod(this, function);
    
    int eventId = this->loadSingleEvent(cond, method);
    
    cond->release();
    method->release();
    
    return eventId;
}

int EventLoader::loadDyingEvent(int creatureId, SEL_CALLBACK0 function)
{
    CreatureDyingCondition * cond = new CreatureDyingCondition(creatureId);
    CallbackMethod * method = new CallbackMethod(this, function);
    
    int eventId = this->loadSingleEvent(cond, method);
    
    cond->release();
    method->release();
    
    return eventId;
}

int EventLoader::loadTeamEvent(CreatureType type, SEL_CALLBACK0 function)
{
    TeamEliminatedCondition * cond = new TeamEliminatedCondition(type);
    CallbackMethod * method = new CallbackMethod(this, function);
    
    int eventId = this->loadSingleEvent(cond, method);
    
    cond->release();
    method->release();
    
    return eventId;
}

void EventLoader::gameOver()
{
    _battleScene->appendMethodToActivity(CALLBACK0_SELECTOR(BattleScene::gameOver));
}

void EventLoader::showTalkMessage(int chapterId, int conversationId, int fromId, int toId)
{
    for (int i = fromId; i <= toId; i++) {
        showTalkMessage(chapterId, conversationId, i);
    }
}

void EventLoader::showTalkMessage(int chapterId, int conversationId, int sequenceId)
{
    int creatureId = LocalizedStrings::getInstance()->getConversationCreatureId(chapterId, conversationId, sequenceId);
    std::string message =LocalizedStrings::getInstance()->getConversation(chapterId, conversationId, sequenceId);
    
    Creature * creature = _battleField->getCreatureById(creatureId);
    
    TalkActivity * talk = TalkActivity::create(TalkActivityType_Speak, _battleScene, creature, message);
    _battleScene->getActivityQueue()->appendActivity(talk);
    
}

void EventLoader::appendActivityMethod(SEL_CALLBACK0 function)
{
    CallbackActivity * activity = CallbackActivity::create(CallbackMethod::create(this, function));
    _battleScene->getActivityQueue()->appendActivity(activity);
}


void EventLoader::BattleField_RemoveObject(Ref * obj)
{
    BattleObject * battleObj = (BattleObject *)obj;
    if (battleObj != nullptr)
    {
        _battleField->removeObject(battleObj);
    }
}