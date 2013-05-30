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
    cocos2d::CCScene *scene = CCScene::create();
    TileMap *layer = TileMap::create();
    scene->addChild(layer);
    return scene;
}

bool TileMap::init() {
    if (!CCLayer::init())return false;
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
    joystickBase->setPosition(ccp(winSize.width * 0.8, winSize.height * 0.15));
    leftJoystick = joystickBase->getJoystick();
    this->addChild(joystickBase, 1, 2);
    
    CCRect buttonRect;
    buttonBase = SneakyButtonSkinnedBase::create();
    button = new SneakyButton;
    button->initWithRect(buttonRect);
    buttonBase->setDefaultSprite(CCSprite::create("ButtonRedNormal.png"));
    buttonBase->setActivatedSprite(CCSprite::create("ButtonRedPress.png"));
    buttonBase->setPressSprite(CCSprite::create("ButtonRedPress.png"));
    buttonBase->setButton(button);
    buttonBase->setPosition(ccp(winSize.width * 0.1, winSize.height * 0.1));
    accelButton = buttonBase->getButton();
    this->addChild(buttonBase, 1, 1);
    
    wallSprite = CCSprite::create("Wallpaper.png");
    wallSprite->setPosition(ccp(160, 240));
    
    player = CCSprite::create("Icon.png");
    player->setPosition(ccp(winSize.height/2, winSize.width/2));
    this->addChild(wallSprite, 0, 0);
    this->addChild(player, 2, 3);
    this->scheduleUpdate();
    
    
    return true;
}

void TileMap::update(float dt) {
    CCPoint velo = leftJoystick->getVelocity();
    if (velo.x != 0 || velo.y != 0) {
        player->setPosition(ccp(player->getPositionX() + (velo.x) , player->getPositionY() + (velo.y)));
    }
    if (accelButton->getIsActive() == true) {
//        this->getCamera()->setCenterXYZ(0, 45, 90);
//        this->getCamera()->setEyeXYZ(0, 0, 0);
//        this->getCamera()->setUpXYZ(0, 1, 0);
        CCLog("ZEye: %f", this->getCamera()->getZEye());
        this->getCamera()->restore();
       
    }
    
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