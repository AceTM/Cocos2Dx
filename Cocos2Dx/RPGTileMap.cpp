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
    tileMap = CCTMXTiledMap::create("Grass.tmx");
    this->addChild(tileMap, 0, 0);
    
    CCArray *childrenArray = tileMap->getChildren();
    CCSpriteBatchNode *childNode = NULL;
    CCObject *object = NULL;
    CCARRAY_FOREACH(childrenArray, object) {
        childNode = (CCSpriteBatchNode*)object;
        if (!childNode) break;
        childNode->getTexture()->setAntiAliasTexParameters();
    }
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    return true;
}

void TileMap::update(float dt) {
    
}

bool TileMap::ccTouchBegan(CCTouch *touch, CCEvent *event) {
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