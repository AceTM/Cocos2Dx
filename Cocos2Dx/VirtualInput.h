//
//  VirtualInput.h
//  Cocos2Dx
//
//  Created by MinhNT on 13/05/29.
//
//

#ifndef __Cocos2Dx__VirtualInput__
#define __Cocos2Dx__VirtualInput__

#include <iostream>
#include "cocos2d.h"
#include "CCInputListener.h"
#include "SneakyJoystick.h"
#include "SneakyButton.h"

USING_NS_CC;

class VirtualInput : public CCLayer, public CCInputListener {
private:
    SneakyJoystick *leftJoystick;
    SneakyButton *accelButton;
public:
    virtual bool init();
    virtual bool ccTouchBegan(CCTouch *touch, CCEvent *event);
    virtual void ccTouchEnded(CCTouch *touch, CCEvent *event);
    
    //Selector callback
    void update(float dt);
    void menuCloseCallback(CCObject *pSender);
    
    static CCScene* scene();
    CREATE_FUNC(VirtualInput);
};

#endif /* defined(__Cocos2Dx__VirtualInput__) */
