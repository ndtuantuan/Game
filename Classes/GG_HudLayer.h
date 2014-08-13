#ifndef ___GG_HUD_LAYER___
#define ___GG_HUD_LAYER___

#include "cocos2d.h"
#include "cocos-ext.h"
using namespace cocos2d;
USING_NS_CC;
USING_NS_CC_EXT;

class GG_HudLayer : public CCLayer
{
private:
    int health;
public:
	 bool init();
    GG_HudLayer();

	 CREATE_FUNC(GG_HudLayer);
    CCProgressTimer *healBar;
    void updateHealth(int heal);
      
	 
};

#endif 