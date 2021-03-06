//
//  BattleField.cpp
//  FlameDragonX
//
//  Created by SuiYi on 8/23/16.
//
//

#include "BattleField.hpp"
#include "StringUtil.hpp"
#include "TextFileReader.hpp"
#include "AnimationLibrary.hpp"
#include "CreatureMoveActivity.hpp"
#include "BatchActivity.hpp"
#include "BattleScene.hpp"
#include "CursorMoveActivity.hpp"
#include "RemoveObjectActivity.hpp"
#include "DataStore.hpp"

USING_NS_CC;

using namespace cocos2d::ui;

BattleField::BattleField()
{
    
}

BattleField::BattleField(BattleScene * scene)
{
    this->_battleScene = scene;
}

BattleScene * BattleField::getBattleScene()
{
    return _battleScene;
}

BattleField::~BattleField()
{
    _touchHandler->release();
    _stateDispatcher->release();
    
    if (_groundMetrix != nullptr)
    {
        _groundMetrix->clear();
    }
    
    if (_battleObjectList != nullptr)
    {
        _battleObjectList->clear();
        delete _battleObjectList;
    }
    
    if (_friendList != nullptr)
    {
        _friendList->clear();
    }
    if (_enemyList != nullptr)
    {
        _enemyList->clear();
    }
    if (_npcList != nullptr)
    {
        _npcList->clear();
    }
    if (_deadCreatureList != nullptr)
    {
        _deadCreatureList->clear();
    }
    
}

void BattleField::initWithChapter(int chapterId)
{
    auto size = Director::getInstance()->getWinSize();
    auto sizein = Director::getInstance()->getWinSizeInPixels();
    float contentScale = Director::getInstance()->getContentScaleFactor();
    
    DEFAULT_DISPLAY_SCALE = 1.5f * contentScale;
    MIN_DISPLAY_SCALE = 0.6f * contentScale;
    MAX_DISPLAY_SCALE = 3.0f * contentScale;
    
    _displayBlockSize = (float)BLOCK_SIZE / contentScale;
    
    std::string filename = StringUtil::format("Maps/Chapter-%02d.png", chapterId);
    _groundImage = Sprite::create(filename);
    _groundImage->setAnchorPoint(Vec2(0, 0));
    this->addChild(_groundImage, 1);
    
    this->setDisplayScale(DEFAULT_DISPLAY_SCALE);
    
    // Init Objects
    this->_battleObjectList = new Vector<BattleObject *>();
    
    // Init Creatures
    this->_friendList = new Vector<Creature *>();
    this->_enemyList = new Vector<Creature *>();
    this->_npcList = new Vector<Creature *>();
    this->_deadCreatureList = new Vector<Creature *>();
    

    // Init Metrix
    this->initData(chapterId);
    
    if (_hasCoverImage)
    {
        Sprite * coverImage = Sprite::create(StringUtil::format("Maps/Chapter-%02d-Cover.png", chapterId));
        coverImage->setAnchorPoint(Vec2(0, 0));
        _groundImage->addChild(coverImage, BattleObjectOrder_Cover);
    }
    
    this->_cursor = new Cursor();
    addObject(_cursor, Vec2(0, 0));
    
    // Adding controls to map
    _touchHandler = new TouchHandler(this);
    
    // State Dispatcher
    _stateDispatcher = new StateDispatcher(_battleScene);
    
    /*
    Creature * creature = new Creature(CreatureType_Friend);
    creature->initWithDefinition(3, 3);
    creature->creatureData()->attackItemIndex = 0;
    
    this->addCreature(creature, Vec2(2, 24));
    Creature * creature2 = new Creature(CreatureType_Friend);
    creature2->initWithDefinition(2, 2);
    this->addCreature(creature2, Vec2(2, 22));
    Creature * creature3 = new Creature(CreatureType_Friend);
    creature3->initWithDefinition(1, 1);
    this->addCreature(creature3, Vec2(10, 9));
    
    Creature * enemy1 = new Creature(CreatureType_Enemy);
    enemy1->initWithDefinition(101, 50101);
    this->addCreature(enemy1, Vec2(10, 18));
    Creature * enemy2 = new Creature(CreatureType_Enemy);
    enemy2->initWithDefinition(102, 50102);
    this->addCreature(enemy2, Vec2(12, 17));
    
    Creature * enemy3 = new Creature(CreatureType_Enemy);
    enemy3->initWithDefinition(201, 50103);
    this->addCreature(enemy3, Vec2(10, 8));
    
    
    BatchActivity * bacth = new BatchActivity();
    Creature * c3 = this->getCreatureAt(2, 24);
    CreatureMoveActivity * activity = new CreatureMoveActivity(this, c3);
    activity->appendPosition(4, 24);
    activity->appendPosition(4, 20);
    
    Creature * c2 = this->getCreatureAt(2, 22);
    CreatureMoveActivity * activity2 = new CreatureMoveActivity(this, c2);
    activity2->appendPosition(5, 22);
    activity2->appendPosition(5 , 19);
    
    bacth->addActivity(activity);
    bacth->addActivity(activity2);
    
    _battleScene->getActivityQueue()->pushBackActivity(bacth);
    activity2->release();
    
    */
    
}

Vec2 BattleField::getFieldPositionOnScreen()
{
    return _groundImage->getPosition();
}

void BattleField::setFieldPositionOnScreen(float x, float y)
{
    _groundImage->setPosition(Vec2(x, y));
}

Size BattleField::getFieldSizeOnScreen()
{
    return _groundImage->getBoundingBox().size;
}

Size BattleField::getFieldSize()
{
    return Size(_fieldWidth, _fieldHeight);
}

float BattleField::getDisplayScale()
{
    return _displayScale;
}

void BattleField::setDisplayScale(float scale)
{
    if (scale > MAX_DISPLAY_SCALE || scale < MIN_DISPLAY_SCALE) {
        return;
    }
    
    _displayScale = scale;
    _groundImage->setScale(_displayScale);
}

void BattleField::initData(int chapterId)
{
    _groundMetrix = new Vector<Ground *>();
    
    std::string filename = StringUtil::format("Data/Maps/Chapter-%02d.dat", chapterId);
    TextFileReader * reader = new TextFileReader(filename);
    
    _fieldWidth = reader->readInt();
    _fieldHeight = reader->readInt();
    
    for (int j = 1; j <= _fieldHeight; j++) {
        for (int i = 1; i <= _fieldWidth; i++) {
            
            GroundType type = (GroundType)reader->readInt();
            Ground * ground = Ground::createGround(type);
            _groundMetrix->pushBack(ground);
        }
    }
    
    _hasCoverImage = (reader->readInt() == 1);
    if (_hasCoverImage)
    {
        for (int j = 1; j <= _fieldHeight; j++) {
            for (int i = 1; i <= _fieldWidth; i++) {
                
                int value = reader->readInt();
                Ground * ground = this->groundAt(i, j);
                ground->setCover(value == 9);
            }
        }
    }
    
    _treasureCount = reader->readInt();
    for(int i = 0; i < _treasureCount; i++)
    {
        int x = reader->readInt();
        int y = reader->readInt();
        
        int type = reader->readInt();
        int itemId = reader->readInt();
        
        Treasure * treasure = new Treasure(itemId, type);
        this->addObject(treasure, Vec2(x, y));
        treasure->release();
    }
    
    reader->release();
}

Ground * BattleField::groundAt(int x, int y)
{
    if (x <= 0 || y <= 0 || x > _fieldWidth || y > _fieldHeight)
    {
        return nullptr;
    }
    
    return _groundMetrix->at((y - 1) * _fieldWidth + x - 1);
}

Creature * BattleField::getCreatureAt(int x, int y)
{
    if (x <= 0 || y <= 0 || x > _fieldWidth || y > _fieldHeight)
    {
        return nullptr;
    }
    
    for(Creature * creature : *_friendList)
    {
        Vec2 position = convertLocationToPosition(creature->getSprite()->getPosition());
        if (position.x == x && position.y == y)
        {
            return creature;
        }
    }
    for(Creature * creature : *_npcList)
    {
        Vec2 position = convertLocationToPosition(creature->getSprite()->getPosition());
        if (position.x == x && position.y == y)
        {
            return creature;
        }
    }
    for(Creature * creature : *_enemyList)
    {
        Vec2 position = convertLocationToPosition(creature->getSprite()->getPosition());
        if (position.x == x && position.y == y)
        {
            return creature;
        }
    }
    
    return nullptr;
}

Creature * BattleField::getCreatureById(int creatureId)
{
    for (Creature * c : *_friendList) {
        if (c->getId() == creatureId) {
            return c;
        }
    }
    for (Creature * c : *_npcList) {
        if (c->getId() == creatureId) {
            return c;
        }
    }
    for (Creature * c : *_enemyList) {
        if (c->getId() == creatureId) {
            return c;
        }
    }
    
    return nullptr;
}

Creature * BattleField::getDeadCreatureById(int creatureId)
{
    for (Creature * c : *_deadCreatureList)
    {
        if (c->getId() == creatureId)
            return c;
    }
    
    return nullptr;
}

Vector<Creature *> * BattleField::getFriendList()
{
    return _friendList;
}

Vector<Creature *> * BattleField::getEnemyList()
{
    return _enemyList;
}

Vector<Creature *> * BattleField::getNPCList()
{
    return _npcList;
}

Vector<Creature *> * BattleField::getDeadCreatureList()
{
    return _deadCreatureList;
}

Vec2 BattleField::getObjectPosition(BattleObject * obj)
{
    if (obj == nullptr)
    {
        return Vec2 (-1, -1);
    }
    
    Vec2 location = obj->getSprite()->getPosition();
    return convertLocationToPosition(location);
}

void BattleField::setObjectPosition(BattleObject * obj, Vec2 position)
{
    if (obj == nullptr)
    {
        return;
    }

    obj->getSprite()->setPosition(convertPositionToLocation(position));
}

bool BattleField::isInteractiveBusy()
{
    return _battleScene->getActivityQueue()->isBusy();
}

void BattleField::notifyStateDispatcher()
{
    _stateDispatcher->onNotified();
}

Vec2 BattleField::convertPositionToLocation(Vec2 pos)
{
    float locX = (pos.x - 1) * _displayBlockSize + _displayBlockSize / 2;
    float locY = (_fieldHeight - pos.y) * _displayBlockSize + _displayBlockSize / 2;
    
    return Vec2(locX, locY);
}

Vec2 BattleField::convertLocationToPosition(Vec2 loc)
{
    int posX = int(loc.x / _displayBlockSize) + 1;
    int posY = _fieldHeight - int(loc.y / _displayBlockSize);
    /// int posY = _fieldHeight - int(loc.y / _displayBlockSize) + 1;
    
    /// int posX = int(loc.x / BLOCK_SIZE) + 1;
    /// int posY = _fieldHeight - int(loc.y / BLOCK_SIZE);
    
    return Vec2(posX, posY);
}

Vec2 BattleField::getScreenLocationByLocation(Vec2 loc)
{
    float locX = loc.x * _displayScale + _groundImage->getPosition().x;
    float locY = loc.y * _displayScale + _groundImage->getPosition().y;
    
    return Vec2(locX, locY);
}

void BattleField::sendObjectToGround(BattleObject * obj, Vec2 position)
{
    int zOrder;
    Creature * creature = (Creature *)obj;
    Ground * ground = groundAt(position.x, position.y);
    
    switch (obj->getObjectType()) {
        case BattleObject_ScopeIndicator:
        case BattleObject_Cursor:
            zOrder = (ground != nullptr && ground->hasCover()) ?  BattleObjectOrder_HigherIndicator : BattleObjectOrder_Indicator;
            break;
        case BattleObject_Creature:
            zOrder = (creature != nullptr && creature->canFly()) ? BattleObjectOrder_FlyCreature : BattleObjectOrder_GroundCreature;
            break;
        case BattleObject_Menu:
            zOrder = BattleObjectOrder_Menu;
            break;
        case BattleObject_Treasure:
            zOrder = BattleObjectOrder_OnGround;
            break;
            
        default:
            break;
    }
    
    
    Vec2 location = this->convertPositionToLocation(position);
    obj->getSprite()->setPosition(location);
    _groundImage->addChild(obj->getSprite(), zOrder);
}

void BattleField::addObject(BattleObject * obj, Vec2 position)
{
    sendObjectToGround(obj, position);
    _battleObjectList->pushBack(obj);
}

void BattleField::addCreature(Creature * creature, Vec2 position)
{
    sendObjectToGround(creature, position);
    
    switch(creature->getType())
    {
        case CreatureType_Friend:
            this->_friendList->pushBack(creature);
            break;
        case CreatureType_Enemy:
            this->_enemyList->pushBack(creature);
            break;
        case CreatureType_Npc:
            this->_npcList->pushBack(creature);
            break;
        default:
            break;
    }
}

void BattleField::removeObject(BattleObject * obj)
{
    if (obj->getObjectType() == BattleObject_Creature)
    {
        Creature * creature = (Creature *)obj;
        switch(creature->getType())
        {
            case CreatureType_Friend:
                this->_friendList->eraseObject(creature);
                break;
            case CreatureType_Enemy:
                this->_enemyList->eraseObject(creature);
                break;
            case CreatureType_Npc:
                this->_npcList->eraseObject(creature);
                break;
            default:
                break;
        }
    }
    else
    {
        this->_battleObjectList->eraseObject(obj);
    }
    
    _groundImage->removeChild(obj->getSprite(), false);
    
}

Treasure * BattleField::getTreasureAt(Vec2 position)
{
    return (Treasure *)this->getObjectByPosition(BattleObject_Treasure, position);
}

void BattleField::takeTick(int synchronizedTick)
{
    for(Vector<BattleObject *>::iterator it = _battleObjectList->begin(); it != _battleObjectList->end(); it++)
    {
        BattleObject * obj = *it;
        obj->takeTick(synchronizedTick);
    }
    
    for(Vector<Creature *>::iterator it = _friendList->begin(); it != _friendList->end(); it++)
    {
        Creature * creature = *it;
        creature->takeTick(synchronizedTick);
    }
    
    for(Vector<Creature *>::iterator it = _enemyList->begin(); it != _enemyList->end(); it++)
    {
        Creature * creature = *it;
        creature->takeTick(synchronizedTick);
    }

    for(Vector<Creature *>::iterator it = _npcList->begin(); it != _npcList->end(); it++)
    {
        Creature * creature = *it;
        creature->takeTick(synchronizedTick);
    }
}

void BattleField::onClickedAt(Vec2 location)
{
    if (isInteractiveBusy())
    {
        return;
    }
    
    Vec2 position = convertLocationToPosition(location);
    if (position.x < 0 || position.y < 0 || position.x > _fieldWidth + 1 || position.y > _fieldHeight + 1)
    {
        return;
    }
    
    _stateDispatcher->handleClickAt(position);
    
}

void BattleField::setCursorTo(Vec2 position)
{
    Vec2 unitLocation = convertPositionToLocation(position);
    _cursor->getSprite()->setPosition(unitLocation);
    
    Ground * ground = groundAt(position.x, position.y);
    if (ground->hasCover())
    {
        _cursor->setZOrder(BattleObjectOrder_HigherIndicator);
    }
    else
    {
        _cursor->setZOrder(BattleObjectOrder_Indicator);
    }
    
    // Make sure the cursor is in the screen
    Vec2 cursorScreenLoc = getScreenLocationByLocation(unitLocation);
    float minX = 2 * Constants::UNIT_ICON_SIZE;
    float minY = 2 * Constants::UNIT_ICON_SIZE;
    float maxX = Constants::getScreenSize().width - minX;
    float maxY = Constants::getScreenSize().height - minY;
    
    float dx = 0;
    if (cursorScreenLoc.x < minX) {
        dx = minX - cursorScreenLoc.x;
    }
    if (cursorScreenLoc.x > maxX) {
        dx = maxX - cursorScreenLoc.x;
    }
    
    float dy = 0;
    if (cursorScreenLoc.y < minY) {
        dy = minY - cursorScreenLoc.y;
    }
    if (cursorScreenLoc.y > maxY) {
        dy = maxY - cursorScreenLoc.y;
    }
    
    if (dx != 0 || dy != 0) {
        log("Set Cursor, dx=%f, dy=%f", dx, dy);
    }
    
    Vec2 originLoc = _groundImage->getPosition();
    _groundImage->setPosition(originLoc.x + dx, originLoc.y + dy);
    
}

void BattleField::setCursorObjectTo(Ref * point)
{
    setCursorTo(((FDPoint *)point)->getValue());
}

void BattleField::moveCursorTo(Vec2 position)
{
    CursorMoveActivity * activity = CursorMoveActivity::create(this, _cursor, position);
    _battleScene->getActivityQueue()->insertSingleActivity(activity);
}

Vec2 BattleField::getCursorPosition()
{
    return getObjectPosition(_cursor);
}

void BattleField::showSystemMenuAt(int menuId, Vec2 position)
{
    showCreatureMenuAt(menuId, position, nullptr);
}

void BattleField::showCreatureMenuAt(int menuId, Vec2 position, Creature * creature)
{
    int menuItemIds[4] = { menuId * 10 + 3, menuId * 10 + 2, menuId * 10 + 1, menuId * 10};
    
    BatchActivity * batch = new BatchActivity();
    
    for (int menuItemId : menuItemIds)
    {
        MenuCursor * menu = new MenuCursor(menuItemId, this, position);
        this->addObject(menu, position);
        menu->checkValidation(creature);
        if (menu->isValid()) {
            this->setActiveMenuCursor(menu);
        }
        
        batch->addActivity(menu->onOpenActivity());
        menu->release();
    }
    
    _battleScene->getActivityQueue()->appendActivity(batch);
    batch->release();
}

void BattleField::closeMenu(bool withAnimation)
{
    if (withAnimation) {
        BatchActivity * batch = new BatchActivity();
    
        for (BattleObject * object : *_battleObjectList) {
            if (object->getObjectType() == BattleObject_Menu && !object->isRemoving())
            {
                RemoveObjectActivity * removing = RemoveObjectActivity::create(this, object);
                batch->addActivity(removing);
            }
        }
        
        _battleScene->getActivityQueue()->insertActivity(batch);
        batch->release();
    }
    else
    {
        for (long i = _battleObjectList->size() - 1; i >= 0; i--) {
            
            BattleObject * obj = _battleObjectList->at(i);
            if (obj->getObjectType() == BattleObject_Menu)
            {
                this->removeObject(obj);
            }
        }
    }
}

void BattleField::setActiveMenuCursor(MenuCursor * selected)
{
    for (BattleObject * object : *_battleObjectList) {
        if (object->getObjectType() == BattleObject_Menu)
        {
            MenuCursor * cursor = (MenuCursor *)object;
            cursor->setSelected(false);
        }
    }
    
    selected->setSelected(true);
}

BattleObject * BattleField::getObjectByPosition(BattleObjectType type, Vec2 position)
{
    for (BattleObject * object : *_battleObjectList) {
        if (object->getObjectType() == type && getObjectPosition(object) == position)
        {
            return object;
        }
    }
    
    return nullptr;
}

bool BattleField::isPositionInScope(Vec2 position)
{
    for (BattleObject * object : *_battleObjectList) {
        if (object->getObjectType() == BattleObject_ScopeIndicator && getObjectPosition(object) == position)
        {
            return true;
        }
    }
    
    return false;
}

void BattleField::removeAllIndicators()
{
    for (int i = (int)_battleObjectList->size() - 1; i >= 0 ; i--) {
        BattleObject * obj = _battleObjectList->at(i);
        if (obj->getObjectType() == BattleObject_ScopeIndicator)
        {
            removeObject(obj);
        }
    }
}

int BattleField::getPositionDistance(Vec2 position1, Vec2 position2)
{
    return abs(position1.x - position2.x) + abs(position1.y - position2.y);
}

int BattleField::getObjectDistance(BattleObject * c1, BattleObject * c2)
{
    Vec2 position1 = this->getObjectPosition(c1);
    Vec2 position2 = this->getObjectPosition(c2);
    
    return getPositionDistance(position1, position2);
}

bool BattleField::hasAdjacentFriend(Creature * creature)
{
    Vec2 position = this->getObjectPosition(creature);
    
    Creature * adjacent;
    adjacent = this->getCreatureAt(position.x + 1, position.y);
    if (adjacent != nullptr && adjacent->getType() == CreatureType_Friend) {
        return true;
    }
    adjacent = this->getCreatureAt(position.x - 1, position.y);
    if (adjacent != nullptr && adjacent->getType() == CreatureType_Friend) {
        return true;
    }
    adjacent = this->getCreatureAt(position.x, position.y + 1);
    if (adjacent != nullptr && adjacent->getType() == CreatureType_Friend) {
        return true;
    }
    adjacent = this->getCreatureAt(position.x, position.y - 1);
    if (adjacent != nullptr && adjacent->getType() == CreatureType_Friend) {
        return true;
    }
    return false;
}

Vector<Creature *> BattleField::searchTargetInAttackRange(Creature * creature)
{
    Vector<Creature *> result;
    
    if (creature == nullptr)
        return result;
    
    FDRange * attackRange = creature->getAttackRange();
    if (attackRange == nullptr)
        return result;

    if (creature->getType() == CreatureType_Enemy)
    {
        for (Creature * target : *_friendList) {
            int distance = getObjectDistance(creature, target);
            if (attackRange->containsValue(distance))
                result.pushBack(target);
        }
        for (Creature * target : *_npcList) {
            int distance = getObjectDistance(creature, target);
            if (attackRange->containsValue(distance))
                result.pushBack(target);
        }
    }
    else
    {
        for (Creature * target : *_enemyList) {
            int distance = getObjectDistance(creature, target);
            if (attackRange->containsValue(distance))
                result.pushBack(target);
        }
    }
    
    return result;
}

Vector<Creature *> BattleField::searchTargetInMagicRange(Creature * creature, int magicId)
{
    Vector<Creature *> result;
    
    if (creature == nullptr)
        return result;
    
    MagicDefinition * magic = DataStore::getInstance()->getMagicDefinition(magicId);
    if (magic == nullptr) {
        return result;
    }
    
    int scope = magic->effectScope();
    bool goodCreature = (creature->getType() == CreatureType_Friend || creature->getType() == CreatureType_Npc);
    bool goodMagic = (magic->getType() == MagicType_Recover || magic->getType() == MagicType_Defensive);
    bool goodGuy = ((goodCreature && goodMagic) || (!goodCreature && !goodMagic));
    
    if (goodGuy)
    {
        for (Creature * target : *_friendList) {
            int distance = getObjectDistance(creature, target);
            if (distance <= scope)
                result.pushBack(target);
        }
        for (Creature * target : *_npcList) {
            int distance = getObjectDistance(creature, target);
            if (distance <= scope)
                result.pushBack(target);
        }
    }
    else
    {
        for (Creature * target : *_enemyList) {
            int distance = getObjectDistance(creature, target);
            if (distance <= scope)
                result.pushBack(target);
        }
    }
    
    return result;
}

Vector<Creature *> BattleField::getCreaturesInRange(Vec2 position, int range, bool badGuy)
{
    Vector<Creature *> result;
    if (!badGuy)
    {
        for (Creature * target : *_friendList) {
            int distance = getPositionDistance(position, this->getObjectPosition(target));
            if (distance <= range)
                result.pushBack(target);
        }
        for (Creature * target : *_npcList) {
            int distance = getPositionDistance(position, this->getObjectPosition(target));
            if (distance <= range)
                result.pushBack(target);
        }
    }
    else
    {
        for (Creature * target : *_enemyList) {
            int distance = getPositionDistance(position, this->getObjectPosition(target));
            if (distance <= range)
                result.pushBack(target);
        }
    }
    
    return result;
}

Vector<Creature *> BattleField::getHostileCreatureList(CreatureType type)
{
    Vector<Creature *> creatureList;
    if (type == CreatureType_Enemy) {
        creatureList.pushBack(*(this->getFriendList()));
        creatureList.pushBack(*(this->getNPCList()));
    }
    else
    {
        creatureList.pushBack(*(this->getEnemyList()));
    }
    
    return creatureList;
}

Vector<Creature *> BattleField::getAmicableCreatureList(CreatureType type)
{
    Vector<Creature *> creatureList;
    if (type != CreatureType_Enemy) {
        creatureList.pushBack(*(this->getFriendList()));
        creatureList.pushBack(*(this->getNPCList()));
    }
    else
    {
        creatureList.pushBack(*(this->getEnemyList()));
    }
    
    return creatureList;
}
