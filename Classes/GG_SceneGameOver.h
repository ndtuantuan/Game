#ifndef ___GG_SCENE_OVER___
#define ___GG_SCENE_OVER___

#include "cocos2d.h"
#include "GG_DEFINE.h"
using namespace cocos2d;

class GG_SceneGameOver : public cocos2d::CCScene
{
public:
	 bool init();	 
	 CREATE_FUNC(GG_SceneGameOver);
	 void gg_menuButtonTapped(CCObject *obj);
};

#endif 