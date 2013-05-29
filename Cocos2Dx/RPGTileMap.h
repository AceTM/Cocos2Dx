//
//  RPGTileMap.h
//  Cocos2Dx
//
//  Created by MinhNT on 13/05/28.
//
//

#ifndef __Cocos2Dx__RPGTileMap__
#define __Cocos2Dx__RPGTileMap__

#include <iostream>
#include "cocos2d.h"
#include "CCInputListener.h"
#include "SneakyJoystick.h"
#include "SneakyButton.h"
#include "SneakyJoystickSkinnedBase.h"
#include "SneakyButtonSkinnedBase.h"

USING_NS_CC;

class TileMap : public CCLayer {
private:
    SneakyJoystick *leftJoystick;
    SneakyButton *accelButton;
    
    SneakyJoystickSkinnedBase *joystickBase;
    SneakyJoystick *joystick;
    
    SneakyButtonSkinnedBase *buttonBase;
    SneakyButton *button;
    
    CCTMXTiledMap *tileMap;
    CCCamera *camera;
public:
    virtual bool init();
    virtual bool ccTouchBegan(CCTouch *touch, CCEvent *event);
    virtual void ccTouchEnded(CCTouch *touch, CCEvent *event);
    
    //Selector callback
    void update(float dt);
    void menuCloseCallback(CCObject *pSender);
    
    static CCScene* scene();
    CREATE_FUNC(TileMap);
};

#endif /* defined(__Cocos2Dx__RPGTileMap__) */
