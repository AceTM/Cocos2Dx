//
//  HelloWorldScene.cpp
//  TestBox2D
//
//  Created by Trung Kien Do on 13/05/23.
//  Copyright __MyCompanyName__ 2013年. All rights reserved.
//
#include "Box2DImport.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

#define PTM_RATIO 32

enum {
    kTagParentNode = 1,
};

PhysicsSprite::PhysicsSprite(): m_pBody(NULL) {
    
}

void PhysicsSprite::setPhysicsBody(b2Body * body) {
    m_pBody = body;
}

// this method will only get called if the sprite is batched.
// return YES if the physics values (angles, position ) changed
// If you return NO, then nodeToParentTransform won't be called.
bool PhysicsSprite::isDirty(void) {
    return true;
}

// returns the transform matrix according the Chipmunk Body values
CCAffineTransform PhysicsSprite::nodeToParentTransform(void) {
    b2Vec2 pos  = m_pBody->GetPosition();
    
    float x = pos.x * PTM_RATIO;
    float y = pos.y * PTM_RATIO;
    
    if ( isIgnoreAnchorPointForPosition() ) {
        x += m_obAnchorPointInPoints.x;
        y += m_obAnchorPointInPoints.y;
    }
    
    // Make matrix
    float radians = m_pBody->GetAngle();
    float c = cosf(radians);
    float s = sinf(radians);
    
    if( ! m_obAnchorPointInPoints.equals(CCPointZero) ){
        x += c*-m_obAnchorPointInPoints.x + -s*-m_obAnchorPointInPoints.y;
        y += s*-m_obAnchorPointInPoints.x + c*-m_obAnchorPointInPoints.y;
    }
    // Rot, Translate Matrix
    m_sTransform = CCAffineTransformMake(c, s, -s, c, x, y);
    return m_sTransform;
}

Box2DImport::Box2DImport() {    
    setTouchEnabled( true );
    setAccelerometerEnabled( true );
    _mouseJoint = NULL;
    
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    CCSprite *groundImg = CCSprite::create("BG_1.png");
    groundImg->setPosition(ccp(s.width/2, s.height/2));
    this->addChild(groundImg, -1);
        
    // init physics
    this->initPhysics();
    
    CCSpriteBatchNode *parent = CCSpriteBatchNode::create("baseSprite.pvr.ccz");
    this->addChild(parent);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("baseSprite.plist");
    //    m_pSpriteTexture = parent->getTexture();
    //    addChild(parent, 0, kTagParentNode);   
    
    for (int i = 1; i<=15; i++) {
        float randx = (s.width/10) * (rand() %10 );
        float randy = (s.height/10) * (rand() %10 );
        addNewSpriteAtPosition(ccp(randx,  randy), 11);
    }
    /**********ICON BODY***********/
    
    CCSprite *iconSprite = CCSprite::create("angrybirds.png");
    iconSprite->setPosition(ccp(0, 0));
    iconSprite->setTag(11);
    this->addChild(iconSprite, 100);
    
    b2BodyDef iconBodyDef;
    iconBodyDef.type = b2_dynamicBody;
    iconBodyDef.position.Set(s.width/2/PTM_RATIO, s.height/12/PTM_RATIO);
    iconBodyDef.userData = iconSprite;
    _iconBody = world->CreateBody(&iconBodyDef);
    
    b2CircleShape circle;
    circle.m_radius = 32.0/PTM_RATIO;
    
    b2FixtureDef iconFixtureDef;
    iconFixtureDef.shape        = &circle;
    iconFixtureDef.density      = 2.0f;
    iconFixtureDef.friction     = 0.5f;
    iconFixtureDef.restitution  = 0.5f;
    
    _iconFixture = _iconBody->CreateFixture(&iconFixtureDef);
    
    /**********ICON BODY***********/
    
    scheduleUpdate();
}

Box2DImport::~Box2DImport() {
    delete world;
    world = NULL;
    
    //delete m_debugDraw;
}

void Box2DImport::initPhysics() {
    b2Vec2 gravity;
    gravity.Set(0.0f, -10.0f);
    bool worldSleep = true;
    world = new b2World(gravity, worldSleep);
    world->SetContinuousPhysics(true);
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    // Define the ground body.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0, 0); // bottom-left corner
    groundBody = world->CreateBody(&groundBodyDef);
    //Groundbox shaping
    b2PolygonShape groundBox;
    //Bottom
    groundBox.SetAsEdge(b2Vec2(0, 0), b2Vec2(winSize.width/PTM_RATIO, 0));
    _bottomFixture = groundBody->CreateFixture(&groundBox, 0);
    //Top
    groundBox.SetAsEdge(b2Vec2(0, winSize.height/PTM_RATIO), b2Vec2(winSize.width/PTM_RATIO, winSize.height/PTM_RATIO));
    groundBody->CreateFixture(&groundBox, 0);
    //Left
    groundBox.SetAsEdge(b2Vec2(0, winSize.height/PTM_RATIO), b2Vec2(0, 0));
    groundBody->CreateFixture(&groundBox, 0);
    //Right
    groundBox.SetAsEdge(b2Vec2(winSize.width/PTM_RATIO, winSize.height/PTM_RATIO), b2Vec2(winSize.width/PTM_RATIO, 0));
    groundBody->CreateFixture(&groundBox, 0);
}

void Box2DImport::draw() {
    CCLayer::draw();
    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );
    kmGLPushMatrix();
    world->DrawDebugData();
    kmGLPopMatrix();
}

void Box2DImport::addNewSpriteAtPosition(CCPoint p, int id) {
    CCLOG("Add sprite %0.2f x %02.f",p.x,p.y);
    
    char str[100];
    sprintf(str, "Props_%d.gif", id);
    CCSprite *sprite = CCSprite::createWithSpriteFrameName(str);
    sprite->setPosition( CCPointMake( p.x, p.y) );
    float ScalVal = CCRANDOM_0_1();
    sprite->setScale(ScalVal < 0.5 ? 0.5 : ScalVal);
    this->addChild(sprite);
    
    CCLOG("%f, %f", sprite->getContentSize().width, sprite->getContentSize().height);
    
    // Define the dynamic body.
    //Set up a 1m squared box in the physics world
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(p.x/PTM_RATIO, p.y/PTM_RATIO);
    
    b2Body *body = world->CreateBody(&bodyDef);
    
    // Define another box shape for our dynamic body.
    //    b2PolygonShape dynamicBox;
    //    dynamicBox.SetAsBox(1.3f, 1.3f);//These are mid points for our 1m box
    
    b2CircleShape circle;
    circle.m_radius = 35.0*ScalVal/PTM_RATIO;
    
    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circle;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.5f;
    fixtureDef.restitution = 0.5f;
    body->CreateFixture(&fixtureDef);
    body->SetUserData(sprite);
    
}

void Box2DImport::addNewCircleAtPosition(CCPoint p, int id) {
    CCLOG("Add circle %0.2f x %02.f",p.x,p.y);
    
    char str[100];
    sprintf(str, "Props_%d.gif", id);
    CCSprite *sprite = CCSprite::createWithSpriteFrameName(str);
    sprite->setPosition( CCPointMake( p.x, p.y) );
    float ScalVal = CCRANDOM_0_1();
    sprite->setScale(ScalVal < 0.5 ? 0.5 : ScalVal);
    this->addChild(sprite);
    
    CCLOG("%f, %f", sprite->getContentSize().width, sprite->getContentSize().height);
    
    // Define the dynamic body.
    //Set up a 1m squared box in the physics world
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(p.x/PTM_RATIO, p.y/PTM_RATIO);
    
    b2Body *body = world->CreateBody(&bodyDef);
    
    // Define another box shape for our dynamic body.
    //    b2PolygonShape dynamicBox;
    //    dynamicBox.SetAsBox(1.3f, 1.3f);//These are mid points for our 1m box
    
    b2CircleShape circle;
    float m_radius = sprite->getContentSize().height/2;
    circle.m_radius = m_radius*ScalVal/PTM_RATIO;
    
    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circle;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.5f;
    fixtureDef.restitution = 0.5f;
    body->CreateFixture(&fixtureDef);
    body->SetUserData(sprite);
    
}


void Box2DImport::addNewStickAtPosition(CCPoint p, int id) {
    CCLOG("Add Stick at %0.2f x %02.f",p.x,p.y);
    
    char str[100];
    sprintf(str, "Props_%d.gif", id);
    CCSprite *sprite = CCSprite::createWithSpriteFrameName(str);
    sprite->setPosition( CCPointMake( p.x, p.y) );
    float ScalVal = CCRANDOM_0_1();
    sprite->setScale(ScalVal < 0.5 ? 0.5 : ScalVal);
    this->addChild(sprite);
    
    float boxWidth = sprite->getContentSize().width;
    float boxHeight = sprite->getContentSize().height;
    
    CCLOG("%f, %f", sprite->getContentSize().width, sprite->getContentSize().height);
    
    // Define the dynamic body.
    //Set up a 1m squared box in the physics world
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(p.x/PTM_RATIO, p.y/PTM_RATIO);
    
    b2Body *body = world->CreateBody(&bodyDef);
    
    // Define another box shape for our dynamic body.
    //    b2PolygonShape dynamicBox;
    //    dynamicBox.SetAsBox(1.3f, 1.3f);//These are mid points for our 1m box
    
    
    b2PolygonShape boxShape;
    boxShape.SetAsBox(boxWidth/2, boxHeight/2);
    
    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &boxShape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.5f;
    fixtureDef.restitution = 0.5f;
    body->CreateFixture(&fixtureDef);
    body->SetUserData(sprite);
    
}



void Box2DImport::update(float dt) {
    
    int velocityIterations = 8;
    int positionIterations = 1;
    
    world->Step(dt, velocityIterations, positionIterations);
    
    for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
    {
        if (b->GetUserData() != NULL) {
			//Synchronize the AtlasSprites position and rotation with the corresponding body
			CCSprite* myActor = (CCSprite*)b->GetUserData();
			myActor->setPosition( CCPointMake( b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO) );
			myActor->setRotation( -1 * CC_RADIANS_TO_DEGREES(b->GetAngle()/5) );
        }
    }
}

void Box2DImport::ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event) {
    if (_mouseJoint != NULL) return;
    CCTouch *myTouch = (CCTouch *)touches->anyObject();
    CCPoint location = myTouch->getLocationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);
    b2Vec2 locationWorld = b2Vec2(location.x/PTM_RATIO, location.y/PTM_RATIO);
    
    if (_iconFixture->TestPoint(locationWorld)) {
        b2MouseJointDef md;
        md.bodyA = groundBody;
        md.bodyB = _iconBody;
        md.target = locationWorld;
        md.collideConnected = true;
        md.maxForce = 1000.0f * _iconBody->GetMass();
        
        _mouseJoint = (b2MouseJoint *)world->CreateJoint(&md);
        
        _iconBody->SetAwake(true);
    }
}

void Box2DImport::ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event) {
    if (_mouseJoint == NULL) return;
    
    CCTouch *myTouch = (CCTouch *)touches->anyObject();
    CCPoint location = myTouch->getLocationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);
    b2Vec2 locationWorld = b2Vec2(location.x/PTM_RATIO, location.y/PTM_RATIO);
    
    _mouseJoint->SetTarget(locationWorld);
}


void Box2DImport::ccTouchesEnded(CCSet* touches, CCEvent* event) {
    CCSetIterator it;
    if (_mouseJoint)
    {
        world->DestroyJoint(_mouseJoint);
        _mouseJoint = NULL;
    }
    
}

CCScene* Box2DImport::scene(){
    CCScene *scene = CCScene::create();
    
    // add layer as a child to scene
    CCLayer* layer = new Box2DImport();
    scene->addChild(layer);
    layer->release();
    
    return scene;
}

