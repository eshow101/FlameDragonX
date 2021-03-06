//
//  Ground.hpp
//  FlameDragonX
//
//  Created by SuiYi on 8/24/16.
//
//

#ifndef Ground_hpp
#define Ground_hpp

#include "cocos2d.h"

USING_NS_CC;

typedef enum GroundType
{
    GroundTypeGround = 0,
    GroundTypeChasm = 1,
    GroundTypeMarsh = 3,
    GroundTypeGap = 4,
    GroundTypeBlackForest = 5,
    GroundTypeForest = 6
} GroundType;


class Ground : public cocos2d::Ref
{
private:
    GroundType _groundType;
    int _attackPoint;
    int _defendPoint;
    bool _hasCover;
    
public:
    
    static Ground * createGround(GroundType type);
    Ground(GroundType type, int aPoint, int dPoint);
    
    void setCover(bool cover);
    
    GroundType getType();
    int getAttackPoint();
    int getDefendPoint();
    bool hasCover();
    
};

#endif /* Ground_hpp */
