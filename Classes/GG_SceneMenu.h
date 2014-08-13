#ifndef ___GG_SCENE_MENU___
#define ___GG_SCENE_MENU___

#include "cocos2d.h"
#include "GG_DEFINE.h"
using namespace cocos2d;

class GG_SceneMenu : public cocos2d::CCScene
{
public:
	 bool init();
	 CREATE_FUNC(GG_SceneMenu);
	 
	 void gg_levelSelection(CCObject *obj);

	 void gg_levelSelectionInterval(float delay);

	 void gg_menuButtonTapped(CCObject *obj);	 
};

#endif 