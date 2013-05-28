//
//  HelloWorldScene.h
//  TestBox2D
//
//  Created by Trung Kien Do on 13/05/23.
//  Copyright __MyCompanyName__ 2013年. All rights reserved.
//
#ifndef __BOX2DIMPORT_H__
#define __BOX2DIMPORT_H__

// When you import this file, you import all the cocos2d classes
#include "cocos2d.h"
#include "Box2D.h"

USING_NS_CC;

class PhysicsSprite : public CCSprite
{
public:
    PhysicsSprite();
    void setPhysicsBody(b2Body * body);
    virtual bool isDirty(void);
    virtual cocos2d::CCAffineTransform nodeToParentTransform(void);
private:
    b2Body* m_pBody;    // strong ref
};

class Box2DImport : public cocos2d::CCLayer {
public:
    ~Box2DImport();
    Box2DImport();
    
    // returns a Scene that contains the HelloWorld as the only child
    static cocos2d::CCScene* scene();
    
    void initPhysics();
    // adds a new sprite at a given coordinate
    void addNewSpriteAtPosition(cocos2d::CCPoint p, int id);
    void addNewCircleAtPosition(cocos2d::CCPoint p, int id);
    void addNewStickAtPosition(cocos2d::CCPoint p, int id);
    

    virtual void draw();
    virtual void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    virtual void ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    virtual void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    void update(float dt);
    
private:
    b2World* world;
    cocos2d::CCTexture2D* m_pSpriteTexture; // weak ref
    
    b2Body *_iconBody;
    b2Body* groundBody;
    b2Fixture *_iconFixture;
    b2Fixture *_bottomFixture;
    b2MouseJoint *_mouseJoint;

};

#endif // __HELLO_WORLD_H__
