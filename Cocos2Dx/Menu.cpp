//
//  Menu.cpp
//  Cocos2Dx
//
//  Created by MinhNT on 13/05/22.
//
//

#include "Menu.h"
#include "SimpleAudioEngine.h"
#include "HelloWorldScene.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* Menu::scene() {
    // 'scene' is an autorelease object
    cocos2d::CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    Menu *layer = Menu::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool Menu::init() {
    if (!CCLayer::init()) return false;
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    background = CCSprite::create("Background 320x480.png");
    button1 = CCSprite::create("Button1.png");
    button2 = CCSprite::create("Button2.png");
    button3 = CCSprite::create("Button3.png");
    button4 = CCSprite::create("Button4.png");
    
    background->setPosition(ccp(winSize.width/2, winSize.height/2));
    button1->setPosition(ccp(winSize.width/2, 350));
    button2->setPosition(ccp(winSize.width/2, 300));
    button3->setPosition(ccp(winSize.width/2, 250));
    button4->setPosition(ccp(winSize.width/2, 200));
    
    button1->setScale(0.5);
    button2->setScale(0.5);
    button3->setScale(0.5);
    button4->setScale(0.5);
    
    this->addChild(background, 0);
    this->addChild(button1, 1);
    this->addChild(button2, 1);
    this->addChild(button3, 1);
    this->addChild(button4, 1);
    
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    return true;
}

void Menu::update(float dt) {
    
}

bool Menu::ccTouchBegan(CCTouch *touch, CCEvent *event) {
    return true;
}

void Menu::ccTouchEnded(CCTouch *touch, CCEvent *event) {
    CCLog("Touch begun!");
    CCPoint touchLoc = this->getParent()->convertTouchToNodeSpace(touch);
    CCRect spriteRec = button1->boundingBox();
    if (spriteRec.containsPoint(touchLoc)) {
        CCScene *shooterScene = HelloWorld::scene();
        CCDirector::sharedDirector()->setDepthTest(true);
        CCTransitionScene *transition = CCTransitionFadeUp::create(1, shooterScene);
        CCDirector::sharedDirector()->replaceScene(transition);
    }
    else {
        
    }
}


void Menu::menuCloseCallback(CCObject* pSender) {
    CCDirector::sharedDirector()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}