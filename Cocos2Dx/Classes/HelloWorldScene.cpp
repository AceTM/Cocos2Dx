#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* HelloWorld::scene() {
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init() {
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    _batchNode = CCSpriteBatchNode::create("Sprites.pvr.ccz");
    this->addChild(_batchNode);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Sprites.plist");
    
    _ship = CCSprite::createWithSpriteFrameName("SpaceFlier_sm_1.png");
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    _ship->setPosition(ccp(winSize.width * 0.1, winSize.height * 0.5));
    _batchNode->addChild(_ship, 1);
    
    //1.Create CCParallaxNode
    _backgroundNode = CCParallaxNodeExtra::node();
    this ->addChild(_backgroundNode, -1);
    
    //2.Create sprites to be added to CCParallaxNode
    _spacedust = CCSprite::create("bg_front_spacedust.png");
    _spacedust2 = CCSprite::create("bg_front_spacedust.png");
    _planetsunrise = CCSprite::create("bg_galaxy.png");
    _galaxy = CCSprite::create("bg_galaxy.png");
    _spacialanomaly = CCSprite::create("bg_spacialanomaly.png");
    _spacialanomaly2 = CCSprite::create("bg_spacialanomaly2.png");
    
    //3.Determine movement speeds for dusts and background
    CCPoint dustSpeed = ccp(0.1, 0.1);
    CCPoint bgSpeed = ccp(0.05, 0.05);
    
    //4.Add children to CCParallaxNode
    _backgroundNode->addChild(_spacedust, 0, dustSpeed, ccp(0, winSize.height/2));
    _backgroundNode->addChild(_spacedust2, 0, dustSpeed, ccp(_spacedust->getContentSize().width, winSize.height/2));
    _backgroundNode->addChild(_galaxy, -1, bgSpeed, ccp(0, winSize.height * 0.7));
    _backgroundNode->addChild(_planetsunrise, -1, bgSpeed, ccp(600, winSize.height * 0));
    _backgroundNode->addChild(_spacialanomaly, -1, bgSpeed, ccp(900, winSize.height * 0.3));
    _backgroundNode->addChild(_spacialanomaly2, -1, bgSpeed, ccp(1500, winSize.height * 0.9));
    
    HelloWorld::addChild(CCParticleSystemQuad::create("Stars1.plist"));
    HelloWorld::addChild(CCParticleSystemQuad::create("Stars2.plist"));
    HelloWorld::addChild(CCParticleSystemQuad::create("Stars3.plist"));
    
    this->scheduleUpdate();
    this->setAccelerometerEnabled(true);
    
    return true;
}


void HelloWorld::update(float dt) {
    CCPoint backgroundScrollVert = ccp(-1000, 0);
    _backgroundNode->setPosition(ccpAdd(_backgroundNode->getPosition(), ccpMult(backgroundScrollVert, dt)));
    
    CCArray *spaceDusts = CCArray::createWithCapacity(2);
    spaceDusts->addObject(_spacedust);
    spaceDusts->addObject(_spacedust2);
    for (int i = 0; i < spaceDusts->count(); i++) {
        CCSprite *spaceDust = (CCSprite *) (spaceDusts->objectAtIndex(i));
        float xPosition = _backgroundNode->convertToWorldSpace(spaceDust->getPosition()).x;
        float size = spaceDust->getContentSize().width;
        if (xPosition < - size/2) {
            _backgroundNode->incrementOffset(ccp(spaceDust->getContentSize().width*2, 0), spaceDust);
        }
    }
    
    CCArray *backgrounds = CCArray::createWithCapacity(4);
    backgrounds->addObject(_galaxy);
    backgrounds->addObject(_planetsunrise);
    backgrounds->addObject(_spacialanomaly);
    backgrounds->addObject(_spacialanomaly2);
    for (int i = 0; i < backgrounds->count(); i++) {
        CCSprite *background = (CCSprite *) (backgrounds->objectAtIndex(i));
        float xPosition = _backgroundNode->convertToWorldSpace(background->getPosition()).x;
        float size = background->getContentSize().width;
        if (xPosition < -size) {
            _backgroundNode->incrementOffset(ccp(2000, 0), background);
        }
    }
    
    //Control by tilting, with I haven't a clue
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    float maxY = winSize.height - _ship->getContentSize().height/2;
    float minY = _ship->getContentSize().height/2;
    
    float diff = (_shipPointsPerSecY * dt);
    float newY = _ship->getPosition().y + diff;
    newY = MIN(MAX(newY, minY), maxY);
    _ship->setPosition(ccp(_ship->getPosition().x, newY));
}

void HelloWorld::didAccelerate(CCAcceleration *pAccelerationValue) {
    //Tilting control function, no clue here
#define KFILTERINGFACTOR 0.1
#define KRESTACCELX -0.6
#define KSHIPMAXPOINTPERSEC (winSize.height * 0.5)
#define KMAXDIFFX 0.2
    
    double rollingX;
    
    //Cocos2DX inverts X and Y accelerometer depending on device orietation
    //in landscape mode right x=-y and y=x
    pAccelerationValue->x = pAccelerationValue->y;
    rollingX = (pAccelerationValue->x * KFILTERINGFACTOR) + (rollingX * (1.0 - KFILTERINGFACTOR));
    float accelX = pAccelerationValue->x - rollingX;
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    float accelDiff = accelX - KRESTACCELX;
    float accelFraction = accelDiff / KMAXDIFFX;
    _shipPointsPerSecY = KSHIPMAXPOINTPERSEC * accelFraction;
}

void HelloWorld::menuCloseCallback(CCObject* pSender) {
    CCDirector::sharedDirector()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
