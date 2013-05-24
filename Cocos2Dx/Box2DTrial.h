//
//  Box2DTrial.h
//  Cocos2Dx
//
//  Created by MinhNT on 13/05/24.
//
//

#ifndef __Cocos2Dx__Box2DTrial__
#define __Cocos2Dx__Box2DTrial__

#include <iostream>
#include "cocos2d.h"
#include "CCInputListener.h"
#include "Box2D.h"

USING_NS_CC;

class Box2DTrial: public CCLayer {
private:
    b2Body *paddleBody;
    b2World *world;
public:
    static CCScene* scene();
    
    virtual bool init();
    virtual bool ccTouchBegan(CCTouch *touch, CCEvent *event);
    virtual void ccTouchEnded(CCTouch *touch, CCEvent *event);
    
    //Selector callback
    void update(float dt);
    void menuCloseCallback(CCObject *pSender);
    
    CREATE_FUNC(Box2DTrial);
};

#endif /* defined(__Cocos2Dx__Box2DTrial__) */
