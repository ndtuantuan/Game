#ifndef ___GG_SCENE_LOADING___
#define ___GG_SCENE_LOADING___

#include "cocos2d.h"
#include "GG_DEFINE.h"
using namespace cocos2d;

class GG_SceneLoading : public cocos2d::CCScene
{
public:
	 bool init();	 
	 CREATE_FUNC(GG_SceneLoading);	 
	 void gg_starGame(float delay);
protected:
	unsigned int short gg_p_count;
};

#endif 