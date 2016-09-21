//
//  Constants.hpp
//  FlameDragonX
//
//  Created by SuiYi on 8/29/16.
//
//

#ifndef Constants_hpp
#define Constants_hpp

#include <stdio.h>
#include "cocos2d.h"

typedef unsigned char FDBYTE;

typedef enum Direction
{
    DirectionNone = 0,
    DirectionUp = 1,
    DirectionDown = 2,
    DirectionLeft = 3,
    DirectionRight = 4
} Direction;


class Constants : public cocos2d::Ref
{
public:
    
    static const int GAME_FPS = 50;
    
    static const int TickPerFrame_IdleAnimation = 15;
    static const int TickPerFrame_MoveAnimation = 5;
    
    constexpr static const float CREATURE_MOVE_SPEED = 0.9f;
    constexpr static const float CURSOR_MOVE_SPEED = 2.5f;
    constexpr static const float MENU_MOVE_SPEED = 3.0f;
    
    static const int SCREEN_WIDTH = 480;
    static const int SCREEN_HEIGHT = 320;
    
    static const int LEVEL_COUNT = 30;
    static const int UNIT_ICON_SIZE = 24;
    
    static const int DEFAULT_SPRITE_SCALE = 1.5f;
    
};


#endif /* Constants_hpp */
