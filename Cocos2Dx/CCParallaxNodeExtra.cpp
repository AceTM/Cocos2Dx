//
//  CCParallaxNodeExtra.cpp
//  Cocos2Dx
//
//  Created by MinhNT on 13/05/17.
//
//

#include "CCParallaxNodeExtra.h"

//Hack to access CCPointObject (which is not public)
class CGPointObject : CCObject {
    CC_SYNTHESIZE(CCPoint, m_tRatio, Ratio)
    CC_SYNTHESIZE(CCPoint, m_tOffset, Offset)
    CC_SYNTHESIZE(CCNode *, m_pChild, Child) //weak ref
};

//Providing costructor
CCParallaxNodeExtra::CCParallaxNodeExtra() {
    CCParallaxNode();
}

CCParallaxNodeExtra * CCParallaxNodeExtra::node() {
    return new CCParallaxNodeExtra();
}

void CCParallaxNodeExtra::incrementOffset(CCPoint offset, CCNode* node) {
    for (unsigned int i = 0; i < m_pParallaxArray-> num; i++) {
        CGPointObject *point = (CGPointObject *)m_pParallaxArray->arr[i];
        CCNode *curNode = point->getChild();
        
        if (curNode->isEqual(node)) {
            point->setOffset(ccpAdd(point->getOffset(), offset));
            break;
        }
    }
}
