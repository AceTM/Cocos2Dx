//
//  Menu.h
//  Cocos2Dx
//
//  Created by MinhNT on 13/05/22.
//
//

#ifndef __Cocos2Dx__Menu__
#define __Cocos2Dx__Menu__

#include <iostream>
#include "cocos2d.h"
#include "CCParallaxNodeExtra.h"
#include "CCInputListener.h"

USING_NS_CC;

class Menu : public CCLayer, public CCInputListener {
private:
    CCSprite *background;
    CCSprite *button1;
    CCSprite *button2;
    CCSprite *button3;
    CCSprite *button4;
    CCTouchDelegate *menuTouchDelegate;
public:
    virtual bool init();
    virtual bool ccTouchBegan(CCTouch *touch, CCEvent *event);
    virtual void ccTouchEnded(CCTouch *touch, CCEvent *event);
    //Selector callback
    void update(float dt);
    void menuCloseCallback(CCObject *pSender);
    
    static CCScene* scene();
    CREATE_FUNC(Menu);
};

#endif /* defined(__Cocos2Dx__Menu__) */
