#ifndef ____GG__DEFINE____
#define ____GG__DEFINE____
#include "cocos2d.h"
using namespace cocos2d;

//CONSTANTS
#define GG_SCR_W (int)(CCDirector::sharedDirector()->getWinSize().width)
#define GG_SCR_H (int)(CCDirector::sharedDirector()->getWinSize().height)

//MACROS
void GG_ADD_TARGET_TOUCH_DELEGATE(CCTouchDelegate *touchAbleObject,unsigned int short priority=0,bool swallow=true);
void GG_REMOVE_TOUCH_DELEGATE(CCTouchDelegate *touchAbleObject);
void GG_LOAD_PLIST_FRAMES(const char *plist);
CCAnimate* GG_CREATE_ANIMATE_FROM_FRAMES(const char *key , unsigned int short starFrame, unsigned int short endFrame,float interval);

///CLASSES
class  GG_Sprite : public cocos2d::CCSprite
{
public:
	void gg_initFile(const char *key);	
	CREATE_FUNC(GG_Sprite);

};

#endif;