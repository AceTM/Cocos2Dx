//
//  VirtualInput.cpp
//  Cocos2Dx
//
//  Created by MinhNT on 13/05/29.
//
//

#include "VirtualInput.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "SneakyJoystick.h"
#include "SneakyButton.h"
#include "SneakyJoystickSkinnedBase.h"
#include "SneakyButtonSkinnedBase.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* VirtualInput::scene() {
    // 'scene' is an autorelease object
    cocos2d::CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    VirtualInput *layer = VirtualInput::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool VirtualInput::init() {
    CCRect joystickRect;
    SneakyJoystickSkinnedBase *joystickBase = SneakyJoystickSkinnedBase::create();
    SneakyJoystick *joystick = new SneakyJoystick;
    joystick->initWithRect(joystickRect);
    joystickBase->setBackgroundSprite(CCSprite::create(""));
    joystickBase->setThumbSprite(CCSprite::create(""));
    joystickBase->setJoystick(joystick);
    joystickBase->setPosition(ccp(0, 0));
    leftJoystick = joystickBase->getJoystick();
    this->addChild(joystickBase);
    
    CCRect buttonRect;
    SneakyButtonSkinnedBase *buttonBase = SneakyButtonSkinnedBase::create();
    SneakyButton *button = new SneakyButton;
    button->initWithRect(buttonRect);
    buttonBase->setDefaultSprite(CCSprite::create(""));
    buttonBase->setActivatedSprite(CCSprite::create(""));
    buttonBase->setPressSprite(CCSprite::create(""));
    buttonBase->setButton(button);
    buttonBase->setPosition(ccp(0, 0));
    accelButton = buttonBase->getButton();
    accelButton->setIsToggleable(true);
    this->addChild(buttonBase);
    
    return true;
}

void VirtualInput::update(float dt) {
    
}

bool VirtualInput::ccTouchBegan(CCTouch *touch, CCEvent *event) {
    return true;
}

void VirtualInput::ccTouchEnded(CCTouch *touch, CCEvent *event) {
    
}

void VirtualInput::menuCloseCallback(CCObject* pSender) {
    CCDirector::sharedDirector()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}