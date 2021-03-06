//
//  ActionMenuState.hpp
//  FlameDragonX
//
//  Created by SuiYi on 9/8/16.
//
//

#ifndef ActionMenuState_hpp
#define ActionMenuState_hpp

#include "cocos2d.h"
#include "ActionState.hpp"
#include "Creature.hpp"
#include "Treasure.hpp"

class ActionMenuState : public ActionState
{
private:
    
    Creature * _creature;
    MagicDefinition * _selectedMagic;
    Treasure * _treasure;
    
public:
    
    static ActionMenuState * create(BattleScene * scene, StateSession * session);
    
    void onEnterState() override;
    void onExitState() override;
    
    void handleClickAt(Vec2 position) override;
    
    void selectMagic();
    void confirmSelectMagic(int result);
    
    void moveCursorToAttackTarget();
    void moveCursorToMagicTarget();
    
    void checkTreatureAndWaiveTurn();
    void confirmPickTreasure(int confirmed);
    void confirmExchangeTreasure(int confirmed);
    void confirmItemToPutBack(int index);
    void waiveTurn();
    
    
};

#endif /* ActionMenuState_hpp */
