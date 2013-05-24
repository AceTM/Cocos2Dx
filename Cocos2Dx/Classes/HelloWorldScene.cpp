#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

using namespace std;

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* HelloWorld::scene() {
    CCScene *scene = CCScene::create();
    HelloWorld *layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init() {
    if (!CCLayer::init())return false;
    _evilDoers = new CCArray;
    _bullets = new CCArray;
    
    _batchNode = CCSpriteBatchNode::create("Sprites.pvr.ccz");
    _fighterNode = CCSpriteBatchNode::create("fighter.pvr.ccz");
    this->addChild(_batchNode);
    this->addChild(_fighterNode);
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    CCSprite *menuSprite = CCSprite::create("ShooterMenu.png");
    menuSprite->setPosition(ccp(winSize.width * 0.5, 25));
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Sprites.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("fighter.plist");
    
    
    _ship = CCSprite::create();
    _ship->setPosition(ccp(winSize.width * 0.5, winSize.height * 0.3));
    _ship->setTag(5);
    
    _shipShadow = CCSprite::createWithSpriteFrameName("frame_0.gif");
    _shipShadow->setPosition(ccp(_ship->getPositionX()-5, _ship->getPositionY()-25));
    _shipShadow->setColor(ccBLACK);
    _shipShadow->setOpacity(80);

    _fighterNode->addChild(_shipShadow, 1);
    //_fighterNode->addChild(_ship, 1);
    
    _backgroundNode = CCParallaxNodeExtra::node();
    
    _spacedust = CCSprite::create("netBackground2.png");
    _spacedust2 = CCSprite::create("netBackground2.png");
    
    CCPoint dustSpeed = ccp(0.3, 0.3);
    
    _backgroundNode->addChild(_spacedust, 0, dustSpeed, ccp(winSize.width/2, -winSize.height));
    //_backgroundNode->addChild(_spacedust2, 0, dustSpeed, ccp(0, -_spacedust->getContentSize().height/1.5));
    
    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    cache->addSpriteFramesWithFile("fighter.plist");
    CCArray* animFrames = CCArray::createWithCapacity(15);
    char str[100] = {0};
    for(int i = 0; i < 4; i++) {
        sprintf(str, "frame_%0d.gif", i);
        CCSpriteFrame* frame = cache->spriteFrameByName( str );
        animFrames->addObject(frame);
    }
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(animFrames, 0.1f);
    _ship->runAction(CCRepeatForever::create(CCAnimate::create(animation)));
    
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    HelloWorld::enemyAppears();
    
    this->addChild(_backgroundNode, -1);
    this->addChild(_ship, 1);
    this->addChild(menuSprite, 5);
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
    //spaceDusts->addObject(_spacedust2);
    for (int i = 0; i < spaceDusts->count(); i++) {
        CCSprite *spaceDust = (CCSprite *) (spaceDusts->objectAtIndex(i));
        float yPosition = _backgroundNode->convertToWorldSpace(spaceDust->getPosition()).y;
        float size = spaceDust->getContentSize().height;
        if (yPosition < -size/2) {
            _backgroundNode->incrementOffset(ccp(0, spaceDust->getContentSize().width+300), spaceDust);
        }
    }
    
    if (_enemyExists) {
        _enemyShadow->setPosition(ccp(_enemy->getPositionX()-5, _enemy->getPositionY()-25));
        
        CCArray *bulletsToDelete = new CCArray;
        CCObject* it = NULL;
        CCObject* jt = NULL;
        CCARRAY_FOREACH(_bullets, it) {
            CCSprite *bullet = dynamic_cast<CCSprite*>(it);
            CCRect bulletRect = CCRectMake(
                                           bullet->getPositionX() - (bullet->getContentSize().width),
                                           bullet->getPositionY() - (bullet->getContentSize().height),
                                           bullet->getContentSize().width,
                                           bullet->getContentSize().height);
            
            CCArray* targetsToDelete =new CCArray;
            CCARRAY_FOREACH(_evilDoers, jt) {
                CCSprite *target = dynamic_cast<CCSprite*>(jt);
                CCRect targetRect = CCRectMake(
                                               target->getPositionX() - (target->getContentSize().width/3),
                                               target->getPositionY() - (target->getContentSize().height/3),
                                               target->getContentSize().width,
                                               target->getContentSize().height);
                if (bulletRect.intersectsRect(targetRect)){
                    bulletsToDelete->addObject(bullet);
                    
                    CCSprite *hitSprite = CCSprite::create();
                    hitSprite->setPosition(bullet->getPosition());
                    hitSprite->setScale(0.5);
                    this->addChild(hitSprite, 3);
                    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
                    cache->addSpriteFramesWithFile("hit.plist");
                    CCArray* animFrames = CCArray::createWithCapacity(15);
                    char str[100] = {0};
                    for(int i = 1; i < 6; i++) {
                        sprintf(str, "Hit%0d.png", i);
                        CCSpriteFrame* frame = cache->spriteFrameByName( str );
                        animFrames->addObject(frame);
                    }
                    CCAnimation *animation = CCAnimation::createWithSpriteFrames(animFrames, 0.05f);
                    CCRemoveSelf *removeHit = CCRemoveSelf::create();
                    CCSequence *hitAnim = CCSequence::createWithTwoActions(CCAnimate::create(animation), removeHit);
                    hitSprite->runAction(hitAnim);
                    if (_enemyHP != 0) {
                        _enemyHP -= 1;
                        CCLog("hp: %i", _enemyHP);
                    }
                    else if (_enemyHP == 0) {
                        
                        CCSprite *explosion = CCSprite::create();
                        explosion->setPosition(ccp(_enemy->getPositionX(), _enemy->getPositionY()));
                        this->addChild(explosion, 1);
                        
                        CCSpriteFrameCache* expCache = CCSpriteFrameCache::sharedSpriteFrameCache();
                        expCache->addSpriteFramesWithFile("explosion2.plist");
                        char str2[100] = {0};
                        CCArray* expFrames = CCArray::createWithCapacity(18);
                        for(int i = 0; i < 19; i++) {
                            sprintf(str2, "explosion2_%0d.gif", i);
                            CCSpriteFrame* expFrame = expCache->spriteFrameByName(str2);
                            expFrames->addObject(expFrame);
                        }
                        CCAnimation* expAnimation = CCAnimation::createWithSpriteFrames(expFrames, 0.1f);
                        CCRemoveSelf *removeExplosion = CCRemoveSelf::create();
                        CCSequence *explosionAnim = CCSequence::createWithTwoActions(CCAnimate::create(expAnimation), removeExplosion);
                        explosion->runAction(explosionAnim);
                        targetsToDelete->addObject(target);
                        _enemyShadow->removeFromParentAndCleanup(true);
                        CCARRAY_FOREACH(targetsToDelete, jt){
                            CCSprite *target = dynamic_cast<CCSprite*>(jt);
                            _evilDoers->removeObject(target);
                            this->removeChild(target, true);
                        }
                        targetsToDelete->release();
                        _enemyHP = 20;
                        _enemyExists = false;
                        HelloWorld::enemyAppears();
                    }
                }
            }
        }
        
        CCARRAY_FOREACH(bulletsToDelete, it) {
            CCSprite* projectile = dynamic_cast<CCSprite*>(it);
            _bullets->removeObject(projectile);
            this->removeChild(projectile, true);
        }
        bulletsToDelete->release();
    }
    else {
        
    }
    
    if (_ship->getPositionY() < 70)_ship->setPositionY(71);
}

inline CCPoint locationInGLFromTouch(CCTouch &touch) {
    auto director = CCDirector::sharedDirector();
    return director -> convertToGL(touch.getLocationInView());
}

bool HelloWorld::ccTouchBegan(CCTouch *touch, CCEvent *event) {
    CCPoint touchLoc = this->getParent()->convertTouchToNodeSpace(touch);
    CCRect spriteRec = _ship->boundingBox();
    if (spriteRec.containsPoint(touchLoc)) {
        //CCLog("Bravo, touched on sprite %i", _ship->getTag());
        _shipTouched = true;
    }
    else {
    
    }
    return true;
}

void HelloWorld::ccTouchMoved(CCTouch *touch, CCEvent *event) {
    CCPoint location = touch->getLocationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);
    //CCLog("Location x:%f location y:%f", location.x, location.y);
    if (_shipTouched == true && _ship->getPositionY() > 70) {
        _ship->cocos2d::CCNode::setPosition(location.x, location.y);
        _shipShadow->setPosition(ccp(_ship->getPositionX()-5, _ship->getPositionY()-25));
    }
    else {

    }
}

void HelloWorld::ccTouchEnded(CCTouch *touch, CCEvent *event) {
    if (_shipTouched == true) {
        _shipTouched = false;
    }
    else {
        _bullet = CCSprite::create("Bullet.png");
        _bullets->addObject(_bullet);
        _bullet->setScale(0.2f);
        _bullet->setPosition(ccp(_ship->getPositionX(),_ship->getPositionY()+35));
        _bullet->setTag(10);
        this->addChild(_bullet, 2);
        
        CCPoint realdest =  ccp(_ship->getPositionX(), _ship->getPositionY()+500);
        CCPoint bulletPos = _bullet->getPosition();
        
        int offRealX = realdest.x - _bullet->getPosition().x;
        int offRealY = realdest.y - _bullet->getPosition().y;
        float length = sqrtf((offRealX*offRealX)+(offRealY*offRealY));
        float velocity = 460/1;
        float realMoveDuration = length/velocity;
        
        CCFiniteTimeAction *timeAction = CCSequence::create(CCMoveTo::create(realMoveDuration, realdest),
                                                            CCCallFuncN::create(this, callfuncN_selector(HelloWorld::spriteMoveFinished)), NULL);
        _bullet->runAction(timeAction);

    }
}

CCAnimation *HelloWorld::animeCreation(char *animeName, float animSpeed,
                                      int animeFrames, char *frameName, char *frameExtension) {
    
    CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    cache->addSpriteFramesWithFile(animeName);
    
    CCArray *animFrames = CCArray::createWithCapacity(animeFrames);
    char countPart[100] = {0};
    char comboPart[100] = {0};
    for(int i = 0; i < animeFrames; i++) {
        sprintf(countPart, "%0d", i);
        strcpy(comboPart, frameName);
        strcpy(comboPart, countPart);
        strcpy(comboPart, frameExtension);
        puts(comboPart);
        CCSpriteFrame* frame = cache->spriteFrameByName(comboPart);
        animFrames->addObject(frame);
    }
    
    CCAnimation *anime = CCAnimation::createWithSpriteFrames(animFrames, 0.1f);
    
    return anime;
}

void HelloWorld::spriteMoveFinished(CCNode *sender) {
    CCSprite *sprite = ((CCSprite *)sender);
    this->removeChild(sprite, true);
    
    if (sprite->getTag() == 10) {
        _bullets->removeObject(sprite);
    }
}

void HelloWorld::enemyAppears() {
    if (!_enemyExists) {
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        
        _enemy = CCSprite::create();
        _enemy->setPosition(ccp(winSize.width * 0.5, winSize.height * 1.2));
        _enemy->setTag(9);
        _evilDoers->addObject(_enemy);
        this->addChild(_enemy, 1);

        _enemyShadow = CCSprite::create();
        _enemyShadow->setPosition(ccp(_enemy->getPositionX()-5, _enemy->getPositionY()-25));
        _enemyShadow->setColor(ccBLACK);
        _enemyShadow->setOpacity(80);
        this->addChild(_enemyShadow, 0);
        
        CCSpriteFrameCache* enCache = CCSpriteFrameCache::sharedSpriteFrameCache();
        enCache->addSpriteFramesWithFile("enemy.plist");
        char str[100] = {0};
        CCArray* enFrames = CCArray::createWithCapacity(18);
        for(int i = 1; i < 17; i++) {
            sprintf(str, "enemy (%1d).gif", i);
            CCSpriteFrame* enFrame = enCache->spriteFrameByName( str );
            enFrames->addObject(enFrame);
        }
        CCAnimation* enAnimation = CCAnimation::createWithSpriteFrames(enFrames, 0.05f);
        _enemy->runAction(CCRepeatForever::create(CCAnimate::create(enAnimation)));
        _enemyShadow->runAction(CCRepeatForever::create(CCAnimate::create(enAnimation)));
        
        CCMoveTo *enemyMove = CCMoveTo::create(3, ccp(winSize.width * 0.5, winSize.height * 0.8));
        _enemy->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(2), enemyMove));
        _enemyExists = true;
    }
}

void HelloWorld::menuCloseCallback(CCObject* pSender) {
    CCDirector::sharedDirector()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
