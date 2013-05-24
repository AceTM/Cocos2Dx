#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "CCParallaxNodeExtra.h"
#include "CCInputListener.h"

USING_NS_CC;
using namespace std;

class HelloWorld : public cocos2d::CCLayer, public cocos2d::CCInputListener {
private:
    CCSpriteBatchNode * _batchNode;
    CCSpriteBatchNode * _fighterNode;
    CCSpriteBatchNode * _bulletNode;
    CCSpriteBatchNode * _enemyNode;
    
    CCParallaxNodeExtra * _backgroundNode;
    
    CCSprite *_ship;
    CCSprite *_enemy;
    CCSprite *_bullet;
    CCSprite *_shipShadow;
    CCSprite *_enemyShadow;
    CCSprite *_spacedust;
    CCSprite *_spacedust2;
    CCSprite *_planetsunrise;
    CCSprite *_galaxy;
    CCSprite *_spacialanomaly;
    CCSprite *_spacialanomaly2;
    CCArray *_asteroids;
    CCArray *_bullets;
    CCArray *_evilDoers;
    
    bool _shipTouched;
    bool _enemyExists;
    int _enemyHP = 20;
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    virtual bool ccTouchBegan(CCTouch *touch, CCEvent *event);
    virtual void ccTouchMoved(CCTouch *touch, CCEvent *event);
    virtual void ccTouchEnded(CCTouch *touch, CCEvent *event);
    
    virtual CCAnimation *animeCreation(char *animeName, float animSpeed,
                                      int animeFrames, char *frameName, char *frameExtension);
    float randomValueBetween(float low, float high);
    void setInvisible(CCNode *node);
    float getTimeTick();
    
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject *pSender);
    void update(float dt);
    void moveBullet(CCSprite *bullet);
    void spriteMoveFinished(CCNode *sender);
    void enemyAppears();
    void addTarget();
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__