//
//  Box2DTrial.cpp
//  Cocos2Dx
//
//  Created by MinhNT on 13/05/24.
//
//

#include "Box2DTrial.h"
#include "Box2D.h"
#include "Menu.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* Box2DTrial::scene() {
    cocos2d::CCScene *scene = CCScene::create();
    Menu *layer = Menu::create();
    scene->addChild(layer);
    return scene;
}

bool Box2DTrial::init() {
    if (!CCLayer::init()) return false;
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    b2Vec2 gravity;
    gravity.Set(1, 1);
    
    bool worldSleep = true;
    world = new b2World(gravity, worldSleep);
    
    CCSprite *paddleSprite = CCSprite::create();
    paddleSprite->setPosition(ccp(winSize.width/2, 50));
    paddleSprite->setTag(1);
    this->addChild(paddleSprite, 1);
    
    b2BodyDef paddleBodyDef;
    paddleBodyDef.type = b2_dynamicBody;
    paddleBodyDef.position.Set(winSize.width/2, 50);
    paddleBodyDef.userData = paddleSprite;
    paddleBody = world->CreateBody(&paddleBodyDef);

    return true;
}

void Box2DTrial::update(float dt) {
    
}

bool Box2DTrial::ccTouchBegan(CCTouch *touch, CCEvent *event) {
    return true;
}

void Box2DTrial::ccTouchEnded(CCTouch *touch, CCEvent *event) {
    
}


void Box2DTrial::menuCloseCallback(CCObject* pSender) {
    CCDirector::sharedDirector()->end();
    
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        exit(0);
    #endif
}