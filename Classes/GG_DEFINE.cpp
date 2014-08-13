#include "GG_DEFINE.h"
using namespace cocos2d;

//MACRO
//TOUCH
void GG_ADD_TARGET_TOUCH_DELEGATE(CCTouchDelegate *touchAbleObject,unsigned int short priority,bool swallow)
{
	static CCDirector *director = CCDirector::sharedDirector();
	director->getTouchDispatcher()->addTargetedDelegate( touchAbleObject,priority,swallow);
}

void GG_REMOVE_TOUCH_DELEGATE(CCTouchDelegate *touchAbleObject)
{
	static CCDirector *director = CCDirector::sharedDirector();
	director->getTouchDispatcher()->removeDelegate(touchAbleObject);
}
//LOAD PLIST FILE
void GG_LOAD_PLIST_FRAMES(const char *plist)
{
	static CCSpriteFrameCache *frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
	frameCache->addSpriteFramesWithFile(plist);	
}
//CREATE ANIMATION FROM PLIST

CCAnimate* GG_CREATE_ANIMATE_FROM_FRAMES(const char *key , unsigned int short starFrame, unsigned int short endFrame,float interval)
{
	 static CCSpriteFrameCache *frameCache = CCSpriteFrameCache::sharedSpriteFrameCache(); 
	 
	 unsigned int short totalFrame = endFrame-starFrame+1;
	 CCArray* animFrames = CCArray::createWithCapacity( totalFrame );
	 //CCLOG("Create animate include %i frames ", totalFrame );
     char str[50] = {0};
	 for( unsigned int short i = starFrame; i <= endFrame ; i++ ) 
     {
         sprintf(str, "%s_%i.png", key , i);
		 ///CCLOG( "Create animate from frame %s" , str);         
		 animFrames->addObject( frameCache->spriteFrameByName(str) );
      }
	 //CCLOG("-------------------------------------------");     	 
	 
	 return CCAnimate::create(CCAnimation::createWithSpriteFrames(animFrames,interval));
}

//CLASSES
void GG_Sprite::gg_initFile(const char *key)
{
	static CCTextureCache *textureCache = CCTextureCache::sharedTextureCache();
	this->initWithTexture( textureCache->addImage(key) );
}