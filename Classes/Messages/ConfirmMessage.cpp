//
//  ConfirmMessage.cpp
//  FlameDragonX
//
//  Created by SuiYi on 9/13/16.
//
//

#include "ConfirmMessage.hpp"

ConfirmMessage::ConfirmMessage(std::string content)
: ConversationMessage(content)
{

}

ConfirmMessage::ConfirmMessage(Creature * creature, std::string content)
: ConversationMessage(creature, content)
{
 
}

void ConfirmMessage::handleClick(Vec2 location)
{
    _returnValue = 1;
    
    this->closeDialog();
}
