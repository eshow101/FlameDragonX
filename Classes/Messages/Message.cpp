//
//  Message.cpp
//  FlameDragonX
//
//  Created by SuiYi on 9/13/16.
//
//

#include "Message.hpp"
#include "MessageLayer.hpp"
#include "CallbackActivity.hpp"
#include "CallbackMethod.hpp"

Message::Message()
{
    _callbackTarget = nullptr;
    _callbackResultMethod = nullptr;
    _returnValue = -1;
    _isBlocking = true;
    
    _messageLayer = nullptr;
    _layer = nullptr;
}

void Message::setReturnFunction(Ref * target, SEL_CALLBACK1 method)
{
    _callbackTarget = target;
    _callbackResultMethod = method;
}

void Message::showDialog(Layer * layer)
{
    _layer = layer;
    
    initDialog();
}

void Message::showDialog(MessageLayer * layer)
{
    _messageLayer = layer;
    
    initDialog();
    
    // Show the animation
    FDActivity * activity = onEnterActivity();
    if (activity != nullptr)
    {
        _messageLayer->getActivityQueue()->appendActivity(activity);
    }
}

void Message::initDialog()
{
    /*
    _sprite = Sprite::create("Others/MessageBox.png");
    _sprite->setPosition(Vec2(240, 160));
    _messageLayer->addChild(_sprite);
    */
}

void Message::removeDialog()
{
    // _messageLayer->removeChild(_sprite);
}

void Message::handleClick(Vec2 location)
{
    _returnValue = 0;
    
    this->closeDialog();
}

void Message::closeDialog()
{
    FDActivity * activity = onExitActivity();
    if (activity != nullptr && _messageLayer != nullptr)
    {
        _messageLayer->getActivityQueue()->appendActivity(activity);
    }

    // Handle Callbacks
    if (_messageLayer != nullptr)
    {
        CallbackMethod * method1 = CallbackMethod::create(_messageLayer, CALLBACK0_SELECTOR(MessageLayer::removeMessage));
        CallbackActivity * callback1 = CallbackActivity::create(method1);
        _messageLayer->getActivityQueue()->appendActivity(callback1);
    
        if (_callbackTarget != nullptr) {
            CallbackMethod * method2 = CallbackMethod::create(_callbackTarget, _callbackResultMethod, _returnValue);
            CallbackActivity * callback2 = CallbackActivity::create(method2);
            _messageLayer->getActivityQueue()->appendActivity(callback2);
        }
    }
    else if (_layer != nullptr)
    {
        this->removeDialog();
        CallbackMethod * method2 = CallbackMethod::create(_callbackTarget, _callbackResultMethod, _returnValue);
        method2->execute();
        
        /*
         
         */
    }
}

void Message::addChildToLayer(Node * node)
{
    if (_messageLayer != nullptr) {
        _messageLayer->addChild(node);
    }
    else if (_layer != nullptr) {
        _layer->addChild(node);
    }
}

void Message::removeChildFromLayer(Node * node)
{
    if (_messageLayer != nullptr) {
        _messageLayer->removeChild(node);
    }
    else if (_layer != nullptr) {
        _layer->removeChild(node);
    }
    
}

FDActivity * Message::onEnterActivity()
{
    return nullptr;
}

FDActivity * Message::onExitActivity()
{
    return nullptr;
}

bool Message::isBlocking()
{
    return _isBlocking;
}

void Message::handleReturnValue(int value)
{
    _returnValue = value;
    this->closeDialog();
}
