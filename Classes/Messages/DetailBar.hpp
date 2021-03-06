//
//  DetailBar.hpp
//  FlameDragonX
//
//  Created by SuiYi on 9/13/16.
//
//

#ifndef DetailBar_hpp
#define DetailBar_hpp


#include "cocos2d.h"
#include "Creature.hpp"
#include "ScaledSprite.hpp"

USING_NS_CC;

class DetailBar : public cocos2d::Ref
{
private:
    
    ScaledSprite * _baseSprite;
    
    Creature * _creature;
public:
    
    DetailBar(Creature * creature);
    
    ScaledSprite * getSprite();
};

#endif /* DetailBar_hpp */
