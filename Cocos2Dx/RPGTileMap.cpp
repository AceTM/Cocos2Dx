//
//  RPGTileMap.cpp
//  Cocos2Dx
//
//  Created by MinhNT on 13/05/28.
//
//

#include "RPGTileMap.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "SneakyJoystick.h"
#include "SneakyButton.h"
#include "SneakyJoystickSkinnedBase.h"
#include "SneakyButtonSkinnedBase.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* TileMap::scene() {
    // 'scene' is an autorelease object
    cocos2d::CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    TileMap *layer = TileMap::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool TileMap::init() {
//    tileMap = CCTMXTiledMap::create("Grass.tmx");
//    this->addChild(tileMap, 0, 0);
//    
//    CCArray *childrenArray = tileMap->getChildren();
//    CCSpriteBatchNode *childNode = NULL;
//    CCObject *object = NULL;
//    CCARRAY_FOREACH(childrenArray, object) {
//        childNode = (CCSpriteBatchNode*)object;
//        if (!childNode) break;
//        childNode->getTexture()->setAntiAliasTexParameters();
//    }
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    CCRect joystickRect;
    joystickBase = SneakyJoystickSkinnedBase::create();
    joystick = new SneakyJoystick;
    joystick->initWithRect(joystickRect);
    joystickBase->setBackgroundSprite(CCSprite::create("JoystickRing.png"));
    joystickBase->setThumbSprite(CCSprite::create("JoystickThumb.png"));
    joystickBase->setJoystick(joystick);
    joystickBase->setPosition(ccp(winSize.width * 0.7, winSize.height * 0.3));
    leftJoystick = joystickBase->getJoystick();
    this->addChild(joystickBase);
    
    CCRect buttonRect;
    buttonBase = SneakyButtonSkinnedBase::create();
    button = new SneakyButton;
    button->initWithRect(buttonRect);
    buttonBase->setDefaultSprite(CCSprite::create("ButtonRedNormal.png"));
    buttonBase->setActivatedSprite(CCSprite::create("ButtonRedPress.png"));
    buttonBase->setPressSprite(CCSprite::create("ButtonRedPress.png"));
    buttonBase->setButton(button);
    buttonBase->setPosition(ccp(winSize.width * 0.3, winSize.height * 0.3));
    accelButton = buttonBase->getButton();
    this->addChild(buttonBase);
    
    return true;
    
    CCSprite *wallSprite = CCSprite::create("Wallpaper.png");
    wallSprite->setPosition(ccp(0, 0));
    wallSprite->setScale(0.5);
    this->addChild(wallSprite, 0, 0);
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    return true;
}

void TileMap::tick(float delta) {

}

void TileMap::update(float dt) {
    
}

bool TileMap::ccTouchBegan(CCTouch *touch, CCEvent *event) {
    CCPoint touchPos = touch->getStartLocation();
    CCPoint touchLoc = this->getParent()->convertTouchToNodeSpace(touch);
    CCLog("Touch position: x%f, y%f;", touchPos.x, touchPos.y);
    CCLog("Position after convertion: x%f, y%f;", touchLoc.x, touchLoc.y);
    return true;
}

void TileMap::ccTouchEnded(CCTouch *touch, CCEvent *event) {
//    CCPoint touchLoc = this->getParent()->convertTouchToNodeSpace(touch);
//    CCTMXLayer *layerMap = tileMap->layerNamed("Tile Layer 1");
//    CCSprite *tileMap = layerMap->tileAt(touchLoc);
//    unsigned int gidPoint = layerMap->tileGIDAt(touchLoc);
//    CCLog("Tile: x%f, y%f; GID: %d;", tileMap->getPositionX(), tileMap->getPositionY(), gidPoint);
}

void TileMap::menuCloseCallback(CCObject* pSender) {
    CCDirector::sharedDirector()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}