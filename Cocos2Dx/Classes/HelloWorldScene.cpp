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
    _fighterNode = CCSpriteBatchNode::create("fighter.pvr.ccz");
    _bulletNode = CCSpriteBatchNode::create("bullet.pvr.ccz");
    
    this->addChild(_batchNode);
    this->addChild(_fighterNode);
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Sprites.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("fighter.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("bullet.plist");
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    _ship = CCSprite::createWithSpriteFrameName("frame_0.gif");
    _ship->setPosition(ccp(winSize.width * 0.5, winSize.height * 0.1));
    _ship->setTag(2);
    
    _shipShadow = CCSprite::createWithSpriteFrameName("frame_0.gif");
    _shipShadow->setPosition(ccp(_ship->getPositionX()-5, _ship->getPositionY()-15));
    _shipShadow->setColor(ccBLACK);
    _shipShadow->setOpacity(80);
    _shipShadow->setTag(1);
    
    _fighterNode->addChild(_shipShadow, 1);
    _fighterNode->addChild(_ship, 1);
    
    //1.Create CCParallaxNode
    _backgroundNode = CCParallaxNodeExtra::node();
    this ->addChild(_backgroundNode, -1);
    
    //2.Create sprites to be added to CCParallaxNode
    _spacedust = CCSprite::create("backgrounddetailed1giant.png");
    _spacedust2 = CCSprite::create("backgrounddetailed1giant.png");
//    _planetsunrise = CCSprite::create("bg_galaxy.png");
//    _galaxy = CCSprite::create("bg_galaxy.png");
//    _spacialanomaly = CCSprite::create("bg_spacialanomaly.png");
//    _spacialanomaly2 = CCSprite::create("bg_spacialanomaly2.png");
    
    //3.Determine movement speeds for dusts and background
    CCPoint dustSpeed = ccp(0.1, 0.1);
//    CCPoint bgSpeed = ccp(0.05, 0.05);
    
    //4.Add children to CCParallaxNode
    _backgroundNode->addChild(_spacedust, 0, dustSpeed, ccp(0, -winSize.height));
    _backgroundNode->addChild(_spacedust2, 0, dustSpeed, ccp(0, -_spacedust->getContentSize().height));
//    _backgroundNode->addChild(_galaxy, -1, bgSpeed, ccp(0, winSize.height * 0.7));
//    _backgroundNode->addChild(_planetsunrise, -1, bgSpeed, ccp(600, winSize.height * 0));
//    _backgroundNode->addChild(_spacialanomaly, -1, bgSpeed, ccp(900, winSize.height * 0.3));
//    _backgroundNode->addChild(_spacialanomaly2, -1, bgSpeed, ccp(1500, winSize.height * 0.9));
    
//    HelloWorld::addChild(CCParticleSystemQuad::create("Stars1.plist"));
//    HelloWorld::addChild(CCParticleSystemQuad::create("Stars2.plist"));
//    HelloWorld::addChild(CCParticleSystemQuad::create("Stars3.plist"));
    
    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    cache->addSpriteFramesWithFile("fighter.plist");
    
    CCArray* animFrames = CCArray::createWithCapacity(15);
    char str[100] = {0};
    for(int i = 0; i < 4; i++)
    {
        sprintf(str, "frame_%0d.gif", i);
        CCSpriteFrame* frame = cache->spriteFrameByName( str );
        animFrames->addObject(frame);
    }
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(animFrames, 0.1f);
    _ship->runAction(CCRepeatForever::create(CCAnimate::create(animation)));
    
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    
    this->scheduleUpdate();
    this->setAccelerometerEnabled(true);
    this->setTouchEnabled(true);
    return true;
}

void HelloWorld::update(float dt) {
    CCPoint backgroundScrollVert = ccp(0, -1000);
    _backgroundNode->setPosition(ccpAdd(_backgroundNode->getPosition(), ccpMult(backgroundScrollVert, dt)));
    
    CCArray *spaceDusts = CCArray::createWithCapacity(2);
    spaceDusts->addObject(_spacedust);
    spaceDusts->addObject(_spacedust2);
    for (int i = 0; i < spaceDusts->count(); i++) {
        CCSprite *spaceDust = (CCSprite *) (spaceDusts->objectAtIndex(i));
        float yPosition = _backgroundNode->convertToWorldSpace(spaceDust->getPosition()).y;
//        CCLog("Y Position: %f", yPosition);
        float size = spaceDust->getContentSize().height;
        if (yPosition < -size/2) {
            _backgroundNode->incrementOffset(ccp(0, spaceDust->getContentSize().width), spaceDust);
        }
    }
    
//    CCArray *backgrounds = CCArray::createWithCapacity(4);
//    backgrounds->addObject(_galaxy);
//    backgrounds->addObject(_planetsunrise);
//    backgrounds->addObject(_spacialanomaly);
//    backgrounds->addObject(_spacialanomaly2);
//    for (int i = 0; i < backgrounds->count(); i++) {
//        CCSprite *background = (CCSprite *) (backgrounds->objectAtIndex(i));
//        float yPosition = _backgroundNode->convertToWorldSpace(background->getPosition()).y;
//        float size = background->getContentSize().height;
//        if (yPosition < -size) {
//            _backgroundNode->incrementOffset(ccp(0, 2000), background);
//        }
//    }
    
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

inline CCPoint locationInGLFromTouch(CCTouch &touch) {
    auto director = CCDirector::sharedDirector();
    return director -> convertToGL(touch.getLocationInView());
}

bool HelloWorld::ccTouchBegan(CCTouch *touch, CCEvent *event) {
    CCLog("Touch begun!");
    CCPoint touchLoc = this->getParent()->convertTouchToNodeSpace(touch);
    CCRect spriteRec = _ship->boundingBox();
    if (spriteRec.containsPoint(touchLoc)) {
        CCLog("Bravo, touched on sprite %i", _ship->getTag());
        _shipTouched = true;
    }
    else {
//        CCMoveTo *moveShip = CCMoveTo::create(0.2f, ccp(touchLoc.x, touchLoc.y));
//        _ship->runAction(moveShip);
        
//        ccBezierConfig bezier;
//        bezier.controlPoint_1 = ccp(_ship->getPositionX(), _ship->getPositionY()+20);
//        bezier.controlPoint_2 = ccp(_ship->getPositionX()-30, _ship->getPositionY()+90);
//        bezier.endPosition = ccp(touchLoc.x, touchLoc.y);
//        
//        CCBezierTo *bezierTo = CCBezierTo::create(2, bezier);
//        _ship->runAction(bezierTo);
    }
    return true;
}

//void HelloWorld::ccTouchesBegan(CCSet *touches, CCEvent *event) {
//    CCLog("Touch begun");
//}

void HelloWorld::ccTouchMoved(CCTouch *touch, CCEvent *event) {
    CCLog("Touch moving");
    
    CCPoint location = touch->getLocationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);
    //auto touch = dynamic_cast<CCTouch *>(*it);
    CCLog("Location x:%f location y:%f", location.x, location.y);
    if (_shipTouched == true) {
        _ship->cocos2d::CCNode::setPosition(location.x, location.y);
        _shipShadow->setPosition(ccp(_ship->getPositionX()-5, _ship->getPositionY()-15));
    }
    else {
//        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        _bullet = CCSprite::create("Bullet.png");
        _bullet->setScale(0.3f);
        _bullet->setPosition(ccp(_ship->getPositionX(),_ship->getPositionY()+25));
        
//        int offX = location.x-projectile->getPosition().x;
//        int offY = location.y-projectile->getPosition().y;
//        
//        if(offX<=0) return;
        
        this->addChild(_bullet);
        
//        int realX = location.x + (projectile->getContentSize().width/2);
//        float ratio = (float) offX/(float) offY;
//        int realY = location.y;
        
        CCPoint realdest =  ccp(_ship->getPositionX(), _ship->getPositionY()+500);
        CCPoint bulletPos = _bullet->getPosition();
        int offRealX = realdest.x - _bullet->getPosition().x;
        int offRealY = realdest.y - _bullet->getPosition().y;
        
        float length = sqrtf((offRealX*offRealX)+(offRealY*offRealY));
        float velocity = 460/1;
        
        float realMoveDuration = length/velocity;
        
        CCActionInterval *actionMove = CCMoveTo::create(realMoveDuration, realdest);
        CCAction *actionRemove = CCRemoveSelf::create();
        CCFiniteTimeAction *timeAction = CCSequence::create(actionMove, actionRemove, NULL);
        // *delay = CCDelayTime::create(1.2f);
        _bullet->runAction(timeAction);

    }
}

void HelloWorld::ccTouchEnded(CCTouch *touch, CCEvent *event) {
    CCLog("Touch ended");
    if (_shipTouched == true) {
        _shipTouched = false;
    }
}

//void HelloWorld::ccTouchCancelled(CCTouch *touch, CCEvent *event) {
//    for (auto it = touches->begin(); it != touches->end(); it++) {
//        CCLog("Touch cancelled");
//    }
//}

void HelloWorld::menuCloseCallback(CCObject* pSender) {
    CCDirector::sharedDirector()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
