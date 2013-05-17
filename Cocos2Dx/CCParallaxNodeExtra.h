//
//  CCParallaxNodeExtra.h
//  Cocos2Dx
//
//  Created by MinhNT on 13/05/17.
//
//

#ifndef __Cocos2Dx__CCParallaxNodeExtra__
#define __Cocos2Dx__CCParallaxNodeExtra__

#include <cocos2d.h>

USING_NS_CC;

class CCParallaxNodeExtra : public CCParallaxNode {
public:
    
    //Constructor
    CCParallaxNodeExtra();
    
    //To avoid cast and for safety
    static CCParallaxNodeExtra * node();
    
    //Facility method
    void incrementOffset(CCPoint offset, CCNode *node);
};

#endif