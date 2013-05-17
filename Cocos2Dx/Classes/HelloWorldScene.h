#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "CCParallaxNodeExtra.h"
#include "CCInputListener.h"

USING_NS_CC;

class HelloWorld : public cocos2d::CCLayer, public cocos2d::CCInputListener {
private:
    CCSpriteBatchNode * _batchNode;
    CCSprite *_ship;
    CCParallaxNodeExtra * _backgroundNode;
    CCSprite *_spacedust;
    CCSprite *_spacedust2;
    CCSprite *_planetsunrise;
    CCSprite *_galaxy;
    CCSprite *_spacialanomaly;
    CCSprite *_spacialanomaly2;
    
    float _shipPointsPerSecY;
    
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    virtual void didAccelerate(CCAcceleration *pAccelerationValue);
    
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    void update(float dt);
    void updateKeyboard(CCTime dt);
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__