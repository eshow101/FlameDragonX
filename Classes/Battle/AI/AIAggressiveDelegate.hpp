//
//  AIAggressiveDelegate.hpp
//  FlameDragonX
//
//  Created by SuiYi on 10/17/16.
//
//

#ifndef AIAggressiveDelegate_hpp
#define AIAggressiveDelegate_hpp

#include "cocos2d.h"
#include "AIDelegate.hpp"
#include "AICreature.hpp"

class AIAggressiveDelegate : public AIDelegate
{
private:
    
    void searchAttackTarget();
    
public:
    
    AIAggressiveDelegate(AICreature * creature, BattleScene * scene);
    
    void takeAction() override;
};

#endif /* AIAggressiveDelegate_hpp */
